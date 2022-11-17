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
#include "SysTick.h"
#include "Petri.h"

#define MAX_ADC_VALUE 4095.0 // Con esta resolución, obtenemos un mínimo de 750 uV, es decir, 75 m°C
// #define MAX_ADC_VALUE 65536.0 // Con esta resolución, obtenemos un mínimo de 47 uV, aprox 0.05mV, es decir, 4.7 m°C

// Cantidad de overflows que va a durar el periodo de trabajo del led cuando se ilumina con MINIMA intensidad
#define MAX_PERIOD_LENGTH 4000

// Cantidad de overflows que va a durar el periodo de trabajo del led cuando se ilumina con MAXIMA intensidad
#define MIN_PERIOD_LENGTH 4

#define COTA_INF 26
#define COTA_SUP 35
#define LM35_CHANNEL PTE20_ADC_CHANNEL // 0

#define PTE21_TOGGLE (PTE->PTOR |= (1 << 21))
#define PTE21_ON (PTE->PSOR |= (1 << 21))
#define PTE21_OFF (PTE->PCOR |= (1 << 21))

// *********************************  Variables locales ******************************************
int medicion_temperatura; // Guarda el valor tomado del ADCO en el canal del LM35
float medicion_en_grados;
int PWM_Match;
int last_light_sensor_value = 0;

extern int Ct;
extern int L;

int Ft = 0;

int map_light_to_time_period(int light_value);
int map_temperature(float temperatura, float cota_inf, float cota_sup, int resolucion);

int main(void)
{

	// ********************************  Consola ********************************************
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	PRINTF("TP3 - Enunciado 3, Raffagnini - Rodriguez\n\n");

	// *******************************  SysTick ********************************************
	SysTick_set_time(2000);

	// ****************************  Configuración del TPM0  *******************************
	// Habilito interrupciones de modulo TPM0
	NVIC_SetPriority(TPM0_IRQn, 0);
	NVIC_EnableIRQ(TPM0_IRQn);

	// Hace todo el maneje para encender el módulo y configurarlo
	TPMO_set();

	// Habilita la cuenta
	TPM0_begin();

	// ****************************  Configuración del ADC0  *******************************
	// Le da clock al puerto E para usar el pin 20 para el sensor
	ADC0_activate_port(SIM_SCGC5_PORTE_MASK);

	// Se debe llamar a esta función siempre que se quiera usar ADC0
	ADC0_begin();

	// Se indica la resolución del canal del LM35 para poder realizar los cálculos
	LM35_attach_channel_resolution(MAX_ADC_VALUE);

	// Se indica la tensión de referencia del ADC0 para poder realizar los cálculos
	LM35_attach_vref(VALT);

	// ****************************  Pin de prueba *****************************************
	// Declaro como GPIO al pin 21, puerto E
	PORTE->PCR[21] = PORT_PCR_MUX(1);

	// Le doy la funcionalidad de salida
	GPIOE->PDDR |= 1 << 21;

	// *****************************  Pin LED ROJO *****************************************
	// Declaro como GPIO al pin 21, puerto E

	PORTE->PCR[PIN_LED_ROJO] = PORT_PCR_MUX(1);

	// Le doy la funcionalidad de salida
	GPIOE->PDDR |= 1 << PIN_LED_ROJO;

	PTE21_ON;

	for (unsigned int i = 0; i < 500000; i++)
	{
		i = i - i + 2 * i - i;
	}

	PTE21_OFF;
	

	// *************************** Algoritmo principal *************************************
	while (1)
	{
		if(Ft)
			PTE21_ON;
		else
			PTE21_OFF;

		if (Ct)
		{
			// No importa cuantas veces se llame
			SysTick_begin();
		}
		else
		{
			SysTick_end();
			Ft = 0;
		}

		if (L)
		{
			medicion_temperatura = ADC0_get(LM35_CHANNEL);
			medicion_en_grados = LM35_codificar_grados(medicion_temperatura);

			PWM_Match = map_temperature(medicion_en_grados, COTA_INF, COTA_SUP, 1000);

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

	return 0;
}

void SysTick_Handler()
{
	SysTick->CTRL; // limpio la bandera de interrupcion (status and control)
	 
	Ft = 1;
}

int map_light_to_time_period(int light_value)
{
	return light_value / MAX_ADC_VALUE * (MAX_PERIOD_LENGTH - MIN_PERIOD_LENGTH) + MIN_PERIOD_LENGTH;
}

int map_temperature(float temperatura, float cota_inf, float cota_sup, int modulo)
{
	/*
	 * Mapea la temperatura indicada en un rango a fondo de escala
	 * */
	float k = modulo / (cota_sup - cota_inf);
	int encode = k * (temperatura - cota_inf);

	if (encode >= 0)
	{
		if (encode > modulo)
			return modulo;
		return encode;
	}
	return 0;
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
