#include "SysTick.h"

#ifndef _MKL43Z4_H_
#include "MKL43Z4.h"			// Contiene los registros y máscaras
#endif

int active_timer_flag = 0;

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
	if(!active_timer_flag) {
		SysTick->VAL = 0; //Limpio el contador
		SysTick->CTRL = 1; // Habilita el contador
		SysTick->CTRL |= 0B10; // Habilita la interrupción

		active_timer_flag = 1;
	}
}

void SysTick_end()
{
	active_timer_flag = 0;
	SysTick->CTRL = 0;
}
