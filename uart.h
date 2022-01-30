/*
 * uart.h
 *
 * Created: 20/11/2021 08:08:15 م
 *  Author: ahmed essam
 */ 
#define F_CPU 8000000UL // 8 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>

#ifndef UART_H_
#define UART_H_

int v;

void uart_init();
void uart_send_ch(char ch);
char uart_recieve_ch();
void serial_debug(char *a);
void uart_recieve_string(char *b);


#endif /* UART_H_ */