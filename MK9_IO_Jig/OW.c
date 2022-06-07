/*
 * OW.c
 *
 * Created: 2015-12-10 09:10:46 AM
 *  Author: Jeefo
 */ 
#include <avr/io.h>
#include "OW.h"
#include "Timer.h"
#include "IO.h"
int delay;
char cmd;
char OW_String[8];
//int outputA_d = 0;
//int immob_d = 0;
//int tagled_d = 0;
out Default;
out triggered;
out faulty;
int faulty_outputs;

void OW_Reset_Wait(){										//1000 count = 750us	//
	delay = 0;
	if((TagOutPort&_BV(TagOutPin))==_BV(TagOutPin))
		while ((TagOutPort&_BV(TagOutPin)) == _BV(TagOutPin)) ;				//wait while high
		//ResetT1();
	while ((TagOutPort&_BV(TagOutPin)) == 0){ 
		//
		delay++;											//count while low
	}
	//if ((PIND&_BV(PD5)) == _BV(PD5)) 
	//delay = ReadT1();
	//if (delay > 415) Presence();							//415 = 680us
	if (delay > 380) Presence();							//415 = 680us
	//if ((PIND5&_BV(1<<PD5)) == 0) PORTA ^= (1<<PA6); 
} 

void Presence(){	
	delay = 26;
	while (delay > 1) delay--;
	//Delay(20);
	TagInPort ^= (1<<TagInPin);
	delay = 260;
	while (delay > 1) delay--;
	//Delay(200);
	TagInPort ^= (1<<TagInPin);
	delay = 46;
	while (delay > 1) delay--;
	//Delay(20);
	ReadCommand();
	//LED_Toggle();
	//
}

void ReadCommand(){
	int bit;
	cmd = 0;
	for (bit = 0; bit < 8; bit++){
		//cmd = ReadBit();
		cmd =  cmd >> 1;
		if (ReadBit() == 1) cmd |= 0x80;		
		//if (cmd == 0x48) LED4Port ^= (1<<LED4Pin);
	}
	if (cmd == 9){ 
		Reset();	
		OK();
		ResetLED();
	}
	
	if (cmd == 0x10){
		Set();
		OK();
	}
	if (cmd == 0x11){															//read default outputs
		if((Immob_Port&_BV(Immob_Pin))==_BV(Immob_Pin)) Default.immob = 0;
		else Default.immob = 1;
		if((OutputA_Port&_BV(OutputA_Pin))==_BV(OutputA_Pin)) Default.outputA = 0;
		else Default.outputA = 1;
		if((TagLED_Port&_BV(TagLED_Pin))==_BV(TagLED_Pin)) Default.tagled = 0;
		else Default.tagled = 1;
		OK();
	}
	if (cmd == 0x12){															//read trigerd outputs
		if((Immob_Port&_BV(Immob_Pin))==_BV(Immob_Pin)) triggered.immob = 1;
		else triggered.immob = 0;
		if((OutputA_Port&_BV(OutputA_Pin))==_BV(OutputA_Pin)) triggered.outputA = 1;
		else triggered.outputA = 0;
		if((TagLED_Port&_BV(TagLED_Pin))==_BV(TagLED_Pin)) triggered.tagled = 1;
		else triggered.tagled = 0;
		OK();
		Faulty_OP();
	}
	if (cmd == 0x13){															//send nubmer of faulty outputs
		
		faulty_outputs = faulty.immob + faulty.outputA + faulty.tagled;
		OW_String[0] = faulty_outputs;	
		WriteString();	
	}
	if (cmd == 0x14){															//send faulty outputs
		if (faulty.immob == 1) OW_String[0] = 1;
		if (faulty.tagled == 1) OW_String[1] = 1;
		if (faulty.outputA == 1) OW_String[2] = 1;
		WriteString();
	}
	if (cmd == 0x15){
		DisplayFault();
	}
	if (cmd > 128){															//decode input faults
		if ((cmd & 0x01) == 0x01){ 
			LED8Port ^= (1<<LED8Pin);
			faulty.ignition = 1;
		}
		if ((cmd & 0x02) == 0x02) faulty.panic = 1;
		if ((cmd & 0x04) == 0x04) faulty.rpm = 1;
		if ((cmd & 0x08) == 0x08) faulty.inputA = 1;
		if ((cmd & 0x10) == 0x10) faulty.inputB = 1;
		if ((cmd & 0x20) == 0x20) faulty.inputC = 1;
	}
}

void OK(){
	OW_String[0] = 0x20;
	WriteString();
}

void Faulty_OP(){
	//if (Default.immob == triggered.immob){ 
	if (Default.immob != triggered.immob){ 
		faulty.immob = 1;
	}
	else faulty.immob = 0;
	if (Default.tagled != triggered.tagled){ 
		faulty.tagled = 1;
	}
	else faulty.tagled = 0;
	if (Default.outputA != triggered.outputA){ 
		faulty.outputA = 1;
	}
	else faulty.outputA = 0;
}

