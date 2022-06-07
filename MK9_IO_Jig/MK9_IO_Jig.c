/*
 * MK9_IO_Jig.c
 *
 * Created: 2015-12-08 12:18:59 PM
 *  Author: Jeefo
 */ 


#include <avr/io.h>
#include "IO.h"
#include "OW.h"
#include "UART.h"
#include "Timer.h"

volatile int OF;
double timer = 0;
int main(void)
{
    IO_Init();
	Timer_Init();
	//Timer1Init();
	//UART_Init();
	SREG = 0x80;
	//sei();
	while(1)
    {
        //TODO:: Please write your application code 	
		if (OF == 1){
			OF = 0;
			timer++;
			
		}	
		//
		
		if (timer == 100){						//1 sec loop
			timer = 0;
			//SendChar(0x31);
			OW_Reset_Wait();
			//PORTA ^= (1<<PA3);
			//if((PIND&_BV(PD5))==_BV(PD5)) PORTA ^= (1<<PA5);
			//if((PING&_BV(PG4))==_BV(PG4)) PORTA ^= (1<<PA6);
		}
    }
}