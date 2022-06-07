/*
 * IO.c
 *
 * Created: 2015-12-08 01:06:44 PM
 *  Author: Jeefo
 */ 

#include <avr/io.h>

void IO_Init(){
	DDRA |= (1<<PA3)|(1<<PA4)|(1<<PA5)|(1<<PA6)|(1<<PA7);		//LEDs
	DDRG |= (1<<PG2);
	DDRC |= (1<<PC5)|(1<<PC6)|(1<<PC7);
	
	DDRC |= (1<<PC0)|(1<<PC1)|(1<<PC2)|(1<<PC3)|(1<<PC4);		//outputs
	DDRG |= (1<<PG1);
	
	
	//DDRD &= ~(1<<PD5);
	//DDRD &= !_BV(PD5);
	DDRD = 0x00;
	PORTD |= (1<<PD5);											//inputs
	PORTG |= (1<<PG3)|(1<<PG4);
	PORTB |= (1<<PB7);
	
}