/*
 * adc_driver.h
 *
 * Created: 14.09.2021 13:20:06
 *  Author: idamand
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

typedef enum Direction {LEFT, RIGHT, UP, DOWN, NEUTRAL} Direction;

void adc_setup(void);

//returns adc value of selected channel (channels 1..4) left_pad = 1, right_pad = 2 joy_x=3, joy_y= 4 
uint8_t adc_read(int channel); 

//returns angle rounded to nearest integer
// coord_num =0 -> x , coord_num =1 -> y, coord_num > 1 -> program doesnt crash, just random value return :(
int adc_joytsick_position(int coord_num);

//int adc_slider_val_left(){return adc_read(0);}
//int adc_slider_val_right(){return adc_read(1);}

Direction adc_joystick_direction(); //get_joystick_direction();
void print_joystick_dir(void);
void print_joystick_pos(void);
void print_all_adc_channels(void); 

int button_read(int button_num); // button num 0-> left, 1 -> right 

#endif /* ADC_DRIVER_H_ */