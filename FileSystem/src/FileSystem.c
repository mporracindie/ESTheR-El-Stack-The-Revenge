#include "FileSystem.h"

int main(void) {
	fileSystem_configuracion configuracion = get_configuracion();

	un_socket kernel = socket_escucha("127.0.0.1", configuracion.PUERTO);
	aceptar_conexion(kernel);
	t_paquete* handshake = recibir(kernel);
	esperar_handshake(kernel, handshake,cop_handshake_fileSystem);
	free(handshake);
	while(1) {
		t_paquete* paqueteRecibido = recibir(kernel);
		char* archivoValidado = "Existe el archivo";
		char* archivoInvalido = "No existe el archivo";
		switch(paqueteRecibido->codigo_operacion) {
			case cop_filesystem_validarArchivo:
			//paqueteKernel = (manejoFileSystem) paqueteRecibido->data;
			//if(comprobar_archivo(paqueteKernel->path)) {
			//	enviar(kernel, cop_filesystem_validarArchivo, sizeof(archivoValidado), archivoValidado);
			//}
			//else {
			//	enviar(kernel, cop_filesystem_validarArchivo, sizeof(archivoInvalido), archivoInvalido);
			//}
			break;

			case cop_filesystem_crearArchivo:


			break;

			case cop_filesystem_borrar:
			break;

			case cop_filesystem_obtenerDatos:
			break;

			case cop_filesystem_guardarDatos:
			break;



		}

	}
		return 0;
}
