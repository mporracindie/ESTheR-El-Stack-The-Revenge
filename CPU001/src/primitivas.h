/*
 * primitivas.h
 *
 *  Created on: 30/5/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_H_
#define PRIMITIVAS_H_

#include <stdio.h>
#include <stdbool.h>
#include <socketConfig.h>
#include <parser/parser.h>

/*
 * DEFINIR VARIABLE
 *
 * Reserva en el Contexto de Ejecución Actual el espacio necesario para una variable llamada identificador_variable y la registra tanto en el Stack como en el Diccionario de Variables. Retornando la posición del valor de esta nueva variable del stack
 * El valor de la variable queda indefinido: no deberá inicializarlo con ningún valor default.
 * Esta función se invoca una vez por variable, a pesar que este varias veces en una línea.
 * Ej: Evaluar "variables a, b, c" llamará tres veces a esta función con los parámetros "a", "b" y "c"
 *
 * @sintax	TEXT_VARIABLE (variables)
 * 			-- nota: Al menos un identificador; separado por comas
 * @param	identificador_variable	Nombre de variable a definir
 * @return	Puntero a la variable recien asignada
 */
t_puntero definirVariable(t_nombre_variable variable);

/*
 * OBTENER POSICION de una VARIABLE
 *
 * Devuelve el desplazamiento respecto al inicio del segmento Stacken que se encuentra el valor de la variable identificador_variable del contexto actual.
 * En caso de error, retorna -1.
 *
 * @sintax	TEXT_REFERENCE_OP (&)
 * @param	identificador_variable 		Nombre de la variable a buscar (De ser un parametro, se invocara sin el '$')
 * @return	Donde se encuentre la variable buscada
 */
t_puntero obtenerPosicionVariable(t_nombre_variable variable);

/*
 * DEREFERENCIAR
 *
 * Obtiene el valor resultante de leer a partir de direccion_variable, sin importar cual fuera el contexto actual
 *
 * @sintax	TEXT_DEREFERENCE_OP (*)
 * @param	direccion_variable	Lugar donde buscar
 * @return	Valor que se encuentra en esa posicion
 */
t_valor_variable dereferenciar(t_puntero puntero);

/*
 * ASIGNAR
 *
 * Inserta una copia del valor en la variable ubicada en direccion_variable.
 *
 * @sintax	TEXT_ASSIGNATION (=)
 * @param	direccion_variable	lugar donde insertar el valor
 * @param	valor	Valor a insertar
 * @return	void
 */
void asignar(t_puntero puntero, t_valor_variable variable);

/*
 * FINALIZAR
 *
 * Cambia el Contexto de Ejecución Actual para volver al Contexto anterior al que se está ejecutando, recuperando el Cursor de Contexto Actual y el Program Counter previamente apilados en el Stack.
 * En caso de estar finalizando el Contexto principal (el ubicado al inicio del Stack), deberá finalizar la ejecución del programa.
 *
 * @sintax	TEXT_END (end)
 * @param	void
 * @return	void
 */
void finalizar(void);

bool terminoElPrograma(void);

#endif /* PRIMITIVAS_H_ */
