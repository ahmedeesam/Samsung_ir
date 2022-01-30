/*
 * timer0.h
 *
 * Created: 14/12/2021 05:49:00 م
 *  Author: ahmed essam
 */ 

#define  F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


#ifndef TIMER0_H_
#define TIMER0_H_

void start_tim0_38khz();
void stop_tim0_38khz();


#endif /* TIMER0_H_ */