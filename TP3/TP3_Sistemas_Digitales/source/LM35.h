#ifndef LM35_H_
#define LM35_H_

#define LM35_CONSTANT 100 // Son 0.01V/°C

void LM35_attach_channel_resolution(int resolucion);
void LM35_attach_vref(float Vref);
float LM35_codificar_grados(int lectura);
float LM35_codificar_en_grados(int lectura, int resolucion, float Vmax);


#endif /* LM35_H_ */

