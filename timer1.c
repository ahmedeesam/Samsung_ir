/*
 * timer1.c
 *
 * Created: 16/12/2021 10:30:04 م
 *  Author: ahmed essam
 */ 

#include "timer1.h"

void start_tim1_ICR()
{
	TCNT1   = 0x00;
	TCCR1B |= ((1<<CS12)|(ICES1));  //8prescaler ,capture Rising edge
	TIFR1   |= (1<<ICF1);   //clearing ic flag
}

void start_tim1_ICF()
{
	TCNT1   = 0x00;
	TCCR1B |= (1<<CS12);  //8prescaler ,capture Falling edge
	TIFR1  |= (1<<ICF1);    //clearing ic flag
}

int read_IC_register()
{
	TCCR1B = 0;
	return ICR1;
}

void start_tim1_normal()
{
	TCNT1 = 0x00;
	//TCCR1B |= (1<<CS12);  //256
	//TCCR1B |= ((1<<CS11)|(1<<CS10));  //64
	TCCR1B |= (1<<CS11);  //8
	//TCCR1B |= ((1<<CS12)|(1<<CS10)); //1024
}

void start_tim1_ctc(int q)
{
	TCNT1 = 0x00;
	OCR1A = q;
	//TCCR1B |= ((1<<CS12)|(1<<WGM12));
	TCCR1B |= ((1<<CS11)|(1<<WGM12));
}

void see_compare()
{
	while(!(TIFR1 & (1<<OCF1A))); //wait until match
	TIFR1 |= (1<<OCF1A); //clear flag
	TCCR1B = 0; //stop timer
}

int read_tim1()
{
	int t;
	t = TCNT1;
	TCCR1B = 0;
	return t;
}

void tim1_OVinterrupt_init()
{
	TIMSK1 |= (1<<TOIE1);
}

void tim1_ICinterrupt_START()
{
	TIMSK1 |= (1<<ICIE1);
}

void tim1_ICinterrupt_STOP()
{
	TIMSK1 &= ~(1<<ICIE1);
}