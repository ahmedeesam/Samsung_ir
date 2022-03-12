/*
 * send_fun.c
 *
 * Created: 14/01/2022 09:28:52 م
 *  Author: ahmed essam
 */ 

#include "send_fun.h"
#include "timer0.h"

void send_start_bit()
{
	start_tim0_38khz(); //  9ms High pulse
	start_tim1_ctc(9000);
	see_compare();
	
	stop_tim0_38khz();  // 4.5ms Low pulse
	start_tim1_ctc(4500);
	see_compare();
}

void send_8bits(int a)
{
	for (char i = 0;i <= 7;i++)
	{
		if ((a & (1<<i)) == 0)   //send zero
		{
			start_tim0_38khz();   //start 38461khz with dutycycle 50% ,time interval 13us ,on OC0A pin without interrupt //  562us High pulse
			serial_debug("0");
			start_tim1_ctc(562);
			see_compare();
			
			stop_tim0_38khz();  // 562us Low pulse
			start_tim1_ctc(562);
			see_compare();	
		}
		else if ((a & (1<<i)) != 0)
		{
			start_tim0_38khz(); //  562us High pulse
			serial_debug("1");
			start_tim1_ctc(562);
			see_compare();
			
			stop_tim0_38khz();  // 1687us Low pulse
			start_tim1_ctc(1687);
			see_compare();			
		}
	}
}

void send_0_bit()
{
			start_tim0_38khz();   //start 38461khz with dutycycle 50% ,time interval 13us ,on OC0A pin without interrupt //  562us High pulse
			serial_debug("0");
			start_tim1_ctc(562);
			see_compare();
			
			stop_tim0_38khz();  // 562us Low pulse
			start_tim1_ctc(562);
			see_compare();
}

void send_1_bit()
{
			start_tim0_38khz(); //  562us High pulse
			serial_debug("1");
			start_tim1_ctc(562);
			see_compare();
			
			stop_tim0_38khz();  // 1687us Low pulse
			start_tim1_ctc(1687);
			see_compare();
}

void send_end_bit()
{
	start_tim0_38khz(); //  562us High pulse
	start_tim1_ctc(562);
	see_compare();	
	stop_tim0_38khz();
}