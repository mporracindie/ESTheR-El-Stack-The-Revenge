/*
 * primitivas.c
 *
 *  Created on: 30/5/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_C_
#define PRIMITIVAS_C_

#include "primitivas.h"

//static const int CONTENIDO_VARIABLE = 20;
static int POSICION_MEMORIA = 0x10;
bool termino = false;
char* identificador_variable;

t_puntero definirVariable(t_nombre_variable identificador_variable) {
	printf("definir la variable %c\n", identificador_variable);
	malloc(sizeof(identificador_variable));
	//RESPONSABILIDAD MEMORIA:
	//registrarEnStack(identificador_variable);
	//registrarEnDiccionarioDeVariables(identificador_variable);
	//POSICION_MEMORIA = posicionEnStack(identificador_variable); ¿Es lo mismo que la dirección a memoria? ¿&identificador_variable?

	//guardar contexto de ejecucions
	//solicitar espacio para la variable a memoria
	//guardar el valor de la variable en el stack
	//devolver el puntero a memoria de dicha variable
	return POSICION_MEMORIA;
}

t_puntero obtenerPosicionVariable(t_nombre_variable variable) {
	printf("Obtener posicion de %c\n", variable);
	//la busqueda en el segmento se delega o se hace aca? en caso de que se haga aca:
	//solicitar segmento (?)
	//algoritmo de busqueda
	return POSICION_MEMORIA;
}

t_valor_variable dereferenciar(t_puntero puntero) {
	t_valor_variable CONTENIDO_VARIABLE = (int) &puntero;
	printf("Dereferenciar %d y su valor es: %d\n", puntero, CONTENIDO_VARIABLE);
	return CONTENIDO_VARIABLE;
}

void asignar(t_puntero puntero, t_valor_variable variable) {
	printf("Asignando en %d el valor %d\n", puntero, variable);
	//&puntero = variable;
}

void finalizar(void){
	termino = true;
	printf("Finalizar\n");
}

bool terminoElPrograma(void){
	return termino;
}

#endif /* PRIMITIVAS_C_ */
