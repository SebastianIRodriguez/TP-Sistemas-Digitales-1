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

int last_light_sensor_value = 0;

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

#define MAX_ADC_VALUE 4095.0

//Cantidad de overflows que va a durar el periodo de trabajo del led cuando se ilumina con MINIMA intensidad
#define MAX_PERIOD_LENGTH 80 

//Cantidad de overflows que va a durar el periodo de trabajo del led cuando se ilumina con MAXIMA intensidad
#define MIN_PERIOD_LENGTH 8

/*
	Determina cuanto debe durar el ciclo de trabajo del LED Rojo en base a la cantidad de luz medida
*/
int map_light_to_time_period(int light_value) {
	return light_value / MAX_ADC_VALUE * (MAX_PERIOD_LENGTH - MIN_PERIOD_LENGTH) + MIN_PERIOD_LENGTH;
}

int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
	#endif

    PRINTF("TP3 - Enunciado 3, Raffagnini - Rodriguez\n\n");

	//Habilito interrupciones de modulo TPM0
	NVIC_SetPriority(TPM0_IRQn,0);
	NVIC_EnableIRQ(TPM0_IRQn);

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

		last_light_sensor_value = ADC0_get(LIGHT_SENSOR_CHANNEL);

		//PRINTF("Lectura de sensor de luz: %d", last_light_sensor_value);
	}

	return 0;
}

/*
	Va llevando un conteo de la cantidad de veces que se finalizo la cuenta desde el ultimo reseteo.
	Mientras no haya alcanzado la mitad del largo del ciclo (calculado en base al nivel de luz) el led esta encendido.
	En el otro semiciclo esta apagado.
*/
void update_led_red_state() {
	static int cycle_counter = 0;
	int cycle_length = map_light_to_time_period(last_light_sensor_value);

	PRINTF("Cycle Length: %d\n", cycle_length);

	if(cycle_counter < cycle_length / 2) {
		LED_ROJO_ON;
	}
	else if(cycle_counter < cycle_length) {
		LED_ROJO_OFF;
	}
	else {
		cycle_counter = 0;
	}
	cycle_counter++;
}

void TPM0_IRQHandler() {
	update_led_red_state();

	TPM0->CONTROLS[CANAL_LED_VERDE].CnSC |= 0x80;
}

//int lectura_escalada = lectura/4.1;
//delay(100000);
//int lectura2 = ADC0_get(INTERNAL_TEMP_SENSOR_CHANNEL);
//float lectura_en_grados2 = Codificar_en_grados(lectura2, 4095, VALT);
