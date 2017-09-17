/*
 * Console001.h
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#ifndef CONSOLE001_H_
#define CONSOLE001_H_

#include <socketConfig.h>
#include <pthread.h>

//ESTRUCTURA ARCHIVO CONFIGURACION
typedef struct console_configuracion {
	char* IP_KERNEL;
	char* PUERTO_KERNEL;
} console_configuracion;

signed int kernel;
console_configuracion configuracion;

enum instrucciones{
	iniciarPrograma = 1,
	finalizarPrograma = 2,
	desconectarConsola = 3,
	limpiarConsola = 4
};

typedef struct t_programa{
	int processID;
	int threadID;
	void* siguiente;
}t_programa;

typedef struct programas{
	t_programa* programa;
}programas;

programas programasActuales;

void hiloPrograma(char* path);

console_configuracion get_configuracion() {
	puts("Inicializando proceso Console\n");
	console_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Console001/config-console.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.IP_KERNEL = get_campo_config_string(archivo_configuracion, "IP_KERNEL");
	configuracion.PUERTO_KERNEL = get_campo_config_string(archivo_configuracion, "PUERTO_KERNEL");
	return configuracion;
}

#endif /* CONSOLE001_H_ */
