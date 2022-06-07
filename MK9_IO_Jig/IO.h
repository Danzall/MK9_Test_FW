/*
 * IO.h
 *
 * Created: 2015-12-08 01:06:58 PM
 *  Author: Jeefo
 */ 


#ifndef IO_H_
#define IO_H_

void IO_Init();


#define TagOutPort PIND
#define TagOutPin PD5
#define TagInPort PORTG
#define TagInPin PG1


#define LED1Port PORTA
#define LED1Pin PA3
#define LED2Port PORTA
#define LED2Pin PA4
#define LED3Port PORTA
#define LED3Pin PA5
#define LED4Port PORTA
#define LED4Pin PA6
#define LED5Port PORTA
#define LED5Pin PA7


#define LED6Port PORTG
#define LED6Pin PG2

#define LED7Port PORTC
#define LED7Pin PC7
#define LED8Port PORTC
#define LED8Pin PC6
#define LED9Port PORTC
#define LED9Pin PC5

#define RPM_Port PORTC
#define RPM_Pin PC4

#define Panic_Port PORTC
#define Panic_Pin PC1

#define InputA_Port PORTC
#define InputA_Pin PC0

#define InputB_Port PORTC
#define InputB_Pin PC3

#define InputC_Port PORTC
#define InputC_Pin PC2


#define OutputA_Port PING
#define OutputA_Pin PG4

#define TagLED_Port PING
#define TagLED_Pin PG3

#define Immob_Port PINB
#define Immob_Pin PB7

#endif /* IO_H_ */