#include <ADC0.h>

/*
 *  En lpágina 342 se encuentra una tabla con la referencia a los canales
 *  Esta biblioteca usa el ADC0
 *
 * */
#ifndef _MKL43Z4_H_
#include "MKL43Z4.h"			// Contiene los registros y máscaras
#endif

void ADC0_begin()
{
	//  System Clock Gating Control Register 6, p 164
	SIM->SCGC6 |= 1<<27; // Da clock al conversor A-D 0

	// **************  ADC Configuration Register 1, p 352 *************************************
	// Clock Divide Select
	ADC0->CFG1 = ADC_CFG1_ADIV(0B10); // input clock/4 (24MHz / 4) = 6MHz

	// Sample Time Configuration
	ADC0->CFG1 |= 1<<4; // Long sample time

	//Conversion mode selection
	ADC0->CFG1 |= ADC_CFG1_MODE(0B01); // Single-ended 12-bit conversion

	// Input Clock Select
	ADC0->CFG1 |= ADC_CFG1_ADICLK(0B01); // Bus clock divided by 2(BUSCLK/2)

	// **************  ADC Status and Control Register 3, p 357 *******************************
	// Voltage Reference Selection
	ADC0->SC2 |= 1; //Queda ver si es Valt, aprox 3.07V

	// **************  ADC Status and Control Register 3, p 359 *******************************

	// Hardware Average Enable
	ADC0->SC3 |= 1<<2; // Hardware average function enabled.

	//Hardware Average Select
	ADC0->SC3 |= 0B01; // 8 samples averaged

}

int ADC0_get(int channel)
{
	ADC0->SC1[0]=channel;
	while((ADC0->SC1[0] & 0x80) == 0){}
	int result = ADC0->R[0];
	return result;
}

void ADC0_activate_port(unsigned int port){
	SIM->SCGC5 |= port;
}

float codificar_en_grados(int lectura, int resolucion, float Vref)
{
	float tension = (lectura * Vref)/ resolucion ;
	float m;
	float tension_ref_25c = 0.703125;

	//Dependiendo si la temperatura medida es mayor o menor a 25°C cambia el calculo
	if(tension > tension_ref_25c) {
		m = 0.001758;
	}
	else {
		m = 0.001668;
	}

	float lectura_en_grados = 25.0 - (tension - tension_ref_25c) / m;
	return lectura_en_grados;
}
