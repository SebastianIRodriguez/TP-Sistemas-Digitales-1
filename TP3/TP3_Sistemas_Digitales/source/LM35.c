#include "LM35.h"

int LM35_CHANNEL_RESOLUTION = 4096;
float LM35_VREF = 3.07;

void LM35_attach_channel_resolution(int resolucion)
{
	LM35_CHANNEL_RESOLUTION = resolucion;
}

void LM35_attach_vref(float Vref)
{
	LM35_VREF = Vref;
}

float LM35_codificar_grados(int lectura)
{
	float lectura_en_grados = ((lectura * LM35_VREF)/LM35_CHANNEL_RESOLUTION) * LM35_CONSTANT;
	return lectura_en_grados;
}

float LM35_codificar_en_grados(int lectura, int resolucion, float Vmax)
{
	float lectura_en_grados = ((lectura * Vmax)/resolucion) * LM35_CONSTANT;
	return lectura_en_grados;
}

