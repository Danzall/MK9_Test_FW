/*
 * OW.h
 *
 * Created: 2015-12-10 09:11:08 AM
 *  Author: Jeefo
 */ 


#ifndef OW_H_
#define OW_H_

void OW_Reset_Wait();
void Presence();
void Delay(int in);
void ReadCommand();
int ReadBit();
void WriteByte(char byte_in);
void WriteBit(char in);
void WriteString();
unsigned long OW_Crc8( unsigned char *addr, int len);
void Reset();
void Set();
void OK();
typedef struct{
	int ignition;
	int inputA;
	int inputB;
	int inputC;
	int tagI;
	int rpm;
	int panic;
	int immob;
	int outputA;
	int tagled;
	}out;
void Faulty_OP();
void LED_Toggle();
void DisplayFault();
void ResetLED();
#endif /* OW_H_ */