/*
 * LM35.h
 *
 *  Created on: 16 nov. 2022
 *      Author: lraff
 */

#ifndef LM35_H_
#define LM35_H_

#define VALT 3.07
#define PTE20_ADC_CHANNEL 0


//int LM35_PORT;
//int LM35_PIN;
//int LM35_CHANNEL;

void ADC0_activate_port(unsigned int port);
void ADC0_begin();
int ADC0_get(int channel);


#endif /* LM35_H_ */
