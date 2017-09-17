#include "CPU001.h"

int main(void) {
	cpu_configuracion configuracion = get_configuracion();

	un_socket kernel = conectar_a(configuracion.IP_KERNEL, configuracion.PUERTO_KERNEL);
	realizar_handshake(kernel, cop_handshake_cpu);

	un_socket memoria = conectar_a(configuracion.IP_MEMORIA, configuracion.PUERTO_MEMORIA);
	realizar_handshake(memoria, cop_handshake_memoria);
	return 0;
}
