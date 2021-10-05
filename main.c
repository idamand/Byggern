/*
 * TestProgram.c
 *
 * Created: 31.08.2021 11:23:23
 * Author : idamand
 */ 

// should maybe have a define of F_CPU

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "sram.h"
#include "usart.h"
#include "adc_driver.h"
#include "oled_driver.h"

#define FOSC 4915200 //1843200 //Clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1



int main(void){

	//initialize uart and check that communication works	
	USART_Init(MYUBRR);
	fdevopen(USART_TRANSMIT,USART_Receive);
	printf("program started\r\n");
	
	// do setup for the external adress interface
	DDRC |= (1<<PC3); //force the MSB of the adress bus to be a write pin 
	MCUCR |= (1<<SRE); // enable external adress memory interface
	SFIOR |= (1<<XMM2); // release top ports of adress space for jtag (pc4-pc7)
	SFIOR &= ~(1<<XMM1); // release top ports of adress space for jtag (pc4-pc7)
	SFIOR &= ~(1<<XMM0); // release top ports of adress space for jtag (pc4-pc7)
	
	adc_setup();
	
    oled_init();
	//SRAM_test(true);
	//test_fonts_with_rsa('!');
	

	
	for (int i = 0; i<30; i++)
	{
		//printf("in for loop\r\n");
        //print_joystick_pos();
        //print_joystick_dir();
      //  print_all_adc_channels();
        //button_read(); //uncomenting this soimetimes makes the program constantly reset 
	}
	
	printf("program finished\r\n");
	
	return 0;
}

