#ifndef TPMO_H
#define TPMO_H

#ifndef _MKL43Z4_H_
#include "MKL43Z4.h"			// Contiene los registros y máscaras
#endif

#ifndef LED_AND_SWITCH_CONTROL
#include "Led_and_switch_control.h"			// Contiene los registros y máscaras
#endif


void TPMO_set();
void TPM0_begin();
void TPM0_end();

#endif
