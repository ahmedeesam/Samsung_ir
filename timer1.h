/*
 * timer1.h
 *
 * Created: 16/12/2021 10:29:49 م
 *  Author: ahmed essam
 */ 


#define  F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


#ifndef TIMER1_H_
#define TIMER1_H_

void start_tim1_normal();
int  read_tim1();
void start_tim1_ICR();
void start_tim1_ICF();
int  read_IC_register();
void start_tim1_ctc(int q);
void see_compare();
void tim1_OVinterrupt_init();
void tim1_ICinterrupt_START();
void tim1_ICinterrupt_STOP();


#endif /* TIMER1_H_ */