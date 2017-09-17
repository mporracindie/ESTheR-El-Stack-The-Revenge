/*
 * FileSystem.h
 *
 *  Created on: 8/6/2017
 *      Author: utnso
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <socketConfig.h>
#include <commons/config.h>

manejoFileSystem paqueteKernel;

typedef struct fileSystem_configuracion {
	int PUERTO;
	char* PUNTO_MONTAJE;
} fileSystem_configuracion;

	fileSystem_configuracion get_configuracion() {
	puts("Inicializando proceso File System\n");
	fileSystem_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/FileSystem/config-fileSystem.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.PUERTO = get_campo_config_int(archivo_configuracion, "PUERTO");
	configuracion.PUNTO_MONTAJE = get_campo_config_string(archivo_configuracion, "PUNTO_MONTAJE");
	return configuracion;
}

#endif /* FILESYSTEM_H_ */
