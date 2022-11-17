#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL43Z4.h"
#include "fsl_debug_console.h"
#include "sensor_conversion.h"
#include "Led_and_switch_control.h"
#include "TPM0.h"
#include "ADC0.h"
#include "LM35.h"
#include "SysTick.h"
#include "Petri.h"

#define LM35_CHANNEL PTE20_ADC_CHANNEL // 0

#define PTE21_TOGGLE (PTE->PTOR |= (1 << 21))
#define PTE21_ON (PTE->PSOR |= (1 << 21))
#define PTE21_OFF (PTE->PCOR |= (1 << 21))

// *********************************  Variables locales ******************************************
extern int Ct;
extern int L;

int last_light_sensor_value = 0;
int Ft = 0;

void config_board() {
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
	#endif
}

void config_led_red() {
	// Declaro como GPIO al pin 21, puerto E

	PORTE->PCR[PIN_LED_ROJO] = PORT_PCR_MUX(1);

	// Le doy la funcionalidad de salida
	GPIOE->PDDR |= 1 << PIN_LED_ROJO;
}

void config_test_pin() {
	// Declaro como GPIO al pin 21, puerto E
	PORTE->PCR[21] = PORT_PCR_MUX(1);

	// Le doy la funcionalidad de salida
	GPIOE->PDDR |= 1 << 21;
}

void config_ADC() {
	// Le da clock al puerto E para usar el pin 20 para el sensor
	ADC0_activate_port(SIM_SCGC5_PORTE_MASK);

	// Se debe llamar a esta función siempre que se quiera usar ADC0
	ADC0_begin();

	// Se indica la resolución del canal del LM35 para poder realizar los cálculos
	LM35_attach_channel_resolution(MAX_ADC_VALUE);

	// Se indica la tensión de referencia del ADC0 para poder realizar los cálculos
	LM35_attach_vref(VALT);
}

void run_main_loop() {
	while (1)
	{
		int Sw1 = Sw1_get();
		int Sw3 = Sw3_get();

		petri(Sw1, Sw3, Ft);

		if (Ct)
		{
			SysTick_begin(); // No importa cuantas veces se llame
		}
		else
		{
			SysTick_end();
			Ft = 0;
		}

		if (L)
		{
			TPM0_begin(); // Habilita la cuenta

			int medicion_temperatura = ADC0_get(LM35_CHANNEL); // Guarda el valor tomado del ADCO en el canal del LM35
			float medicion_en_grados = LM35_codificar_grados(medicion_temperatura);

			int PWM_Match = map_temperature(medicion_en_grados, COTA_INF, COTA_SUP, 1000);

			// PRINTF("Lectura en grados de LM35: %.2f°C  encode: %d\n", lectura_en_grados, PWM_Match);

			TPM0->CONTROLS[CANAL_LED_VERDE].CnV = PWM_Match;

			last_light_sensor_value = ADC0_get(LIGHT_SENSOR_CHANNEL);
		}
		else
		{
			TPM0_end();
			LED_ROJO_OFF;
		}
	}
}

void SysTick_Handler()
{
	SysTick->CTRL; // limpio la bandera de interrupcion (status and control)

	Ft = 1;
}

/*
	Va llevando un conteo de la cantidad de veces que se finalizo la cuenta desde el ultimo reseteo.
	Mientras no haya alcanzado la mitad del largo del ciclo (calculado en base al nivel de luz) el led esta encendido.
	En el otro semiciclo esta apagado.
*/
void update_led_red_state()
{
	static int cycle_counter = 0;
	int cycle_length = map_light_to_time_period(last_light_sensor_value);

	// PRINTF("Cycle Length: %d\n", cycle_length);

	if (cycle_counter < cycle_length / 2)
	{
		LED_ROJO_OFF;
	}
	else if (cycle_counter < cycle_length)
	{
		LED_ROJO_ON;
	}
	else
	{
		cycle_counter = 0;
	}
	cycle_counter++;
}

void TPM0_IRQHandler()
{
	update_led_red_state();

	TPM0->CONTROLS[CANAL_LED_VERDE].CnSC |= 0x80;
}

int main(void)
{
	config_board();

	PRINTF("TP3 - Enunciado 3, Raffagnini - Rodriguez\n\n");

	SysTick_set_time(2000);

	TPMO_set();

	config_ADC();

	config_test_pin();

	config_led_red();

	Sw1_init();
	Sw3_init();

	run_main_loop();

	return 0;
}
