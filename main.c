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
#include <string.h>
#include "var.h"
#include "ext_int.h"
#include "timer0.h"
#include "timer1.h"
#include "send_fun.h"
#include "uart.h"
#include "Internal_EEprom.h"




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
		if ((puls_time > 8950) && (puls_time < 9050 ) )
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
	DDRD &= ~(1<<PORTD7);
	uart_init();
	serial_debug("Start\n");
	
	/*eeprom_write(0 , 0xFF);
	eeprom_write(1 , 0xEF);
	eeprom_write(2 , 0x05);
	eeprom_write(3 , 0xFA);*/
	
	if ((PIND & (1<<PORTD7)) == 128)
	{
		while (signal_num <= 21)
		{
			int0_START();
			serial_debug("receiving:\n");	
			int1_init();
			sei();
			_delay_ms(300);		
			while(!(TIFR1 & (1<<TOV1)));     //wait until stop receiving, timer1 overflow works good
			cli();
			TIFR1 = (1<<TOV1);          //clear timer1 overflow flag
			trash =  read_tim1();                //stop timer1
			if ((puls_comp.start1 == 1) && (puls_comp.start2 == 1))
			{
				signal_num++;
				puls_comp.address_    = reverse(puls_comp.address_);
				puls_comp.address_INV = reverse(puls_comp.address_INV);
				puls_comp.command_    = reverse(puls_comp.command_);
				puls_comp.command_INV = reverse(puls_comp.command_INV);	
				
				eeprom_write(ee_adr , puls_comp.address_ );
				eeprom_write((ee_adr + 1) , puls_comp.address_INV);
				eeprom_write((ee_adr + 2) , puls_comp.command_ );
				eeprom_write((ee_adr + 3) , puls_comp.command_INV ); 
				ee_adr += 4;
				
				serial_debug("signal");
				itoa(signal_num,buffer2,9);
				serial_debug(buffer2);
				serial_debug(":");		
			}	
		
			puls_num = 0;
			bit_num = 0;
			puls_comp.start1 = 0;
			puls_comp.start2 = 0;					
		}
		int0_STOP();
		int1_STOP();
		//sei(); //if using other interrupts in the remaining of the program
		ee_adr = 0;
	}
	
	while (1)
	{	
		uart_recieve_string(buff);
		serial_debug(buff);
if (strcmp(buff,"power") == 0)
{
	puls_comp.address_ = eeprom_read(0);
	puls_comp.address_INV = eeprom_read(1);
	puls_comp.command_ = eeprom_read(2);
	puls_comp.command_INV = eeprom_read(3);

	serial_debug("Sending:power\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}

else if (strcmp(buff,"one") == 0)
{
	puls_comp.address_ = eeprom_read(4);
	puls_comp.address_INV = eeprom_read(5);
	puls_comp.command_ = eeprom_read(6);
	puls_comp.command_INV = eeprom_read(7);

	serial_debug("Sending:one\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}

else if (strcmp(buff,"two") == 0)
{
	puls_comp.address_ = eeprom_read(8);
	puls_comp.address_INV = eeprom_read(9);
	puls_comp.command_ = eeprom_read(10);
	puls_comp.command_INV = eeprom_read(11);

	serial_debug("Sending:two\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}

else if (strcmp(buff,"three") == 0)
{
	puls_comp.address_ = eeprom_read(12);
	puls_comp.address_INV = eeprom_read(13);
	puls_comp.command_ = eeprom_read(14);
	puls_comp.command_INV = eeprom_read(15);

	serial_debug("Sending:three\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"four") == 0)
{
	puls_comp.address_ = eeprom_read(16);
	puls_comp.address_INV = eeprom_read(17);
	puls_comp.command_ = eeprom_read(18);
	puls_comp.command_INV = eeprom_read(19);

	serial_debug("Sending:four\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"five") == 0)
{
	puls_comp.address_ = eeprom_read(20);
	puls_comp.address_INV = eeprom_read(21);
	puls_comp.command_ = eeprom_read(22);
	puls_comp.command_INV = eeprom_read(23);

	serial_debug("Sending:five\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"six") == 0)
{
	puls_comp.address_ = eeprom_read(24);
	puls_comp.address_INV = eeprom_read(25);
	puls_comp.command_ = eeprom_read(26);
	puls_comp.command_INV = eeprom_read(27);

	serial_debug("Sending:six\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"seven") == 0)
{
	puls_comp.address_ = eeprom_read(28);
	puls_comp.address_INV = eeprom_read(29);
	puls_comp.command_ = eeprom_read(30);
	puls_comp.command_INV = eeprom_read(31);

	serial_debug("Sending:seven\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"eight") == 0)
{
	puls_comp.address_ = eeprom_read(32);
	puls_comp.address_INV = eeprom_read(33);
	puls_comp.command_ = eeprom_read(34);
	puls_comp.command_INV = eeprom_read(35);

	serial_debug("Sending:eight\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"nine") == 0)
{
	puls_comp.address_ = eeprom_read(36);
	puls_comp.address_INV = eeprom_read(37);
	puls_comp.command_ = eeprom_read(38);
	puls_comp.command_INV = eeprom_read(39);

	serial_debug("Sending:nine\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"zero") == 0)
{
	puls_comp.address_ = eeprom_read(40);
	puls_comp.address_INV = eeprom_read(41);
	puls_comp.command_ = eeprom_read(42);
	puls_comp.command_INV = eeprom_read(43);

	serial_debug("Sending:zero\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"vup") == 0)
{
	puls_comp.address_ = eeprom_read(44);
	puls_comp.address_INV = eeprom_read(45);
	puls_comp.command_ = eeprom_read(46);
	puls_comp.command_INV = eeprom_read(47);

	serial_debug("Sending:vup\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"vdown") == 0)
{
	puls_comp.address_ = eeprom_read(48);
	puls_comp.address_INV = eeprom_read(49);
	puls_comp.command_ = eeprom_read(50);
	puls_comp.command_INV = eeprom_read(51);

	serial_debug("Sending:vdown\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"mute") == 0)
{
	puls_comp.address_ = eeprom_read(52);
	puls_comp.address_INV = eeprom_read(53);
	puls_comp.command_ = eeprom_read(54);
	puls_comp.command_INV = eeprom_read(55);

	serial_debug("Sending:mute\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"up") == 0)
{
	puls_comp.address_ = eeprom_read(56);
	puls_comp.address_INV = eeprom_read(57);
	puls_comp.command_ = eeprom_read(58);
	puls_comp.command_INV = eeprom_read(59);

	serial_debug("Sending:up\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"down") == 0)
{
	puls_comp.address_ = eeprom_read(60);
	puls_comp.address_INV = eeprom_read(61);
	puls_comp.command_ = eeprom_read(62);
	puls_comp.command_INV = eeprom_read(63);

	serial_debug("Sending:down\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"right") == 0)
{
	puls_comp.address_ = eeprom_read(64);
	puls_comp.address_INV = eeprom_read(65);
	puls_comp.command_ = eeprom_read(66);
	puls_comp.command_INV = eeprom_read(67);

	serial_debug("Sending:right\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"left") == 0)
{
	puls_comp.address_ = eeprom_read(68);
	puls_comp.address_INV = eeprom_read(69);
	puls_comp.command_ = eeprom_read(70);
	puls_comp.command_INV = eeprom_read(71);

	serial_debug("Sending:left\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"ok") == 0)
{
	puls_comp.address_ = eeprom_read(72);
	puls_comp.address_INV = eeprom_read(73);
	puls_comp.command_ = eeprom_read(74);
	puls_comp.command_INV = eeprom_read(75);

	serial_debug("Sending:ok\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
else if (strcmp(buff,"exit") == 0)
{
	puls_comp.address_ = eeprom_read(76);
	puls_comp.address_INV = eeprom_read(77);
	puls_comp.command_ = eeprom_read(78);
	puls_comp.command_INV = eeprom_read(78);

	serial_debug("Sending:exit\n");
	_delay_ms(300);
	send_start_bit();
	send_8bits(puls_comp.address_);
	send_8bits(puls_comp.address_INV);
	send_8bits(puls_comp.command_);
	send_8bits(puls_comp.command_INV);
	send_end_bit();
}
		else
		{
			serial_debug("No match!\n");
		}
		}
		}
		

//for (unsigned char i = ee_adr;i =< (ee_adr + 3);i++)
	
		/*puls_comp[0].address_    = reverse(0xFF);
		puls_comp[0].address_INV = reverse(0xFE);
		puls_comp[0].command_    = reverse(0x50);
		puls_comp[0].command_INV = reverse(0xAF);*/