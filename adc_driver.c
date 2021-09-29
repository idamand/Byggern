/*
 * adc_driver.c
 *
 * Created: 14.09.2021 13:21:34
 *  Author: idamand
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <stdio.h>
#include "adc_driver.h"

//typedef enum Direction {LEFT, RIGHT, UP, DOWN, NEUTRAL} Direction;

//shoould have some defines for the adress settings of the adc.

uint8_t adc_read(int channel){
	//should just do a read of the memory space of the adc. 
	//test with voltage generator
	uint16_t ext_ram_size = 0x400; //ram size for adc 
	volatile char *ext_ram = (char *)0x1400; // Start address for the ADC
	 
	ext_ram[0]=0b11100000;
	_delay_us(50);
	
	uint8_t adc_values_arr[4]; 

	//printf("doing read of adc");
	for (int i=0; i<4; i++)
	{
		//uint8_t adc_raw_value = ext_ram[0];
		adc_values_arr[i] = ext_ram[0];
	}
	
	//printf("[%4d,  %4d,  %4d,  %4d] \r\n",  adc_values_arr[0],  adc_values_arr[1],  adc_values_arr[2],  adc_values_arr[3]);
	return(adc_values_arr[channel]);
}

void print_all_adc_channels(){
    uint8_t adc_values_arr[4]; 

	//printf("doing read of adc");
	for (int i=0; i<4; i++)
	{
		//uint8_t adc_raw_value = ext_ram[0];
		adc_values_arr[i] = adc_read(i);
	}
	
	printf("[%4d,  %4d,  %4d,  %4d] \r\n",  adc_values_arr[0],  adc_values_arr[1],  adc_values_arr[2],  adc_values_arr[3]);
}


void adc_setup(void){
	//make sure clock is turned on
	
	
	DDRD |=(1<<DDD4);
	
	cli();
	TCCR3A |= (1<<COM3A0);
	TCCR3B |= (1<<CS30);
	TCCR3B |= (1<<WGM32);
	OCR3A  = 1;
	sei(); 
	
	//make sure the multiplexer configuration in table 1 is set correctly
	//A0-A1 should be set in such a way that it reads analog 1 and then analog 2 
	
}


//returns angle rounded to nearest integer
// coord_num =0 -> x , coord_num =1 -> y, coord_num > 1 -> program crash
int adc_joytsick_position(int coord_num){
	int calibrated_zero_adc_num = 133; // will be from calibration later
	
	int max_amplitude_adc_num = 2*calibrated_zero_adc_num;
	int max_angle_joystick_factor= 2; // 1/sin(theta_max), in which we observe theta_max = 60 deg from looking at the joystick
	
	uint8_t x_adc = adc_read(2);

	
	uint8_t y_adc = adc_read(3);

	float theta_rad_x = asin( (x_adc - calibrated_zero_adc_num) / (float)(max_angle_joystick_factor * calibrated_zero_adc_num) );
	float theta_deg_x = (theta_rad_x*180)/3.14;
	
	
	float theta_rad_y = asin( ( y_adc - calibrated_zero_adc_num) / (float)(max_angle_joystick_factor * calibrated_zero_adc_num) );
	float theta_deg_y = (theta_rad_y*180)/3.14;
	
	float theta_arr[2] = {theta_deg_x, theta_deg_y};

	return (int)theta_arr[coord_num]; 
}

Direction adc_joystick_direction(){
	int x_angle = adc_joytsick_position(0);
	int y_angle = adc_joytsick_position(1);
	
	int x_y_thresh = 15; 
	
	if (abs(x_angle) >= abs(y_angle))
	{
		if (x_angle > x_y_thresh)
		{
			return RIGHT;
		}
		else if (x_angle< - x_y_thresh)
		{
			return LEFT;
		}
		else {
            return NEUTRAL;
        }
    }
    else {
   		if (y_angle > x_y_thresh)
		{
			return UP;
		}
		else if (y_angle < - x_y_thresh)
		{
			return  DOWN;
		}
		else {
            return NEUTRAL;
        }     
    }

    return NEUTRAL;
}

void print_joystick_dir(void){
    if(adc_joystick_direction() == NEUTRAL){
            printf("Neutral\r\n");
        }
        else if(adc_joystick_direction() == DOWN){
            printf("Down\r\n");
        }
        else if(adc_joystick_direction() == UP){
             printf("Up\r\n");
        }
        else if(adc_joystick_direction() == RIGHT){
            printf("Right\r\n");
        }
        else if(adc_joystick_direction() == LEFT){
            printf("Left\r\n");
        }
}

void print_joystick_pos(void){
    int x_angle = adc_joytsick_position(0);
    int y_angle = adc_joytsick_position(1);

    printf("x joystick degrees float: %4d E-2 \r\n", x_angle);
    printf("y joystick degrees float: %4d E-2 \r\n", y_angle);
}

void button_read(){
    
    //set direction as input
    DDRB &= ~(1<<PB0);
    DDRB &= ~(1<<PB1);
    
    //print the value from pb 0
    printf("left button val: %4d \r\n", PINB & (1<<PB0));
    printf("right button val: %4d \r\n", (PINB & (1<<PB1)) >> 1 ); // the last bitshift is just for the int to be 1 rather than 2 when the button is pressed.
}