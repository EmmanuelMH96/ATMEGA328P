/* 
* USART.h
*
* Created: 2/3/2024 2:59:51 PM
* Author: Emmanuel Maya
*/


#ifndef __USART_H__
#define __USART_H__
#include <avr/io.h>

enum UMSEL {ASYNC, SYNC, MASTERSPI}; //Mode of operation
enum UPM {DISABLED, EVEN, ODD}; //Parity Mode
enum USBS {ONEBIT, TWOBIT}; // Stop Bit Select
				
class USART
{
//variables
public:
protected:
private:
	unsigned int fosc;
	unsigned int BAUD;
	unsigned int UBRRVAL;
	uint8_t frame_bits;
	UPM parity;
	USBS stop_bits;
	UMSEL op_mode;

//functions
public:
	USART();
	USART(unsigned int fosc, unsigned int baud, unsigned int urbrr, uint8_t bits, UPM parity, USBS stop):
		fosc(fosc), BAUD(baud), UBRRVAL(urbrr), frame_bits(bits), parity(parity), stop_bits(stop)
	{}
	void init();
	void transmit(unsigned char * msg);
	char readbyte();
	~USART();
protected:
	void transmitByte(unsigned char data);
private:
	//USART( const USART &c );
	//USART& operator=( const USART &c );
	
	

}; //USART

#endif //__USART_H__


/*Equations for Calculating Baud Rate Register Setting

BAUD Baud rate (in bits per second, bps) 
fOSC System oscillator clock frequency
UBRRn Contents of the UBRRnH and UBRRnL registers, (0-4095)

Asynchronous normal mode (U2Xn = 0)
	- Equation for Calculating Baud Rate
		BAUD = fosc /( 16*(UBRRn + 1))
		
	- Equation for CalCulating UBRRn Value
		UBRRn = (fosc / 16*BAUD) - 1
		
Asynchronous double speed mode (U2Xn = 1)
	- Equation for Calculating Baud Rate
		BAUD = fosc /( 8*(UBRRn + 1))

	- Equation for CalCulating UBRRn Value
		UBRRn = (fosc / 8*BAUD) - 1
		
Synchronous master mode
	- Equation for Calculating Baud Rate
		BAUD = fosc /( 8*(UBRRn + 1))

	- Equation for CalCulating UBRRn Value
		UBRRn = (fosc / 2*BAUD) - 1
		
Maximum External clocking frequency for synchronous slave modes
	fxck < fosc / 4
	Note that fosc depends on the stability of the system clock source. It is therefore recommended to add some margin to avoid
	possible loss of data due to frequency variations.
end-comment*/