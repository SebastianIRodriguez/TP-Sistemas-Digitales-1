#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL43Z4.h"
#include "fsl_debug_console.h"
#include "Led_and_switch_control.h"
#include "TPM0.h"
#include "ADC0.h"

void delay(unsigned int count)
{
	for(unsigned int i = 0; i<count; i++)
	{
		i= (i + i)/2 + i - i;
	}
}

int main(void) {
	//ejercicio19();
	//ejercicio25();
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
	#endif

    PRINTF("TP3 - Enunciado 3, Raffagnini - Rodriguez\n\n");

    TPMO_Set();

    // Le da clocl al puerto E para usar el pin 20 para el sensor
    ADC0_activate_port(SIM_SCGC5_PORTE_MASK);
    ADC0_begin();

	while(1) {

		for(int i = 0; 1 ;i++)
		{
		// Match del canal del led verde
		if(i == 1000)
			i = 0;

		int lectura = ADC0_get(PTE20_ADC_CHANNEL);
		int lectura_escalada = lectura/4.1;

		TPM0->CONTROLS[CANAL_LED_VERDE].CnV = lectura_escalada;
		//delay(100000);

		}


	}

	return 0;
}

