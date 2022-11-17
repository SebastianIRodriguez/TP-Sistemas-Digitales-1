/*
 * SysTick.c
 *
 *  Created on: 16 nov. 2022
 *      Author: lraff
 */

#include "SysTick.h"

#ifndef _MKL43Z4_H_
#include "MKL43Z4.h"			// Contiene los registros y máscaras
#endif

void SysTick_set_time(unsigned long long milisegundos)
{
	// Se indica el tiempo en milisegundos de cada cuanto se
	// quiere largar la interrupción

	unsigned long long cuentas = milisegundos * 3000;

	if(cuentas > MAX_LOAD)
		cuentas = MAX_LOAD;

	SysTick->LOAD = cuentas;
}


void SysTick_begin()
{
	SysTick->VAL = 0; //Limpio el contador
	SysTick->CTRL = 1; // Habilita el contador
	SysTick->CTRL |= 0B10; // Habilita la interrupción
}

void SysTick_end()
{
	SysTick->CTRL = 0;
}
