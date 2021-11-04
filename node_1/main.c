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
#include "CAN.h"
#include "mcp2515_driver.h"

#define FOSC 4915200 //1843200 //Clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1



int main(void){

	//initialize uart and check that communication works	
	USART_Init(MYUBRR); 
	fdevopen(USART_TRANSMIT,USART_Receive);

	printf("program started"); oled_newline(); 
	
	sram_setup();
    //SRAM_test();
    
	//adc_setup();
	
    //oled_init();
	//fdevopen(oled_print_char,USART_Receive); //remember to set variable USE_OLED_FOR_PRINTF in oled.h
	
	//oled_clear_screen(); 
	
	CAN_init();
	can_loopback_selftest(); 

	
	
	// create a can message to be used for testing
	struct CANmessage can_normal_msg; 
	can_normal_msg.IDH = 0b1111111;
	can_normal_msg.IDL = 0b00000111; // 3 lowest bits are part of 11 bit ID
	can_normal_msg.length = 0x06;
	can_normal_msg.data[0] = 0x03; can_normal_msg.data[1] = 0x02; can_normal_msg.data[2] = 0x01;
	
	
	for (int i=0; i<600; i++)
	{
		CAN_send(can_normal_msg, 0x01);
	}
	
    
    /*

*/
    
	printf("program finished"); oled_newline();



	//update_current_menu(); //needs to be called in beginning as well to have menu initialize 
	
	//oled_write_menu_to_screen();
	//menu_go_in();
	//menu_go_down();
	
	//oled_print_char('\n');
	//oled_goto_line(2);
	
	
	
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
	
	return 0;
}

