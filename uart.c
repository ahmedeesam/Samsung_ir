/*
 * uart.c
 *
 * Created: 20/11/2021 08:08:30 م
 *  Author: ahmed essam
 */ 


#include "uart.h"

void uart_init()
{
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	UBRR0H = (unsigned char)(51>>8);
	UBRR0L = (unsigned char)51;
}

void uart_send_ch(char ch)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = ch;
}

char uart_recieve_ch()
{
	char f ;
	while(!(UCSR0A & (1<<RXC0)));
	f = UDR0;
	return f;
}

void serial_debug(char *a)
{
	while (*a != '\0')
	{
		uart_send_ch(*a);
		a++;
	}
}

 void uart_recieve_string(char *b)
 {
	 char i;
	 i = uart_recieve_ch();
	 for (unsigned char z = 0;z <= (i-1);z++)
	 {
		 b[z] = uart_recieve_ch();
	 }
	 b[i] = '\0';
 }
 
 void uart_send_string(char *a ,char c)
 {
	 uart_send_ch(c);
	 _delay_ms(50);
	 for(char z=0;z <= (c-1);z++)
	 {
		 uart_send_ch(a[z]);
	 }
 }
 
/*void uart_send_string(char *a)
{
	while (*a != '\0')
	{
		uart_send_ch(*a);
		a++;
	}
}*/
/* void uart_recieve_stringf(char *b,char buff)
{
	char i;
	i = uart_recieve_ch();
	for (unsigned char z = 0;z <= (i-1);z++)
	{
		b[z] = uart_recieve_ch();
	}
		for (unsigned char x = i;x < buff;x++)
		{
			b[x] = 0;
		}
	b[buff] = '\0';
}
*/



/* faild tries
void uart_recieve_string(char *b)
{
	for (unsigned char z = 0;z <= 2;z++)
	{
		b[z] = "0";
	}
		while (*b != '\0')     //receiving until receive null character
		{
			*b = uart_recieve_ch(); 
			b++;
		}
}


void uart_recieve_strings(char *x)
{
	char i = 0;
	while (x[i] != 'Q')     //receiving until receive null character
	{
		x[i] = uart_recieve_ch();
		i++;
	}
	x[2] = '\0';
}
*/