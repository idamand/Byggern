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
#include "sram.h"
#include "usart.h"
#include "adc_driver.h"
#include "oled_driver.h"
#include "menu_oled.h"
#include "spi_driver.h"


#define FOSC 4915200 //1843200 //Clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1



int main(void){

	//initialize uart and check that communication works	
	USART_Init(MYUBRR); 
	//fdevopen(USART_TRANSMIT,USART_Receive);
	
	
	
	// do setup for the external adress interface
	DDRC |= (1<<PC3); //force the MSB of the adress bus to be a write pin 
	MCUCR |= (1<<SRE); // enable external adress memory interface
	SFIOR |= (1<<XMM2); // release top ports of adress space for jtag (pc4-pc7)
	SFIOR &= ~(1<<XMM1); // release top ports of adress space for jtag (pc4-pc7)
	SFIOR &= ~(1<<XMM0); // release top ports of adress space for jtag (pc4-pc7)
	
	//adc_setup();
	
	
	
    oled_init();
	fdevopen(oled_print_char,USART_Receive);
	
	oled_clear_screen(); 
	printf("program started"); oled_newline(); 
	
	SPI_MasterInit();
	
	for (int i = 0; i<15; i++)
	{
		SPI_MasterTransmit(0x05);
		SPI_MasterTransmit(0x06);
		
		SPI_MasterTransmit(0x07);
		SPI_MasterTransmit(0x07);
		SPI_MasterTransmit(0x07);
		
		
		SPI_MasterTransmit(0x11);
		SPI_MasterTransmit(0x11);
		SPI_MasterTransmit(0x11);
		SPI_MasterTransmit(0x11);
		SPI_MasterTransmit(0x11);
		
	}
	
	//update_current_menu(); //needs to be called in beginning as well to have menu initialize 
	
	//oled_write_menu_to_screen();
	//menu_go_in();
	//menu_go_down();
	
	//oled_print_char('\n');
	//oled_goto_line(2);
	
	
	//SRAM_test();
	
	/*
	for (int i = 0; i<30; i++)
	{
		_delay_ms(1000);
		if (adc_joystick_direction() == DOWN)
		{
			menu_go_down();
		}
		if (adc_joystick_direction() == UP){
			menu_go_up();
		}
		if (button_read(0)){ //read right button 
			menu_go_in();
		}
	}
	
	*/
	printf("program finished"); oled_newline(); 
	return 0;
}

