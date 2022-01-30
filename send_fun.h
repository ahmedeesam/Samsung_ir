/*
 * send_fun.h
 *
 * Created: 14/01/2022 09:28:33 م
 *  Author: ahmed essam
 */ 

#include <avr/io.h>
#include "timer1.h"
#include "uart.h"

#ifndef SEND_FUN_H_
#define SEND_FUN_H_

void send_start_bit();
void send_0_bit();
void send_1_bit();
void send_8bits(int a);
void send_end_bit();


#endif /* SEND_FUN_H_ */