/*
 * socketConfig.h
 *
 *  Created on: 6/5/2017
 *      Author: utnso
 */

#ifndef SOCKETCONFIG_H_
#define SOCKETCONFIG_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <commons/string.h>
#include <commons/error.h>
#include <commons/config.h>
#include <unistd.h>

enum codigos_de_operacion {
	cop_generico = 0,
	cop_handshake_consola = 1,
	cop_handshake_cpu = 2,
	cop_handshake_memoria = 3,
	cop_handshake_fileSystem = 4,
	cop_handshake_kernel = 5,
	cop_archivo_programa = 6,
	cop_imprimi = 7,
	cop_terminar_proceso = 8,
	cop_handshake_consolaInterfazUsuario = 9,
	cop_obtener_direccion = 10,
	cop_memoria_inicializarPrograma = 11,
	cop_memoria_solicitarBytes = 12,
	cop_memoria_almacenarBytes = 13,
	cop_memoria_asignarPaginas = 14,
	cop_memoria_finalizarPrograma = 15,
	cop_filesystem_validarArchivo = 16,
	cop_filesystem_crearArchivo = 17,
	cop_filesystem_borrar = 18,
	cop_filesystem_obtenerDatos = 19,
	cop_filesystem_guardarDatos = 20

//cop_envio_pid = , es necesario o podemos asumir con seguridad que el kernel nos va a mandar el pid?
};

typedef int un_socket;
typedef struct {
	int codigo_operacion;
	int tamanio;
	void * data;
} t_paquete;

typedef struct{
	int pid;
	int paginas;
} t_parametros_memoria_inicializar;

typedef struct{
	int pid;
	int numero_pagina;
	int offset;
	int tamanio;
} t_parametros_memoria_leer;

typedef struct{
	int pid;
	int paginas;
} t_parametros_memoria_asignar;

typedef struct{
	int pid;
	int offset;
	int numero_pagina;
	int tamanio;
	void* buffer
} t_parametros_memoria_escribir;

typedef struct{
	int pid;
} t_parametros_memoria_finalizar;

enum estados_proceso {
	estado_new = 0,
	estado_ready = 1,
	estado_exit = 2,
	estado_exec = 3,
	estado_block = 4
};

//------------ESTRUCTURAS PROCESOS----------------
typedef struct indiceDeCodigo{
	int posicion;
	int longitud;
	void* siguiente;
}indiceDeCodigo;

typedef struct indiceDeEtiquetas{

}indiceDeEtiquetas;

typedef struct posicionMemoria{
	int pagina;
	int offset;
	int size;
}posicionMemoria;

typedef struct argumentos{
	posicionMemoria argumento;
	void* siguiente;
}argumentos;

typedef struct variables{
	char nombreVar;
	posicionMemoria posMem;
	void* siguiente;
}variables;

typedef struct indiceDeStack{
	argumentos argumentos;
	variables variables;
	int retPos; //posision de retorno
	posicionMemoria retVal; //posicion del return value
}indiceDeStack;

typedef struct pcb{
	int PID;// Identificador del proceso
	int ProgramCounter;// Program Counter
	int PaginasCodigo; //Paginas de codigo
	int ExitCode;// Exit Code
	indiceDeCodigo indiceDeCodigo;
	indiceDeEtiquetas indiceDeEqtiquetas;
	indiceDeStack indiceDeStack;
}pcb;

typedef struct listaDePCBs{
	pcb* pcb;
	void* siguiente;
}listaDePCBs;

typedef struct proceso_consola {
	bool habilitado;
	int socket;
	void* siguiente;
} proceso_consola;

typedef struct proceso_cpu {
	bool habilitado;
	int socket;
	int estado;// para saber si esta corrriendo algo
	void* siguiente;
} proceso_cpu;

typedef struct proceso_memoria {
	int socket;
} proceso_memoria;

typedef struct proceso_fileSystem {
	int socket;
} proceso_fileSystem;

typedef struct proceso_kernel {
	int socket;
} proceso_kernel;

typedef struct procesos {
	proceso_consola* consolas;
	proceso_cpu* cpus;
	proceso_memoria memoria;
	proceso_fileSystem fileSystem;
	proceso_kernel kernel;
} procesos;

typedef struct manejoFileSystem {
	char* path;
	int offset; //desplazamiento
	int size;
	void* buffer; //char* o void*??
} manejoFileSystem;
//------------FIN ESTRUCTURAS PROCESOS----------------

/**	@NAME: conectar_a
 * 	@DESC: Intenta conectarse.
 * 	@RETURN: Devuelve el socket o te avisa si hubo un error al conectarse.
 *
 */

un_socket conectar_a(char *IP, char* Port);

/**	@NAME: socket_escucha
 * 	@DESC: Configura un socket que solo le falta hacer listen.
 * 	@RETURN: Devuelve el socket listo para escuchar o te avisa si hubo un error al conectarse.
 *
 */

un_socket socket_escucha(char* IP, char* Port);

/**	@NAME: enviar
 * 	@DESC: Hace el envio de la data que le pasamos. No hay que hacer más nada.
 *
 */

void enviar(un_socket socket_envio, int codigo_operacion, int tamanio,
		void * data);

/**	@NAME: recibir
 * 	@DESC: devuelve un paquete que está en el socket pedido
 *
 */
t_paquete* recibir(un_socket socket_para_recibir);

/**	@NAME: aceptar_conexion
 * 	@DESC: devuelve el socket nuevo que se quería conectar
 *
 */
un_socket aceptar_conexion(un_socket socket_escuchador);

/**	@NAME: liberar_paquete
 * 	@DESC: libera el paquete y su data.
 *
 */
void liberar_paquete(t_paquete * paquete);

/**	@NAME: realizar_handshake
 *
 */
bool realizar_handshake(un_socket socket_del_servidor, int cop_handshake);

/**	@NAME: esperar_handshake
 *
 */
bool esperar_handshake(un_socket socket_del_cliente,
		t_paquete* inicio_del_handshake, int cop_handshake);

char get_campo_config_char(t_config* archivo_configuracion, char* nombre_campo);

int get_campo_config_int(t_config* archivo_configuracion, char* nombre_campo);

char** get_campo_config_array(t_config* archivo_configuracion,
		char* nombre_campo);

char* get_campo_config_string(t_config* archivo_configuracion,
		char* nombre_campo);

/**	@NAME: enviar_archivo
 *
 */
void enviar_archivo(un_socket socket, char* path);

bool comprobar_archivo(char* path);

#endif /* SOCKETCONFIG_H_ */
