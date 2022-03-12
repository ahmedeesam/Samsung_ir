/*
 * var.h
 *
 * Created: 16/12/2021 08:34:07 م
 *  Author: ahmed essam
 */ 


#ifndef VAR_H_
#define VAR_H_

char ee_adr = 0;
char trash;
int bit_num = 0;
int puls_num = 0;                  //number of pulses of one signal
char buffer1[16],buffer2[10];
int  puls_time = 0;                  //array of time per pulses for one signal
int signal_num = 0;                //number of received signal
char buff[7];

typedef struct
{
	char start1 ;
	char start2 ;
	char address_ ;
	char command_ ;
	char address_INV ;
	char command_INV ;
} puls_components;

puls_components puls_comp;



#endif /* VAR_H_ */