/*
 * ADC0.h
 *
 *  Created on: 16 nov. 2022
 *      Author: lraff
 */

#ifndef ADC0_H_
#define ADC0_H_

#define VALT 3.07
#define PTE20_ADC_CHANNEL 0
#define INTERNAL_TEMP_SENSOR_CHANNEL 0x1A


//int LM35_PORT;
//int LM35_PIN;
//int LM35_CHANNEL;

void ADC0_activate_port(unsigned int port);
void ADC0_begin();
int ADC0_get(int channel);


#endif /* ADC0_H_ */
