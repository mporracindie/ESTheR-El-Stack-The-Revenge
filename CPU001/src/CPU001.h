/*
 * CPU001.h
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#ifndef CPU001_H_
#define CPU001_H_

#include <socketConfig.h>
#include <commons/config.h>
#include <parser/metadata_program.h>

#define TRUE   1
#define FALSE  0

//ESTRUCTURA ARCHIVO CONFIGURACION
typedef struct cpu_configuracion {
	char* PUERTO_KERNEL;
	char* IP_KERNEL;
	char* PUERTO_MEMORIA;
	char* IP_MEMORIA;
} cpu_configuracion;

cpu_configuracion get_configuracion() {
	printf("Inicializando proceso CPU\n");
	cpu_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/CPU001/config-cpu.cfg";
	t_config* archivo_configuracion = config_create(path);
	configuracion.PUERTO_KERNEL = get_campo_config_string(archivo_configuracion, "PUERTO_KERNEL");
	configuracion.IP_KERNEL = get_campo_config_string(archivo_configuracion, "IP_KERNEL");
	configuracion.IP_MEMORIA = get_campo_config_string(archivo_configuracion, "IP_MEMORIA");
	configuracion.PUERTO_MEMORIA = get_campo_config_string(archivo_configuracion, "PUERTO_MEMORIA");
	return configuracion;
}

#endif /* CPU001_H_ */
