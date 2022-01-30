/*
 * pin_change_INT.c
 *
 * Created: 20/12/2021 07:37:26 م
 *  Author: ahmed essam
 */ 
#include "pin_change_INT.h"

void enable_PCI_4()
{
	PCICR |= (1<<PCIE0);
	PCMSK0 = (1<<PCINT4);
}