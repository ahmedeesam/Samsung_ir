/*
 * pin_config.h
 *
 * Created: 25/11/2021 11:07:04 م
 *  Author: ahmed essam
 */ 


#ifndef PIN_CONFIG_H_
#define PIN_CONFIG_H_

#define F_CPU 8000000UL // 8 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>

#define DATA_BUS	PORTB
#define CTL_BUS		PORTD
#define DATA_DDR	DDRB
#define CTL_DDR		DDRD
#define LCD_D1			1
#define LCD_D2			2
#define LCD_D3			3
#define LCD_D4			4
#define LCD_EN			5
#define	LCD_RW			6
#define	LCD_RS			7



#endif /* PIN_CONFIG_H_ */