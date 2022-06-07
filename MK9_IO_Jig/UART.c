/*
 * UART.c
 *
 * Created: 2015-12-10 09:17:41 AM
 *  Author: Jeefo
 */ 
#include <avr/io.h>

void UART_Init(){
	UBRR0H=0;
	//UBRR0L=p_baud;
	UBRR0L=103;

	UCSR0B=0xd8;
	UCSR0C=0x06;
	
	DDRE |= (1<<PE1);
}

void SendChar(char in){
	//PORTA ^= (1<<PA4);
	UDR0 = in;
	while (!(UCSR0A&(1<<UDRE0)));
}
/*
SIGNAL (USART0_RX_vect){
	PORTA ^= (1<<PA5);
}*/