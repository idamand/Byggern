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
 
 
 // a shitty delay fuinction copied from a forum.
/*
void sysTickDelay2 (uint32_t count)
{
	//also seems systick counts downwards and not upwards :( 
	int32_t start, elapsed, reload;

	start = SysTick -> VAL;
	
	reload = SysTick->LOAD;
	while (1) {
		elapsed = start - SysTick->VAL;
		printf("systick val: %4d", SysTick -> VAL );
		printf("elapsed %4d", elapsed );
		//if (elapsed < 0)
		//elapsed += reload;  // Handle wrap.
		if (elapsed >= count){
			return;
		}
	}
}
*/
//#include <util/delay.h>

// pmc, offset, settling time 9 

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	// remember to clear wpen bit in pio write protect mode register
	//luckily it seems to be disabled because we can turn led on and off
    
	WDT -> WDT_MR = WDT_MR_WDDIS;
	//SysTick -> CTRL |= 1; // 1st bit is enable. p.193 //for delay, however it does not work well enough
	
	configure_uart();    
    
	printf("Hello guys!\n\r");
	pwm_init();
	adc_init();
    Led_init();
    
    Led_blink_once_quickly();
	//adc_print_whether_ball_detected();
    //pwm_test_a_few_angles();    

   // beginning of can experimentation ----------------------------------------------------------------------------------------------------------- 
   
	can_init_def_tx_rx_mb(0x290457); //(0x00140561); //
	
	CAN_MESSAGE reception_message; 
	reception_message.id = 79; //just a random arbitrary number.
	uint8_t reception_mailbox_id = 1; //mailbox has to be either 1 or 0 according to CAN0_handler
		
	//CAN0_Handler(); // denne skal ikke kalles, den blir aktivert av en interrupt. // this is where we should clear shit (inside this function) 
	
    printf("before calling can recieve");
	can_receive(&reception_message,reception_mailbox_id);
    printf("after calling can recieve");
	
	//for (int i = 0; i<3; i++)
	//{
		if(!can_receive(&reception_message,reception_mailbox_id)){ //it returns 0-> sucess, 1-> faliure
			printf("recieved sucessfully\n\r");
		}
		else{
			printf("reception failed\n\r");
		}
	
	
	printf("reception message ID: %4d", reception_message.id);
	printf("reception message data[1] is: %4d", reception_message.data[1]);
	
    // end of can experimentation -----------------------------------------------------------------------------------------------------------
	
	printf("end of program");
	
}
