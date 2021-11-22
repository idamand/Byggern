/*
 * oled_driver.h
 *
 * Created: 29.09.2021 16:47:30
 *  Author: idamand
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_


#define USE_OLED_FOR_PRINTF 1 // Necessary for oled newline. if 1 it does not printf \r\n, but rather gives newline on oled

void oled_init();
void oled_print_char(char* in_char);

//writes a lot of zeroes to oled data memory
void oled_invert_display(); 
void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);

void oled_newline();
void oled_clear_screen();
void oled_turn_off();

#endif /* OLED_DRIVER_H_ */