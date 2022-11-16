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
#include "LM35.h"

#define COTA_INF 35
#define COTA_SUP 60

void delay(unsigned int count)
{
	for(unsigned int i = 0; i<count; i++)
	{
		i= (i + i)/2 + i - i;
	}
}

float Codificar_en_grados(int lectura, int resolucion, float Vref)
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

int Encode_temperature(float temperatura, float cota_inf, float cota_sup, int resolucion)
{
	/*
	 * Mapea la temperatura indicada en un rango a fondo de escala
	 * */
	float k = resolucion/(cota_sup - cota_inf);
	int encode = k * (temperatura - cota_inf);

	if(encode >= 0)
	{
		if(encode > resolucion)
			return resolucion;
		return encode;
	}

	return 0;
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

    // Se debe llamar a esta función siempre que se quiera usar ADC0
    ADC0_begin();

    // Se indica la resolución del canal del LM35 para poder realizar los cálculos
    LM35_attach_channel_resolution(4095);

    // Se indica la tensión de referencia del ADC0 para poder realizar los cálculos
    LM35_attach_vref(VALT);

	while(1) {

		int lectura = ADC0_get(PTE20_ADC_CHANNEL);
		float lectura_en_grados = LM35_codificar_grados(lectura);

		int PWM_Match = Encode_temperature(lectura_en_grados, COTA_INF, COTA_SUP, 4095);

		//PRINTF("Lectura en grados de LM35: %.2f°C  encode: %d\n", lectura_en_grados, PWM_Match);

		TPM0->CONTROLS[CANAL_LED_VERDE].CnV = PWM_Match;

	}

	return 0;
}

//int lectura_escalada = lectura/4.1;
//delay(100000);
//int lectura2 = ADC0_get(INTERNAL_TEMP_SENSOR_CHANNEL);
//float lectura_en_grados2 = Codificar_en_grados(lectura2, 4095, VALT);
