#ifndef ADC0_H_
#define ADC0_H_

#define VALT 3.07
#define PTE20_ADC_CHANNEL 0
#define INTERNAL_TEMP_SENSOR_CHANNEL 0x1A
#define LIGHT_SENSOR_CHANNEL 0x03

void ADC0_activate_port(unsigned int port);
void ADC0_begin();
int ADC0_get(int channel);

float codificar_en_grados(int lectura, int resolucion, float Vref);


#endif /* ADC0_H_ */
