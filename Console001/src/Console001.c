#include "Console001.h"

int main(void) {
	un_socket kernel = conectar_a(configuracion.IP_KERNEL,configuracion.PUERTO_KERNEL);
	realizar_handshake(kernel, cop_handshake_consolaInterfazUsuario);


	configuracion = get_configuracion();
	while(1){
		printf("Ingrese la opcion deseada:\n1) Iniciar Programa\n2) Finalizar Programa\n3) Desconectar Consola\n4) Limpiar Mensajes\n");
		int seleccion;
		scanf("%i\n",seleccion);
		switch(seleccion){
			case iniciarPrograma: {
				printf("Ingrese el path del archivo a ejecutar:");
				char* path= malloc(255); //hace falta malloc?
				scanf("%s\n",path);
				if(comprobar_archivo(path)){
					pthread_t hilo;
					pthread_create(&hilo, NULL,hiloPrograma,(void*)path);
					//como logro que identifique a cada hilo de forma distinta
					//pthread_create();
				}
			}
				break;
			case finalizarPrograma:
				printf("Ingrese el pid del archivo a finalizar:");
				int pid;
				scanf("%s\n",pid);
				enviar(kernel, 8, sizeof(pid), pid);
				break;

			case desconectarConsola:
				//finalizar todos los hilos
				while(programasActuales.programa != NULL){
					enviar(kernel, 8, sizeof(pid), pid);
					pthread_join(programasActuales.programa->threadID,NULL);
					programasActuales.programa= programasActuales.programa->siguiente;
				}
				break;
			case limpiarConsola:
				system("clear");
				break;
		}
	}


	return 0;
}

//-------------------------HILO PROGRAMA--------------------------------------
void hiloPrograma(char* path){
	un_socket kernel = conectar_a(configuracion.IP_KERNEL,configuracion.PUERTO_KERNEL);
	realizar_handshake(kernel, cop_handshake_consola);
	enviar_archivo(kernel, path);

	t_paquete* pid = recibir(kernel);
	t_programa* nuevo_nodo = malloc(sizeof(t_programa));
	t_programa* auxiliar;
	t_programa* ultimo_nodo;

	auxiliar = programasActuales.programa;
	while(auxiliar != NULL){
		ultimo_nodo = auxiliar;
		auxiliar = (t_programa*)auxiliar->siguiente;
	}
	if(ultimo_nodo == NULL){
		programasActuales.programa = nuevo_nodo;
	}else{
		ultimo_nodo->siguiente = nuevo_nodo;
	}
	nuevo_nodo->processID = (int)pid->data;
	nuevo_nodo->threadID = (int)pthread_self();

	while(1){
		t_paquete* paqueteRecibido = recibir(kernel);
		switch(paqueteRecibido->codigo_operacion){
			case cop_imprimi:
				printf("%s\n",(char*)paqueteRecibido->data);
				break;
			case cop_terminar_proceso:{
				programas* auxiliarAnterior = programasActuales.programa;
				int posicion = 0;
				while(programasActuales.programa->processID != pid){
					auxiliarAnterior = programasActuales.programa;
					programasActuales.programa = programasActuales.programa->siguiente;
				}
				free(programasActuales.programa);
				liberar_paquete(paqueteRecibido);
				return;
			}
				break;
		}
		liberar_paquete(paqueteRecibido);
	}


}


