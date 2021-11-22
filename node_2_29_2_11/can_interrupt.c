/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include <stdlib.h> // for abs function
#include "sam.h"
#include "can_controller.h"

#include "PWM_driver.h"
#include "motor_driver.h"
#include "solenoid.h"
#include "adc_sam.h"

//#include "../uart_and_printf/printf-stdarg.h"
#include "printf-stdarg.h"


#define DEBUG_INTERRUPT 0
#define BALL_DETECT_THRESHOLD 1500 // found experimentally, seems to be a good value
#define USE_PI_CONTROL 0 // if this is 0 speed controller will be used instead
#define MOTORSPEED_FACTOR 30 // if we choose to use speed controller, this will determine how agressive it is

//called from can handler when the can handler has decoded the message recieved from node 1
// it activates the relevant stuff (motors etc) on node 2, (not replying to node 1)
// all functions that run continiously on node 2 are called from this
void can_message_respond(int can_data_0, int can_data_1, int can_data_2, int can_data_3){

	int x_angle_joystick = can_data_0 - 30;
	int y_angle_joystick = can_data_1 -30;
	int button_pressed = can_data_2;

	if (button_pressed){solenoid_bounce();}
		
	adc_print_whether_ball_detected(BALL_DETECT_THRESHOLD,0);
	
	if (USE_PI_CONTROL){
		int target_position = 4000 - (4000/30)*x_angle_joystick;
		pi_control(target_position, 0.3, 0.05, 0);	
	}
	else{
		int target_velocity = x_angle_joystick*MOTORSPEED_FACTOR; 
		int motor_direction =  (x_angle_joystick>0) ? 1 : 0; // angle > 0 -> move to right -> direction = 1
		//printf("motor speed %4d\n\r", target_velocity); 
		motorbox_motor_move(abs(target_velocity), motor_direction);
	}
}


/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */
void CAN0_Handler( void )
{
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		
        can_message_respond(message.data[0], message.data[1], message.data[2], message.data[3]);
		
		for (int i = 0; i < message.data_length; i++)
		{
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}

