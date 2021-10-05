/*
 * oled_driver.h
 *
 * Created: 29.09.2021 16:47:30
 *  Author: idamand
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

typedef enum Oled_command {RESET, LIGHT_UP_SCREEEN, CLEAR} Oled_command;

void oled_init();
void test_fonts_with_rsa(unsigned char test_char);
void print_int_as_bits(uint8_t int_to_print);
void execute_oled_command(Oled_command command_to_execute);

#endif /* OLED_DRIVER_H_ */