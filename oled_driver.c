/*
 * oled_driver.c
 *
 * Created: 29.09.2021 16:46:43
 *  Author: idamand
 */ 

#include <avr/pgmspace.h>
#include <util/delay.h>
#include "fonts.h" //we have the fonts in fonts.h
#include "oled_driver.h"

#define OLED_ADRESS_CTRL 0x1000

volatile char *ext_ram_oled = (char *)OLED_ADRESS_CTRL; // Start address for the OLED
volatile char *ext_ram_oled_data = (char *)0x1200; // Start address for the OLED
//1200 - 10 bit

//menu

//choose level
// -> 1,2,3,4 
// close game
// letter menu
// -> a,b,c,d



//write command to oled
void write_c(uint8_t command){
	_delay_ms(1);
	
    ext_ram_oled[0] = command; //zero because we use address as control
    //printf("command to oled: %4d\r\n", command );
}

//write data to oled
void write_d(uint8_t data_to_write){
    ext_ram_oled_data[0] = data_to_write; //one because we use address as control
}

//helper function for test_fonts_with_rsa
void print_int_as_bits(uint8_t int_to_print){
	char buf1[8]; // ={'0','0','0','0','0','0','0','0'};
	itoa(int_to_print, buf1,2);
	printf("%s",buf1);
}

//this writes zeroes and ones to putty in the same way as data would be written to oled
// in order to make this into an oled print function the only thing that would need to be changed is
// print_int_as_bits -> write_d
void test_fonts_with_rsa(unsigned char test_char){
	printf("char selection test running\r\n");
	printf("index input to test function: %4d \r\n", test_char);
	printf("using font8\r\n");
	
	int font_array_outer_index = test_char-32; // the fonts[0] corresponds to char 32. 
	
	for(int i=0; i<8; i++){
		uint8_t fontval = pgm_read_byte(&(font8[font_array_outer_index][i]));
		print_int_as_bits(fontval);
		printf("\r\n");
	}
	
	
	
	/*
	int font_index = test_char;
	printf("font_index: %4d \r\n", font_index);
	
	for(int i= 0; i<8; ++i){
		
	}
	*/
}

//suggested in p.15 of oled-ly-190
void oled_init() {
    printf("initializing OLED");
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
    write_c(0x02);
    write_c(0xdb); //VCOM deselect level mode
    write_c(0x30);
    write_c(0xad);  //master configuration
    write_c(0x00);
    write_c(0xa4); //out follows RAM content
    write_c(0xa6); //set normal display
    write_c(0xaf);  // display on
    
	write_c(0xa5); // will light up the screen
	printf("OLED init finished");
	
}

// might be necessary with separate function for the commands that require arguments
void execute_oled_command(Oled_command command_to_execute){
	switch(command_to_execute){
		case RESET:
			//write_c(0xsomething) // add in relevant command
			//write_c(0xsomething) //A (in datasheet)
			break;
		case LIGHT_UP_SCREEEN:
			write_c(0xa5); // will light up the screen
			break;
		case  CLEAR:
			//write_c(0xsomething) // add in relevant command
			break;
	}
	//{RESET, LIGHT_UP_SCREEEN, CLEAR}
}

/*
void test_char_printing(char* in_char){
    printf("char selection test running");
    printf("in_char: %4d\r\n", in_char );
    printf("using font8");

    for (int i = 0; i < 8; ++i)
    {
        printf("byte %4d is: %4d \r\n", i, pgm_read_byte(&(font8[font_index][i])))
    }

    //print the entire font to test progmem
    //but what makes it difficult is that theree exist several possible representation of the numbers in binary
    for (int i = 0; i < 8; ++i) //loop all chars
    {

        for (int i = 0; i < 8; ++i) //columns
        {
            printf( "%4d", pgm_read_byte(&(font8[font_index][i])) )
        }
    }
}

void oled_print_char(char* in_char){
    uint_16_t ext_ram_size = 0x400;
    volatile char *ext_ram = (char *)0x0000 //start adress for the adc

    //the char corresponds to the index in the fonts
    font_index = in_char; //might be wrong so I move it to new var for easier editing

    //loop through second dimension of indexes to write the char
    //to begin with we just use font8 (todo: later add functionality for other fonts)
    for (int i = 0; i < sizeof(font8[font_index])/sizeof(font8[font_index][0]); ++i) // aka i<8
    {
        //write the current column of the content to the place the column pointer is

        DATA_SELECT_PIN=1; //force the pin connected to D/C# high, so that we atate that what is sent is data.

        // if the fonts were sotored in program memory it would be something like:
        // row_content_oled = font8[font_index][i];
        // but we are using progmem, so it looks like
        ext_ram[0] = pgm_read_byte(&(font8[font_index][i])); //should index be zero?
        //when writing we should make sure that the data bus is in data mode and not command mode.

        //where ext_ram[0] is row_content_oled (uncertain about this one).


        //the column pointer is automatically increemented when it is written to
        //so we can just jump to the top of the for loop again without that being a problem
        
    }
}

void oled_goto_line(line){
    //change the line pointed to
    //this is well explained in the instruction set
    //command 21h
    // or d3h
}

void oled_goto_column(column){
    //change the line pointed to
    //this is well explained in the instruction set
}


//uncertain what this should do
void oled_reset(){
}

//uncertain what this should do
void oled_home(){
}
*/