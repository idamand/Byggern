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
	
	
	
	// do setup for the external adress interface
	DDRC |= (1<<PC3); //force the MSB of the adress bus to be a write pin 
	MCUCR |= (1<<SRE); // enable external adress memory interface
	SFIOR |= (1<<XMM2); // release top ports of adress space for jtag (pc4-pc7)
	SFIOR &= ~(1<<XMM1); // release top ports of adress space for jtag (pc4-pc7)
	SFIOR &= ~(1<<XMM0); // release top ports of adress space for jtag (pc4-pc7)
	
	//adc_setup();
	
	
	
    oled_init();
	//fdevopen(oled_print_char,USART_Receive);
	
	oled_clear_screen(); 
	printf("program started"); oled_newline(); 
	
	//mcp_reset();
	CAN_init();
	
	uint8_t can_ctrl_reg_addr = 0x0f;
	uint8_t can_ctrl_content =  mcp_read(can_ctrl_reg_addr);
	printf("the initial content of canctrl is %4d: \r\n", can_ctrl_content);

	
	
	// create a can message to be used for testing
	struct CANmessage can_test_msg; 
	can_test_msg.IDH = 0b1111111;
	can_test_msg.IDL = 0b00000111; // 3 lowest bits are part of 11 bit ID
	can_test_msg.length = 0x06;
	can_test_msg.data[0] = 0x03; can_test_msg.data[1] = 0x02; can_test_msg.data[2] = 0x01;
	
	
	
	for (int i=0; i<600; i++)
	{
		CAN_send(can_test_msg, 0x01);
	}
	
	CAN_send(can_test_msg, 0x01); // to send to t1
	struct CANmessage can_test_recieve = CAN_receive(0x01); // to recieve from to r0
	printf("can test recieve r0 is: %4d, %4d, %4d\r\n", can_test_recieve.data[0], can_test_recieve.data[1], can_test_recieve.data[2]);
	struct CANmessage can_test_recieve_r1 = CAN_receive(0b00000010); // to recieve from to r1
	printf("can test recieve r1 is: %4d, %4d, %4d\r\n", can_test_recieve_r1.data[0], can_test_recieve_r1.data[1], can_test_recieve_r1.data[2]);	
	
	can_test_msg.data[0] = 0x09; can_test_msg.data[1] = 0x08; can_test_msg.data[2] = 0x07;
	CAN_send(can_test_msg, 0x01); // to send to t1
	can_test_recieve = CAN_receive(0x01); // to recieve from to r0
	printf("can test recieve r0 is: %4d, %4d, %4d\r\n", can_test_recieve.data[0], can_test_recieve.data[1], can_test_recieve.data[2]);
	can_test_recieve_r1 = CAN_receive(0b00000010); // to recieve from to r1
	printf("can test recieve r1 is: %4d, %4d, %4d\r\n", can_test_recieve_r1.data[0], can_test_recieve_r1.data[1], can_test_recieve_r1.data[2]);

/*
	
	printf("doing test of loading bufs");
	mcp_load_tx_buffer(0x05); // just arbitrary number
	uint8_t recieved1 = mcp_read_rx_buffer();
	printf("reading rx buffer yields: %4d: \r\n", recieved1);
*/
	printf("program finished"); oled_newline();


	/*
	uint8_t spi_recieved_bit = 5;	//5 is just arbitrary start value
	for (int i = 0; i<15; i++)
	{
		for (int j = 0; j<1000; j++){
			SPI_MasterTransmit(0x05);
			SPI_MasterTransmit(0x06);
			SPI_MasterTransmit(0x07);
			SPI_MasterTransmit(0x11);
		}
		SPI_MasterTransmit(0x05);
		SPI_MasterTransmit(0x06);
		spi_recieved_bit = SPI_MasterRead();
		printf("received:"); 
		printf(": %4d", spi_recieved_bit); 
		oled_newline();

	
		SPI_MasterTransmit(0x07);
		
		SPI_MasterTransmit(0x07);
		
		SPI_MasterTransmit(0x07);
		

	
		SPI_MasterTransmit(0x11);
		

		
		SPI_MasterTransmit(0x11);
		
		SPI_MasterTransmit(0x11);
		
		SPI_MasterTransmit(0x11);
		
		SPI_MasterTransmit(0x11);
		
		

	} */
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
	
	return 0;
}

