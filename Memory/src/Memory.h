/*
 * Memory.h
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <socketConfig.h>
#include <pthread.h>

#define TRUE   1
#define FALSE  0

typedef struct memory_configuracion {
	int PUERTO;
	int MARCOS;
	int MARCOS_SIZE;
	int ENTRADAS_CACHE;
	int CACHE_X_PROC;
	int RETARDO_MEMORIA;
} memory_configuracion;

typedef struct tabla_de_paginas {
	int pid;
	int numero_pagina;
	int numero_frame;
	void* siguiente;
} tabla_de_paginas;


void* bloqueMemoria;
tabla_de_paginas* tabla_de_paginas;
memory_configuracion get_configuracion() {
	puts("Inicializando proceso Memory\n");
	memory_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Memory/config-memory.cfg";
	t_config* archivo_configuracion = config_create(path);
	configuracion.PUERTO = get_campo_config_int(archivo_configuracion, "PUERTO");
	configuracion.MARCOS = get_campo_config_int(archivo_configuracion, "MARCOS");
	configuracion.MARCOS_SIZE = get_campo_config_int(archivo_configuracion, "MARCOS_SIZE");
	configuracion.ENTRADAS_CACHE = get_campo_config_int(archivo_configuracion, "ENTRADAS_CACHE");
	configuracion.CACHE_X_PROC = get_campo_config_int(archivo_configuracion, "CHACHE_X_PROC");
	configuracion.RETARDO_MEMORIA = get_campo_config_int(archivo_configuracion, "RETARDO_MEMORIA");

	bloqueMemoria = malloc(configuracion.MARCOS * configuracion.MARCOS_SIZE);
	return configuracion;
}

#endif /* MEMORY_H_ */
