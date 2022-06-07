/*
 * Timer.c
 *
 * Created: 2015-12-08 03:24:38 PM
 *  Author: Jeefo
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
extern int OF;
int time = 0;

void Timer_Init(){
	TCCR0=0x0f;
	OCR0=0x4d;
	TIMSK|=_BV(OCIE0);
}

void Timer1Init(){
	TCCR1A=0x00;			//normal operation
	TCCR1B=0x09;			//clock set, mode set
	OCR1AH = 0;
	OCR1AL = 1;
	TIMSK|=_BV(OCIE1A);
	TIMSK|=_BV(TOIE1);
}

void ResetT1(){
	time = 0;
}

int ReadT1(){
	
	return time;
}

SIGNAL (TIMER0_COMP_vect){
	OF = 1;
}

SIGNAL (TIMER1_OVF_vect	){
	
}

SIGNAL (TIMER1_COMPA_vect){
	OF = 1;
	time++;
}