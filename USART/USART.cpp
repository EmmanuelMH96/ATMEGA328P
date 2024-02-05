/* 
* USART.cpp
*
* Created: 2/3/2024 2:59:51 PM
* Author: Emmanuel Maya
*/


#include "USART.h"

// default constructor
USART::USART()
{
	fosc = 1000000;
	BAUD = 9600;
	UBRRVAL = 6;
	frame_bits = 8;
	parity = DISABLED;
	stop_bits = ONEBIT;
	op_mode = ASYNC;
} //USART

// default destructor
USART::~USART()
{
} //~USART

void USART::init()
{
	//Set Baud Rate
	UBRR0H = (UBRRVAL>>8);	//High byte
	UBRR0L = UBRRVAL;		//Low byte
	
	//Enable Receiver and Transmitter
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	
	//FRAME FORMAT
	//Set Operation Mode
	switch (op_mode)
	{
		case ASYNC:	UCSR0C |= (0<<UMSEL01)|(0<<UMSEL00); break;
		case SYNC:	UCSR0C |= (0<<UMSEL01)|(1<<UMSEL00); break;
		case MASTERSPI: UCSR0C |= (1<<UMSEL01)|(1<<UMSEL00); break;
		default: UCSR0C |= (0<<UMSEL01)|(0<<UMSEL00); break; //Normal Asynchronous mode
	}
	
	//Parity mode
	switch (parity)
	{
		case DISABLED:  UCSR0C |= (0<<UPM01) | (0<<UPM00); break; //No parity
		case EVEN:		UCSR0C |= (1<<UPM01) | (0<<UPM00); break;
		case ODD:		UCSR0C |= (0<<UPM01) | (1<<UPM00); break;
		default: UCSR0C |= (0<<UPM01) | (0<<UPM00); //No parity
	}
	
	//Stop Bits
	if (stop_bits == ONEBIT)
		UCSR0C |= (0<<USBS0); //1-bit parity
	else
		UCSR0C |= (1<<USBS0); //2-bit parity
	//data frame size
	switch (frame_bits)
	{
		case 5: UCSR0C |= (0<<UCSZ02)|(0<<UCSZ01)|(0<<UCSZ00); break;
		case 6: UCSR0C |= (0<<UCSZ02)|(0<<UCSZ01)|(1<<UCSZ00); break;
		case 7: UCSR0C |= (0<<UCSZ02)|(1<<UCSZ01)|(0<<UCSZ00); break;
		case 8: UCSR0C |= (0<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00); break;
		case 9: UCSR0C |= (1<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00); break;
		default: UCSR0C |= (0<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00); // 8 bit default
	}
}

void USART::transmitByte(unsigned char data)
{
	//Wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data; //Puts the data to be transferred
}

void USART::transmit(unsigned char * msg)
{
	unsigned int i = 0;
	while (msg[i] != '\0')
	{
		transmitByte(msg[i]);
		i++;
	}
}

char USART::readbyte()
{
	if(UCSR0A & (1<<7))
		return UDR0;
	else
		return 0;	
}