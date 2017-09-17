#include "Kernel.h"

int main(void){
	printf("Inicializando proceso Kernel\n");
	kernell_configuracion configuracion = get_configuracion();
	procesos procesos;

//--------------------CONEXION CON MEMORIA-----------------------------
	un_socket socketMemoria = conectar_a(configuracion.IP_MEMORIA,configuracion.PUERTO_MEMORIA);
	realizar_handshake(socketMemoria, cop_handshake_kernel);
	proceso_memoria nodo_memoria;
	procesos.memoria = nodo_memoria;

//---------------------------------------------------------------------

	fd_set listaOriginal;
	fd_set listaAux;
	FD_ZERO(&listaOriginal);
	FD_ZERO(&listaAux);
	int fd_max = 1;
	int newfd;        // newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	int nbytes;
	char remoteIP[INET6_ADDRSTRLEN];
	int yes=1;        // for setsockopt() SO_REUSEADDR, below
	int socketActual, j, rv;
	struct addrinfo hints, *ai, *p;
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(configuracion.PUERTO_PROG);
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

//--------------------CONFIGURACION EN RUNTIME-------------------------
	int update_config = inotify_init();

	FD_SET(update_config,&listaOriginal);

	if(update_config > socketServer) {
		fd_max = update_config;
	} else {
		fd_max = socketServer;
	}

	inotify_add_watch(update_config, path, IN_MODIFY);
//---------------------------------------------------------------------

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
	            printf("Kernel recibio una nueva conexion\n");

			} else if(socketActual == update_config){ //considerar que cambio ocurrio
				configuracion = get_configuracion(); //actualiza la config
			} else {
				t_paquete* paqueteRecibido = recibir(socketActual);
				switch(paqueteRecibido->codigo_operacion){ //revisar validaciones de habilitados
					case cop_handshake_consola:
						esperar_handshake(socketActual, paqueteRecibido, cop_handshake_kernel);
						proceso_consola* nuevo_nodo_consola = malloc(sizeof(proceso_consola));
						proceso_consola* auxiliar_consola;
						proceso_consola* ultimo_nodo_consola;
						auxiliar_consola = procesos.consolas;
						while(auxiliar_consola != NULL){
							ultimo_nodo_consola = auxiliar_consola;
							auxiliar_consola = (proceso_consola*)auxiliar_consola->siguiente;
						}
						if(ultimo_nodo_consola == NULL){
							procesos.consolas = nuevo_nodo_consola;
						}else{
							ultimo_nodo_consola->siguiente = nuevo_nodo_consola;
						}
						nuevo_nodo_consola->habilitado = true;
						nuevo_nodo_consola->socket = socketActual;
				    break;

					case cop_handshake_cpu:
						esperar_handshake(socketActual, paqueteRecibido, cop_handshake_kernel);
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
					break;
				}
			}
		}
		listaAux = listaOriginal;
	}// for
}//while

void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {return &(((struct sockaddr_in*)sa)->sin_addr);}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
	return 0;
}

void planificador(char* modo){
	return;
}