int ReadBit(){
	delay = 0;
	if((TagOutPort&_BV(TagOutPin))==_BV(TagOutPin))
		while ((TagOutPort&_BV(TagOutPin)) == _BV(TagOutPin)) ;				//wait while high
	while ((TagOutPort&_BV(TagOutPin)) == 0) delay++;				//count while low
	if (delay > 20){ 
		return 0;
	}
	else{ 
		return 1;
	}
	
}

void WriteString(){
	int bytes;
	for (bytes = 0; bytes < 4; bytes++){
		//if ((PIND&_BV(PD5)) == 0)
		//while ((PIND&_BV(PD5)) == 0);		//wait if OW line isn't pulled up yet
		WriteByte(OW_String[bytes]);
	}
}

void WriteByte(char byte_in){
	int bit;
	for (bit = 0; bit < 8; bit++){
		if((TagOutPort&_BV(TagOutPin))==_BV(TagOutPin))
		while ((TagOutPort&_BV(TagOutPin)) == _BV(TagOutPin)) ;				//wait while high
		if ((byte_in & 0x01) == 0x01) WriteBit(1);
		else WriteBit(0);
		byte_in = byte_in >> 1;
		
	}

}

/*void WriteBit(char in){
	if (in == 1) delay = 10;
	else delay = 60;
	PORTG |= (1<<PG1);
	while (delay) delay--;				//count while low
	PORTG &= ~(1<<PG1);
	while ((PIND&_BV(PD5)) == 0);		//wait if OW line isn't pulled up yet
}*/

void WriteBit(char in){
	if (in == 1) delay = 15;
	else delay = 80;
	TagInPort |= (1<<TagInPin);
	while (delay) delay--;							//count while low
	TagInPort &= ~(1<<TagInPin);
	while ((TagOutPort&_BV(TagOutPin)) == 0);		//wait if OW line isn't pulled up yet
}

unsigned long OW_Crc8( unsigned char *addr, int len){
	uint8_t crc=0;
	int i;
	char tt[10];

	for (i=0; i<len;i++){
		unsigned char inbyte = addr[i];
		int j;
		for (j=0;j<8;j++){
			unsigned long mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix)
			crc ^= 0x8c; //
			inbyte >>= 1;
		}
	}
	return crc;
}

void Reset(){
	TagInPort &= ~(1<<TagInPin);
	RPM_Port &= ~(1<<RPM_Pin);
	Panic_Port &= ~(1<<Panic_Pin);
	InputA_Port &= ~(1<<InputA_Pin);
	InputB_Port &= ~(1<<InputB_Pin);
	InputC_Port &= ~(1<<InputC_Pin);
	//TagInPort &= ~(1<<TagInPin);
	
	faulty.ignition = 0;
	faulty.panic = 0;
	faulty.rpm = 0;
	faulty.inputA= 0;
	faulty.inputB = 0;
	faulty.inputC = 0;
	faulty.immob = 0;
	faulty.tagled = 0;
	faulty.outputA = 0;
}

void ResetLED(){
	LED1Port&=(_BV(LED1Pin )^0xff);
	LED2Port&=(_BV(LED2Pin )^0xff);
	LED3Port&=(_BV(LED3Pin )^0xff);
	LED4Port&=(_BV(LED4Pin )^0xff);
	LED5Port&=(_BV(LED5Pin )^0xff);
	LED6Port&=(_BV(LED6Pin )^0xff);
	LED7Port&=(_BV(LED7Pin )^0xff);
	LED8Port&=(_BV(LED8Pin )^0xff);
	LED9Port&=(_BV(LED9Pin )^0xff);
}

void Set(){
	TagInPort |= (1<<TagInPin);
	RPM_Port |= (1<<RPM_Pin);
	Panic_Port |= (1<<Panic_Pin);
	InputA_Port |= (1<<InputA_Pin);
	InputB_Port |= (1<<InputB_Pin);
	InputC_Port |= (1<<InputC_Pin);
}

void LED_Toggle(){
	LED1Port ^= (1<<LED1Pin);
	LED2Port ^= (1<<LED2Pin);
	LED3Port ^= (1<<LED3Pin);
	LED4Port ^= (1<<LED4Pin);
	LED5Port ^= (1<<LED5Pin);
	LED6Port ^= (1<<LED6Pin);
	LED7Port ^= (1<<LED7Pin);
	LED8Port ^= (1<<LED8Pin);
	//LED9Port ^= (1<<LED9Pin);
}

void DisplayFault(){
	ResetLED();
	if (faulty.ignition == 1) LED1Port|=_BV(LED1Pin );
	if (faulty.panic == 1) LED2Port|=_BV(LED2Pin );
	if (faulty.rpm == 1) LED3Port|=_BV(LED3Pin );
	if (faulty.inputA == 1) LED4Port|=_BV(LED4Pin );
	if (faulty.inputB == 1) LED5Port|=_BV(LED5Pin );
	if (faulty.inputC == 1) LED6Port|=_BV(LED6Pin );
	
	if (faulty.immob == 1) LED7Port|=_BV(LED7Pin );
	if (faulty.tagled == 1) LED8Port|=_BV(LED8Pin );
	if (faulty.outputA == 1) LED9Port|=_BV(LED9Pin );
}

void Delay(int in){
	//in *= 1.3;
	while (in > 1) in--;
}

