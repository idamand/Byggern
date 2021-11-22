/*
 * node_2_29.c
 *
 * Created: 01.11.2021 12:05:55
 * Author : raffaelv
 */ 


#include "sam.h"
#include "printf-stdarg.h"
#include "uart.h"
#include "PWM_driver.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "system_sam3xa.h"
#include "Led.h" 
#include "timer.h"
#include "adc_sam.h"
#include "motor_driver.h"
#include "solenoid.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	// remember to clear wpen bit in pio write protect mode register
	//luckily it seems to be disabled because we can turn led on and off
    
	WDT -> WDT_MR = WDT_MR_WDDIS;
	
	configure_uart();    
    
	printf("Hello guys!\n\r");
	
	//initialize systems
	adc_init();
	systick_init(); //configure systick handler to be called at regular intervals. 
	pwm_init();
    Led_init();
	motorbox_init();
	solenoid_init();
	
	//for testing:
    //Led_blink_once_quickly(); //too quick to see without breakpoints
    //solenoid_bounce(); 
	//adc_print_whether_ball_detected();
    pwm_test_a_few_angles();    


	//initialize CAN
	//after this function is called can messages from node 1 will trigger CAN0_Handler and in turn can_message_respond in file can_interrupt.c
	//can_message:respond is what calls the pid
	//it is also what calls the print wether adc detected
	//and in general all other services running after initialization 
	can_init_def_tx_rx_mb(0x290457); //(0x00140561); 
	
	
	//move motor all the way to the right, we want that as the starting point for our game 
	//motorbox_motor_move(4000, 1);
	delay_ms(500);
	motorbox_motor_move(0, 1);
	printf("encoder value (should be 0): %4d", motorbox_read_encoder()); 
	
	printf("end of program");
	
}
