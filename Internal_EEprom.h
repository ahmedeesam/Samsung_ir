/*
 * Internal_EEprom.h
 *
 * Created: 06/03/2022 03:37:49 م
 *  Author: ahmed essam
 */ 
#include <avr/io.h>
#include "uart.h"

#ifndef INTERNAL_EEPROM_H_
#define INTERNAL_EEPROM_H_



void eeprom_write(char addr , char data);
char eeprom_read(char addr);


#endif /* INTERNAL_EEPROM_H_ */