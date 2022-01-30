/*
 * ext_int.c
 *
 * Created: 14/12/2021 05:01:39 م
 *  Author: ahmed essam
 */ 

#include "ext_int.h"

void int0_START()
{
	//DDRD &= ~(1<<PORTD2);
	EICRA |= (1<<ISC01);
	EIMSK |= (1<<INT0);
}

void int0_STOP()
{
	//DDRD &= ~(1<<PORTD2);
	EICRA &= ~(1<<ISC01);
	EIMSK &= ~(1<<INT0);
}

void int1_init()
{
	//DDRD &= ~(1<<PORTD3);
	EICRA |= ((1<<ISC11)|(1<<ISC10));
	EIMSK |= (1<<INT1);
}

void int1_STOP()
{
	EICRA &= ~((1<<ISC11)|(1<<ISC10));
	EIMSK &= ~(1<<INT1);
}
