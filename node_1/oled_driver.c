/*
 * oled_driver.c
 *
 * Created: 29.09.2021 16:46:43
 *  Author: idamand
 */ 

#include <avr/pgmspace.h>
#include "fonts.h"
//we have the fonts in fonts.h

volatile char *oled_command_addr = (char *)0x1000; // Start address for the OLED
volatile char *oled_data_addr = (char *)0x1200; // Start address for the OLED

uint8_t oled_cursor_line = 0; // the placement we want the cursor to be at

//1200 - 10 bit


//write command to oled
void write_c(uint8_t command){
    oled_command_addr[0] = command;
}

//write data to oled
void write_d(uint8_t data_to_write){
    oled_data_addr[0] = data_to_write;
}




//suggested in p.15 of oled-ly-190
void oled_init() {
    write_c(0xae); // display off
    write_c(0xa1); //segment remap
    write_c(0xda); //common pads hardware: alternative
    write_c(0x12);
    write_c(0xc8); //common output scan direction:com63~com0
    write_c(0xa8); //multiplex ration mode:63
    write_c(0x3f);
    write_c(0xd5); //display divide ratio/osc. freq. mode
    write_c(0x80);
    write_c(0x81);  //contrast control
    write_c(0x50);
    write_c(0xd9); //set pre-charge period
    write_c(0x21);
    write_c(0x20); //Set Memory Addressing Mode
    write_c(0x02);		// page adressing mode
    write_c(0xdb); //VCOM deselect level mode
    write_c(0x30);
    write_c(0xad);  //master configuration
    write_c(0x00);
    write_c(0xa4); //out follows RAM content
    write_c(0xa6); //set normal display
    write_c(0xaf);  // display on
    write_c(0xa5); // will light up the screen, useful to set breakpoint here for debugging purpouses
	write_c(0xa4); // undo lighting up the entire screen, so that you can start writing stuff
}


void oled_print_char(char* in_char){
	
	/*if (in_char == 10){ //newline
		oled_newline();
	}
	else{*/
		
	int font_index = in_char - 32;
	
	for (int i = 0; i < 8; ++i)
	{
		write_d(pgm_read_byte(&(font8[font_index][i]))  );
	}
	
	
	
}

// to fill the screen with zeroes, making it turn black
void oled_clear_screen(){
	//should be done 896 times, but for safetys sake i do it fewer 
	for (int i = 0; i < 8; ++i) // row*col = 7*128 =  896 
	{
		oled_goto_line(i);
		for (int j = 0; j < 128; ++j)
		{
			write_d(0);	
		}
	}
	oled_goto_line(0);
}

void oled_invert_display(){
	write_c(0xa7); //command to invert the display
}

void oled_newline(){
	if (oled_cursor_line<7)
	{
		oled_goto_line(oled_cursor_line+1);
	}
	else{
		oled_goto_line(0);
	}
	oled_goto_column(0);
}

void oled_goto_line(uint8_t line){
	// line corresponds to the last three bits, so the command is 10110 x_1 x_2 x_3
	
	write_c(0xb0 | line); //set page start for page adressing mode
	oled_cursor_line = line; 
}

void oled_goto_column(uint8_t column){
    //change the column pointed to
	uint8_t lower_nibble = (column & 0b00001111);
	uint8_t upper_nibble = (column >> 4); 
	write_c(0x0|lower_nibble); // set lower nibble of column
	write_c(0x10|upper_nibble); // set upper nibble of column
}


//uncertain what this should do
void oled_reset(){
}

//uncertain what this should do
void oled_home(){
}
