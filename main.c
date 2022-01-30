/*
 * external-INT-timer0.c
 *
 * Created: 14/12/2021 04:04:28 م
 * Author : ahmed essam
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "var.h"
#include "ext_int.h"
#include "timer0.h"
#include "timer1.h"
#include "send_fun.h"
#include "uart.h"




ISR (INT0_vect)
{
	if(puls_num == 0)
	{
		start_tim1_normal();
	}
	else if (puls_num != 0)
	{
		puls_time = read_tim1();
		start_tim1_normal();
		puls_num++;
		if (puls_num == 2)
		{
			if ((puls_time > 4400) && (puls_time < 4600) )
			{
				puls_comp.start2 = 1;
			}
		}
		if ((puls_comp.start1 == 1) && (puls_comp.start2 == 1) && (puls_num > 2))
		{
		if ((bit_num >= 0) && (bit_num <= 7))
		{
			if ((puls_time > 1100) && (puls_time < 1400) )  //1250 for LOW +- 50 timing error
			{
				puls_comp.address_ = (puls_comp.address_ << 1);
				serial_debug("0");
			}
			else if ((puls_time > 2200) && (puls_time < 2300) )  //2250 for HIGH +- 50 timing error
			{
				puls_comp.address_ = (puls_comp.address_ << 1)|(0x01);
				serial_debug("1");
			}
			bit_num++;
			
		}
		else if ((bit_num > 7) && (bit_num <= 15))
		{
			if ((puls_time > 1100) && (puls_time < 1400) )  //1250 for LOW +- 50 timing error
			{
				puls_comp.address_INV = (puls_comp.address_INV << 1);
				serial_debug("0");
			}
			else if ((puls_time > 2200) && (puls_time < 2300) )  //2250 for HIGH +- 50 timing error
			{
				puls_comp.address_INV = (puls_comp.address_INV << 1)|(0x01);
				serial_debug("1");
			}
			bit_num++;
			
		}
		else if ((bit_num > 15) && (bit_num <= 23))
		{
			if ((puls_time > 1100) && (puls_time < 1400) )  //1250 for LOW +- 50 timing error
			{
				puls_comp.command_ = (puls_comp.command_ << 1);
				serial_debug("0");
			}
			else if ((puls_time > 2200) && (puls_time < 2300) )  //2250 for HIGH +- 50 timing error
			{
				puls_comp.command_ = (puls_comp.command_ << 1)|(0x01);
				serial_debug("1");
			}
			bit_num++;
			
		}
		else if ((bit_num > 23) && (bit_num <= 31))
		{
			if ((puls_time > 1100) && (puls_time < 1400) )  //1250 for LOW +- 50 timing error
			{
				puls_comp.command_INV = (puls_comp.command_INV << 1);
				serial_debug("0");
			}
			else if ((puls_time > 2200) && (puls_time < 2300) )  //2250 for HIGH +- 50 timing error
			{
				puls_comp.command_INV = (puls_comp.command_INV << 1)|(0x01);
				serial_debug("1");
			}
			bit_num++;
			
		}
	}
}
}


ISR (INT1_vect)
{
	puls_time = read_tim1();
	puls_num++;
		if ((puls_time > 4400) && (puls_time < 4600 ) )
		{
			puls_comp.start1 = 1;
		}
	start_tim1_normal();
	int1_STOP();
}

int reverse(int b) {
	b = (((b & 0xF0) >> 4) | ((b & 0x0F) << 4));
	b = (((b & 0xCC) >> 2) | ((b & 0x33) << 2));
	b = (((b & 0xAA) >> 1) | ((b & 0x55) << 1));
	return b;
}

int main(void)
{
	_delay_ms(3000);
	DDRD |= (1<<PORTD6);
	uart_init();
	puls_comp.address_    = 0xE0;
	puls_comp.address_INV = 0xE0;
	puls_comp.command_    = 0x40;
	puls_comp.command_INV = 0xBF;
		puls_comp.address_    = reverse(puls_comp.address_);
		puls_comp.address_INV = reverse(puls_comp.address_INV);
		puls_comp.command_    = reverse(puls_comp.command_);
		puls_comp.command_INV = reverse(puls_comp.command_INV);
	/*int0_START();     
	serial_debug("signal:");*/
	 
	while (1)
	{
		/*int1_init();
		sei();
		_delay_ms(300);
		
		while(!(TIFR1 & (1<<TOV1)));     //wait until stop receiving, timer1 overflow works good

		cli();
		
		TIFR1 = (1<<TOV1);          //clear timer1 overflow flag
		trash =  read_tim1();                //stop timer1

		if ((puls_comp.start1 == 1) && (puls_comp.start2 == 1))
		{		
			
		puls_comp.address_    = reverse(puls_comp.address_);
		puls_comp.address_INV = reverse(puls_comp.address_INV);
		puls_comp.command_    = reverse(puls_comp.command_);
		puls_comp.command_INV = reverse(puls_comp.command_INV);	
		for (char z = 0;z < 8;z++)
		{*/
		serial_debug("sending:");	
		_delay_ms(1500);
		send_start_bit(); 
		send_8bits(puls_comp.address_);
		send_8bits(puls_comp.address_INV);
		send_8bits(puls_comp.command_);
		send_8bits(puls_comp.command_INV);
		send_end_bit();
		/*}
		serial_debug("signal");
		signal_num++;
		itoa(signal_num,buffer2,9);
		serial_debug(buffer2);
		serial_debug(":");
		}
			puls_num = 0;
			bit_num = 0;
			puls_comp.start1 = 0;
			puls_comp.start2 = 0;*/
		}
		}
		
				/*serial_debug("sending:");
				_delay_ms(1500);
				send_start_bit();
				send_8bits(puls_comp.address_);
				send_8bits(puls_comp.address_INV);
				send_8bits(puls_comp.command_);
				send_8bits(puls_comp.command_INV);
				send_end_bit();
				
				serial_debug("signal");
				signal_num++;
				itoa(signal_num,buffer2,9);
				serial_debug(buffer2);
				serial_debug(":");
			}
			puls_num = 0;
			bit_num = 0;
			puls_comp.start1 = 0;
			puls_comp.start2 = 0;
			puls_comp.address_ = 0;
			puls_comp.address_INV = 0;
			puls_comp.command_ = 0;
			puls_comp.command_INV = 0;*/

	/*	if (puls_comp.start1 == 1)
		{
			lcd_clear();
			lcd_goto_xy(0,0);
			lcd_write_word("start 1!");
		} 
		if (puls_comp.start2 == 1)
		{
			lcd_goto_xy(1,0);
			lcd_write_word("start 2!");
			_delay_ms(500);
		}*/
	
			/*serial_debug("address:");
		itoa(puls_comp.address_,buffer1,13);
		serial_debug(buffer1);
		
		serial_debug("address_INV:");
		itoa(puls_comp.address_INV,buffer1,13);
		serial_debug(buffer1);
		
		serial_debug("command:");
		itoa(puls_comp.command_,buffer1,13);
		serial_debug(buffer1);
		
		serial_debug("command_INV:");
		itoa(puls_comp.command_INV,buffer1,13);
		serial_debug(buffer1);*/
			
		//while(puls_comp.start1 == 0);