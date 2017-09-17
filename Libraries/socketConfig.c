/*
 * socketConfig.c
 *
 *  Created on: 6/5/2017
 *      Author: utnso
 */

#include "socketConfig.h"

/**  @NAME: _configurar_addrinfo
 *	 @DESC: Retorna un puntero a una addrInfo totalmente lista para usar,
 *	 dado una IP y un Host cualesquiera.
 *	 SOLO para uso interno.
 */

struct addrinfo* _configurar_addrinfo(char *IP, char* Port) {
	struct addrinfo hints;
	struct addrinfo* serverInfo;
	int16_t error;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (!strcmp(IP, "localhost")) {
		hints.ai_flags = AI_PASSIVE;
		error = getaddrinfo(NULL, Port, &hints, &serverInfo);
	} else
		error = getaddrinfo(IP, Port, &hints, &serverInfo);
	if (error) {
		error_show("Problema con el getaddrinfo()!: %s\n", gai_strerror(error));
		exit(EXIT_FAILURE);
	}
	return serverInfo;
}

un_socket conectar_a(char *IP, char* Port) {
	struct addrinfo* serverInfo = _configurar_addrinfo(IP, Port);
	if (serverInfo == NULL) {
		exit(EXIT_FAILURE);
	}
	int serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol);
	if (serverSocket == -1) {
		error_show("\n No se pudo conectar\n",
		errno);
		exit(EXIT_FAILURE);
	}
	if (connect(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen)
			== -1) {
		puts("\n");
		error_show(
				"No se pudo conectar con el proceso que hace de servidor, error: %d\n",
				errno);
		close(serverSocket);
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(serverInfo);
	return serverSocket;
}

un_socket socket_escucha(char* IP, char* Port) {

	struct addrinfo* serverInfo = _configurar_addrinfo(IP, Port);

	if (serverInfo == NULL) {
		exit(EXIT_FAILURE);
	}

	int socketEscucha;

	socketEscucha = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol);

	int enable = 1;setsockopt(socketEscucha, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));//forzar la reutilizacion del socket
	bind(socketEscucha, serverInfo->ai_addr, serverInfo->ai_addrlen);

	freeaddrinfo(serverInfo);

	return socketEscucha;
}

void enviar(un_socket socket_para_enviar, int codigo_operacion, int tamanio,
		void * data) {

	int tamanio_paquete = 2 * sizeof(int) + tamanio;
	void * buffer = malloc(tamanio_paquete);

	memcpy(buffer, &codigo_operacion, sizeof(int));
	memcpy(buffer + sizeof(int), &tamanio, sizeof(int));
	memcpy(buffer + 2 * sizeof(int), data, tamanio);

	send(socket_para_enviar, buffer, tamanio_paquete, MSG_WAITALL);

	free(buffer);

}

t_paquete* recibir(un_socket socket_para_recibir) {

	t_paquete * paquete_recibido = malloc(sizeof(t_paquete));
	int retorno = 0;
	retorno = recv(socket_para_recibir, &paquete_recibido->codigo_operacion, sizeof(int),
	MSG_WAITALL);

	if(retorno==0){
		paquete_recibido->codigo_operacion=-1;
		void * informacion_recibida = malloc(sizeof(int));
		paquete_recibido->data = informacion_recibida;
		return paquete_recibido;

	}
	recv(socket_para_recibir, &paquete_recibido->tamanio, sizeof(int),
	MSG_WAITALL);

	void * informacion_recibida = malloc(paquete_recibido->tamanio);

	recv(socket_para_recibir, informacion_recibida, paquete_recibido->tamanio,
	MSG_WAITALL);

	paquete_recibido->data = informacion_recibida;

	return paquete_recibido;
}

un_socket aceptar_conexion(un_socket socket_escuchador) {

	struct sockaddr_storage remoteaddr;

	socklen_t addrlen;

	addrlen = sizeof remoteaddr;

	un_socket nuevo_socket = accept(socket_escuchador,
			(struct sockaddr *) &remoteaddr, &addrlen);

	return nuevo_socket;
}

void liberar_paquete(t_paquete * paquete) {
	free(paquete->data);
	free(paquete);
}

bool realizar_handshake(un_socket socket_del_servidor, int cop_handshake) {

	char * mensaje = malloc(18);
	mensaje = "Inicio autenticacion";

	enviar(socket_del_servidor, cop_handshake, 21, mensaje);

	t_paquete * resultado_del_handhsake = recibir(socket_del_servidor);

	bool resultado = string_equals_ignore_case(
			(char *) resultado_del_handhsake->data, "Autenticado");

	liberar_paquete(resultado_del_handhsake);

	return resultado;

}

bool esperar_handshake(un_socket socket_del_cliente, t_paquete* inicio_del_handshake, int cop_handshake) {

	bool resultado = string_equals_ignore_case(
			(char *) inicio_del_handshake->data, "Inicio autenticacion");

	liberar_paquete(inicio_del_handshake);

	if (resultado) {

		char * respuesta = malloc(12);
		respuesta = "Autenticado";
		enviar(socket_del_cliente, cop_handshake, 12, respuesta);

	} else {

		char * respuesta = malloc(6);
		respuesta = "Error";
		enviar(socket_del_cliente, cop_handshake, 6, respuesta);

	}

	return resultado;
}

char get_campo_config_char(t_config* archivo_configuracion, char* nombre_campo) {
	char* valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_string_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

int get_campo_config_int(t_config* archivo_configuracion, char* nombre_campo) {
	int valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_int_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %i\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}


char* get_campo_config_string(t_config* archivo_configuracion, char* nombre_campo) {
	char* valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_string_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}


char** get_campo_config_array(t_config* archivo_configuracion, char* nombre_campo) {
	char** valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_array_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

void enviar_archivo(un_socket socket, char* path){
	FILE *fp = fopen (path, "r");
	fseek(fp, 0, SEEK_END);
	int tamanioArchivo = sizeof(char) * ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* archivo = malloc(tamanioArchivo+1);
	fread(archivo, tamanioArchivo, 1, fp);
	enviar(socket, cop_archivo_programa, tamanioArchivo+1, archivo);
	fclose (fp);
	free(archivo);
}

bool comprobar_archivo(char* path){
	bool retorno;
	if(access(path, R_OK) == 1){
		printf("Archivo Validado");
		retorno = true;
	}else{
		printf("No se pudo leer/encontrar el archivo"); //hace falta imprimir un codigo de error y una descrip?
	}
	return retorno;
}

