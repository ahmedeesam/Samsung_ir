/*
 * ext_int.h
 *
 * Created: 14/12/2021 05:01:06 م
 *  Author: ahmed essam
 */ 

#include <avr/io.h>

#ifndef EXT_INT_H_
#define EXT_INT_H_


void int0_START();
void int0_STOP();
void int1_init();
void int1_STOP();

#endif /* EXT_INT_H_ */