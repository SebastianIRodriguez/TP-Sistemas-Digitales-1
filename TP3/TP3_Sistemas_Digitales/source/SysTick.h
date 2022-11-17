/*
 * SysTick.h
 *
 *  Created on: 16 nov. 2022
 *      Author: lraff
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#define CLK_FREQ 48000000UL
#define ALT_CLK_FREQ 3000000UL
#define MAX_LOAD 16777216UL

void SysTick_set_time(unsigned long long milisegundos);
void SysTick_begin();
void SysTick_end();

#endif /* SYSTICK_H_ */
