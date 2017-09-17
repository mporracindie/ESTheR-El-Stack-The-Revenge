#include "Memory.h"


int main(void) {
	memory_configuracion configuracion = get_configuracion();
	procesos procesos;
	inicializarMemoria(configuracion);
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	int nbytes;
	char remoteIP[INET6_ADDRSTRLEN];
	int yes=1;        // for setsockopt() SO_REUSEADDR, below
	int socketActual, j, rv;
	struct addrinfo hints, *ai, *p;
	struct sockaddr_in direccionServidor;
	fd_set listaOriginal;
	fd_set listaAux;
	FD_ZERO(&listaOriginal);
	FD_ZERO(&listaAux);
	int fd_max = 1;
	int newfd;

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(configuracion.PUERTO);

	un_socket socketServer = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	int error=0;
	error=bind(socketServer, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor));
	if(error <0)
		perror('error en bind');
		error=listen(socketServer, 256);
	if(error <0)
		perror('error en listen');

	FD_SET(socketServer,&listaOriginal);
	fd_max=socketServer;

	while(1){
		listaAux = listaOriginal;
		select(fd_max + 1, &listaAux, NULL, NULL, NULL);
		for(socketActual = 0; socketActual <= fd_max; socketActual++) {
			if (FD_ISSET(socketActual, &listaAux)) {
				if (socketActual == socketServer) { //es una conexion nueva
					newfd = aceptar_conexion(socketActual);
					FD_SET(newfd, &listaOriginal); //Agregar al master SET
					if (newfd > fd_max) {    //Update el Maximo
						fd_max = newfd;
					}
		            printf("Memoria recibio una nueva conexion\n");

				} else {
					t_paquete* paqueteRecibido = recibir(socketActual);
					switch(paqueteRecibido->codigo_operacion){ //revisar validaciones de habilitados
						case cop_handshake_kernel: //validar que no haya ya un kernel
							pthread_t hiloKernel;
							FD_CLR(socketActual, &listaOriginal);
							pthread_create(&hiloKernel, NULL, hiloKernel_memoria, int socket_escucha);

					    break;

						case cop_handshake_cpu:
							pthread_t hiloCpu;
							FD_CLR(socketActual, &listaOriginal);
							pthread_create(&hiloCpu, NULL, hiloCpu_memoria, int socket_escucha);

							/*
							esperar_handshake(socketActual, paqueteRecibido, cop_handshake_memoria);
							proceso_cpu* nuevo_nodo_cpu = malloc(sizeof(proceso_cpu));
							proceso_cpu* auxiliar_cpu;
							proceso_cpu* ultimo_nodo_cpu;
							auxiliar_cpu = procesos.cpus;
							while(auxiliar_cpu != NULL){
								ultimo_nodo_cpu = auxiliar_cpu;
								auxiliar_cpu = (proceso_cpu*)auxiliar_cpu->siguiente;
							}
							if(ultimo_nodo_cpu == NULL){
								procesos.cpus = nuevo_nodo_cpu;
							}else{
								ultimo_nodo_cpu->siguiente = nuevo_nodo_cpu;
							}
							nuevo_nodo_cpu->habilitado = true;
							nuevo_nodo_cpu->socket = socketActual;
							*/
						break;

					}
				}
			}
			listaAux = listaOriginal;
		}// for
	}//while

	return 0;
}


//------------------------------------hilo para el manejo del kernel---------------------------
void hiloKernel_memoria(int puertoEscucha){
	while(1){

		t_paquete* paqueteRecibido = recibir(puertoEscucha);
		switch(paqueteRecibido->codigo_operacion){
		case cop_memoria_inicializarPrograma:
		{
			t_parametros_memoria_inicializar* parametros = ((t_parametros_memoria_inicializar)paqueteRecibido->data);
			tabla_de_paginas* tabla_aux=tabla_de_paginas;
			int cantidadAsignadas = 0;
			for(int i=0; i< parametros->paginas; i++){
				//int numeroFrame=LLAMAR funcion hashing(parametros.pid, i)
				//
			}
			while(tabla_aux->siguiente != NULL &&  cantidadAsignadas < parametros->paginas)
			{
				if (tabla_aux->pid == -1){
					tabla_aux->pid=parametros->pid;
					tabla_aux->numero_pagina = cantidadAsignadas;
					cantidadAsignadas++;
				}

				tabla_aux=tabla_aux->siguiente;
			}
		}
		break;
		case cop_memoria_finalizarPrograma:
		{
			t_parametros_memoria_finalizar* parametros = ((t_parametros_memoria_finalizar)paqueteRecibido->data);
		}
		break;
		}
	}
}

tabla_de_paginas* BuscarFrameLibre(){

	tabla_de_paginas* tabla_aux=tabla_de_paginas;
	int cantidadAsignadas = 0;
	while(tabla_aux->siguiente != NULL)
	{
		if (tabla_aux->pid == -1){
			return tabla_aux;
		}
	}
}

//------------------------------------hilo para el manejo de la cpu--------------------------
void hiloCpu_memoria(int puertoEscucha){
	while(1){

		t_paquete* paqueteRecibido = recibir(puertoEscucha);
		switch(paqueteRecibido->codigo_operacion){
		case cop_memoria_solicitarBytes:
		{
			t_parametros_memoria_leer* parametros = ((t_parametros_memoria_leer)paqueteRecibido->data);
		}
		break;
		case cop_memoria_almacenarBytes:
		{
			t_parametros_memoria_escribir* parametros = ((t_parametros_memoria_escribir)paqueteRecibido->data);
		}
		break;
		case cop_memoria_asignarPaginas:
		{
			t_parametros_memoria_asignar* parametros = ((t_parametros_memoria_asignar)paqueteRecibido->data);
		}
		break;
		}
	}
}

void inicializarMemoria (memory_configuracion configuracion){
	bloqueMemoria = malloc (configuracion.MARCOS * configuracion.MARCOS_SIZE);
	generarEstructurasAdministrativas (bloqueMemoria, configuracion);
}

void generarEstructurasAdministrativas (void *memoria, memory_configuracion configuracion){
	tabla_de_paginas* tabla = malloc( (configuracion.MARCOS * (sizeof(tabla_de_paginas)) / configuracion.MARCOS_SIZE +1) * configuracion.MARCOS_SIZE);
	tabla_de_paginas* auxiliarTabla = tabla;
	for (int i=0; i<configuracion.MARCOS; i++){
		auxiliarTabla->pid= -1;
		auxiliarTabla->numero_pagina= -1;
		auxiliarTabla->numero_frame= i;
		auxiliarTabla=auxiliarTabla->siguiente;
	}


	memcpy(bloqueMemoria, tabla,(configuracion.MARCOS * (sizeof(tabla_de_paginas)) / configuracion.MARCOS_SIZE +1) * configuracion.MARCOS_SIZE);
    tabla_de_paginas=bloqueMemoria;
}
