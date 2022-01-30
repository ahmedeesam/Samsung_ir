/*
 * timer0.c
 *
 * Created: 14/12/2021 05:48:44 م
 *  Author: ahmed essam
 */ 

#include "timer0.h"

void start_tim0_38khz()
{
	TCNT0   = 0x00;
	OCR0A   = 0xD;
	TCCR0A |= ((1<<WGM01)|(1<<COM0A0));
	//TIMSK0 |= (1<<OCIE0A);
	TCCR0B |= (1<<CS01);
}

void stop_tim0_38khz()
{
	TCCR0B = 0;
}


/*		PORTC |= (1<<PORTC0);
_delay_ms(150);
PORTC &= ~(1<<PORTC0);*/