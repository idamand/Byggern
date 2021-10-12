/*
 * usart.h
 *
 * Created: 14.09.2021 11:15:47
 *  Author: idamand
 */ 


#ifndef USART_H_
#define USART_H_


void USART_Init( unsigned int ubrr);
void USART_TRANSMIT(unsigned char data);
unsigned char USART_Receive( void);


#endif /* USART_H_ */