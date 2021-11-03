/*
 * usart.c
 *
 * Created: 14.09.2021 11:15:05
 *  Author: idamand
 */ 

#include <avr/io.h>

void USART_Init( unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/*Set frame format: 8data, 2stop bit*/
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00); // studass said this numbering is correct

}

void USART_TRANSMIT(unsigned char data){
	/*wait for empty transmit buffer*/
	while(!(UCSR0A & (1<<UDRE0))  ) //note:UCSR0A OR 1A?
	;
	

	// transmit the data
	UDR0 = data;

}

unsigned char USART_Receive( void)
{
	/*Wait for data to be received*/
	//TODO: change into interrupt instead
	//TODO: make this work.
	while(!(UCSR0A & (1<<RXC0)))
	;
	/*Get and return received data from buffer*/
	return UDR0;
}
