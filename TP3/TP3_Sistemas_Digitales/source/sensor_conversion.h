#ifndef SENSOR_CONVERSION_H_
#define SENSOR_CONVERSION_H_

#define MAX_ADC_VALUE 4095.0 // Con esta resolución, obtenemos un mínimo de 750 uV, es decir, 75 m°C
// #define MAX_ADC_VALUE 65536.0 // Con esta resolución, obtenemos un mínimo de 47 uV, aprox 0.05mV, es decir, 4.7 m°C

// Cantidad de overflows que va a durar el periodo de trabajo del led cuando se ilumina con MINIMA intensidad
#define MAX_PERIOD_LENGTH 4000

// Cantidad de overflows que va a durar el periodo de trabajo del led cuando se ilumina con MAXIMA intensidad
#define MIN_PERIOD_LENGTH 4

#define COTA_INF 25
#define COTA_SUP 35

int map_light_to_time_period(int light_value);

int map_temperature(float temperatura, float cota_inf, float cota_sup, int modulo);

#endif /* SENSOR_CONVERSION_H_ */
