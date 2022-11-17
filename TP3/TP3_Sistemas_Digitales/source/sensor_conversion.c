#include "sensor_conversion.h"

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