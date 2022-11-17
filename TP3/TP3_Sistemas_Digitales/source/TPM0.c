#include "TPM0.h"

/*
 *  La función set prepara al equipo para generar un PWM de manera tal qe
 *  al llegar al módulo pone 0 en el led para que se encienda y al llegar a
 *  match pone un 1 para apagarlo, es decir, contra mayor sea el valor de match,
 *  mayor luminosidad
 *
 * */

void TPMO_set(){

	// Habilito interrupciones de modulo TPM0
	NVIC_SetPriority(TPM0_IRQn, 0);
	NVIC_EnableIRQ(TPM0_IRQn);

	// Habilito la referencia al clock interno, p 441
	MCG->C1 |= 0x02;

	// Selecciono fuente de clock, p 442
	MCG->C2 = 0x00; // 2 MHz

	// Factor de división, página 443
	MCG->SC = 0x00; // Divide x1, o sea no divide

	// Selecciono el clocl para el módulo TPM, p 152
	SIM->SOPT2|=(0x03 << 24); // MCG source clock

	//  System Clock Gating Control Register 6, p 164
	SIM->SCGC6 |= 1<<24; // Da clock al módulo TPM0

	// System Clock Gating Control Register 5, p 162
	SIM->SCGC5 |= 1<<12; // Da clocl al puerto D donde está el led verde

	// Pin control register, p 139
	PORTD->PCR[PIN_LED_VERDE] |= PORT_PCR_MUX(4); // Conecta el pin del led verde al canal del TPM

	// La p 472 tiene el mapa de memoria de todo lo referido a TPMx

	// Funcionamiento del canal, página 472
	TPM0->CONTROLS[CANAL_LED_VERDE].CnSC|=0x64;

	// Bit 0x40 habilita interrupciones
	// Para PWM clear output en match: 0x28 | 101000
	// Para PWM ser output en match: 0x24 | 100100
	// Para Output compare: 0x14
	// Para PWM clear output en match, con interrupcion habilitada: 0x64


	// Módulo
	TPM0->MOD=1000;

	// Match del canal del led verde
	TPM0->CONTROLS[CANAL_LED_VERDE].CnV = 0;
}

void TPM0_begin()
{
	// Habilita el comienzo de la cuenta
	TPM0->SC |= (1<<3);
}

void TPM0_end()
{
	TPM0->SC &= ~(1<<3);
}