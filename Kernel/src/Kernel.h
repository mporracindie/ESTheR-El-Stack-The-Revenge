/*
 * Kernel.h
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <socketConfig.h>
#include <commons/config.h>
#include <sys/inotify.h>

#define TRUE   1
#define FALSE  0

//ESTRUCTURA ARCHIVO CONFIGURACION
typedef struct kernell_configuracion {
	char* PUERTO_PROG;
	char* PUERTO_CPU;
	char* IP_MEMORIA;
	char* PUERTO_MEMORIA;
	char* IP_FS;
	char* PUERTO_FS;
	int QUANTUM;
	int QUANTUM_SLEEP;
	char* ALGORITMO;
	int GRADO_MULTIPROG;
	int STACK_SIZE;
	char** SEM_IDS;
	char** SEM_INIT;
	char** SHARED_VARS;

} kernell_configuracion;

const char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Kernel/config-kernell.cfg";

typedef struct tablaArchivosAbiertos{
	char* nombreArchivo;
	int descriptor;
	void* siguiente;
}tablaArchivosAbiertos;

typedef struct PCBKernel{
	int pid;
	tablaArchivosAbiertos tabla;
}PCBKernel;

kernell_configuracion get_configuracion() {
	printf("Inicializando proceso Kernel\n");
	kernell_configuracion configuracion;
	// Obtiene el archivo de configuracion
	t_config* archivo_configuracion = config_create(path);
	configuracion.PUERTO_PROG = get_campo_config_string(archivo_configuracion, "PUERTO_PROG");
	configuracion.PUERTO_CPU = get_campo_config_string(archivo_configuracion, "PUERTO_CPU");
	configuracion.IP_MEMORIA = get_campo_config_string(archivo_configuracion, "IP_MEMORIA");
	configuracion.PUERTO_MEMORIA = get_campo_config_string(archivo_configuracion, "PUERTO_MEMORIA");
	configuracion.IP_FS = get_campo_config_string(archivo_configuracion, "IP_FS");
	configuracion.PUERTO_FS = get_campo_config_string(archivo_configuracion, "PUERTO_FS");
	configuracion.QUANTUM = get_campo_config_int(archivo_configuracion, "QUANTUM");
	configuracion.QUANTUM_SLEEP = get_campo_config_int(archivo_configuracion, "QUANTUM_SLEEP");
	configuracion.ALGORITMO = get_campo_config_string(archivo_configuracion, "ALGORITMO");
	configuracion.GRADO_MULTIPROG = get_campo_config_int(archivo_configuracion, "GRADO_MULTIPROG");
	configuracion.SEM_IDS = get_campo_config_array(archivo_configuracion, "SEM_IDS");
	configuracion.SEM_INIT = get_campo_config_array(archivo_configuracion, "SEM_INIT");
	configuracion.SHARED_VARS = get_campo_config_array(archivo_configuracion, "SHARED_VARS");
	configuracion.STACK_SIZE = get_campo_config_int(archivo_configuracion, "STACK_SIZE");
	return configuracion;
}

#endif /* KERNEL_H_ */
