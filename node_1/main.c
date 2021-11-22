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

#define GAME_LOOP_PERIOD 20 // in milliseconds. Node 2 relies on this to work correctly



// the thing that happens after initialization while game is running, continiously running. 
// needs to be called every 20ms
void game_loop(){
	// create a can message to be used for testing
	struct CANmessage can_normal_msg;
	can_normal_msg.IDH = 0b1111111;
	can_normal_msg.IDL = 0b00000111; // 3 lowest bits are part of 11 bit ID
	can_normal_msg.length = 0x06;
	can_normal_msg.data[0] = 0x03; can_normal_msg.data[1] = 0x02; can_normal_msg.data[2] = 0x01;
	
	//just add offset of 30 (which is maximum negative angle) that will be subtracted in reciever. (in order to not send negative number over can)
	int joy_x = adc_joytsick_position(0) + 30 ;
	int joy_y = adc_joytsick_position(1) + 30;
	
	can_normal_msg.data[0] = joy_x;
	can_normal_msg.data[1] = joy_y;
	can_normal_msg.data[2] = button_read(0);
	
	CAN_send(can_normal_msg, 0x01);
}

int main(void){

	//initialize uart and check that communication works	
	USART_Init(MYUBRR); 
	
	//will need to be placed inside function defining wether we want uart debug stuff
	//fdevopen(USART_TRANSMIT,USART_Receive);

	printf("program started"); oled_newline(); 
	
	sram_setup();
    //SRAM_test();
    
	adc_setup();
	
    oled_init();
	fdevopen(oled_print_char,USART_Receive); //remember to set variable USE_OLED_FOR_PRINTF in oled.h
	
	//oled_clear_screen(); 
	
	//temporarily uncommented because they contain prints
	CAN_init(); 
	can_loopback_selftest(); 
		
	//menu stuff
	update_current_menu(); //needs to be called in beginning as well to have menu initialize 
	// then the menu loop should be called
	//for (int i = 0; i<70; i++){
	while (1){
		//menu_loop();
		if (menu_loop()!=-1)
		{
			printf("chosen");
			break;
		}
		_delay_ms(1000);
	}
	
	//game loop stuff
	
	//for (int i=0; i<250; i++)
	while (1)
	{
		game_loop(); 
		oled_turn_off();
		//menu_loop();
		// we can do oled updates inside here as well
		_delay_ms(GAME_LOOP_PERIOD);
	}
	
	
    //menu_loop()
	printf("program finished"); oled_newline();
	
	return 0;
}

