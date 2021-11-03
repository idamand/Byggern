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
	
	PIOA ->PIO_PER = PIO_PA19; // enable pin as output
	PIOA -> PIO_OER = PIO_PA19; // set as output
	PIOA -> PIO_PUDR = PIO_PA19;
	
	WDT -> WDT_MR = WDT_MR_WDDIS;
	
	//for delay 
	//SysTick -> CTRL |= 1; // 1st bit is enable. p.193 
	
	configure_uart();    

	PIOA -> PIO_SODR = PIO_PA19; // turn led high 
	PIOA -> PIO_CODR = PIO_PA19; // turn led low
	printf("Hello guys!\n\r");
	pwm_init();
	
	adc_init();
	
	printf("adc starting");
	int tempval1 = 0;
	
	for (int i=0; i<900; i++)
	{
		printf("ab %4d: \n\r", adc_get_val());
		
		if(adc_get_val()< 100){
			printf("BALL DETECTED!"); 
		
		}
		//sysTickDelay2(1);
		//adc_get_val();
		//printf("val: %4d",tempval1);
		
	}
	
	printf("adc finished");
	
	
	
	/*
	can_init_def_tx_rx_mb(0x29457);
	
	CAN_MESSAGE reception_message; 
	reception_message.id = 79; //just a random arbitrary number.
	uint8_t reception_mailbox_id = 1; //mailbox has to be either 1 or 0 according to CAN0_handler
		
	//CAN0_Handler(); // denne skal ikke kalles, den blir aktivert av en interrupt. // this is where we should clear shit (inside this function) 
	
	can_receive(&reception_message,reception_mailbox_id);
	
	//for (int i = 0; i<3; i++)
	//{
		if(!can_receive(&reception_message,reception_mailbox_id)){ //it returns 0-> sucess, 1-> faliure
			printf("recieved sucessfully");
		}
		else{
			printf("reception failed");
		}
	//}
	
	printf("reception message ID: %4d", reception_message.id);
	printf("reception message data[0] is: %4d", reception_message.data[0]);
	*/
	
	/*
	printf("setting to 90 deg");
	pwm_set_angle(90);

	printf("setting to 180 deg");
	pwm_set_angle(180);

	printf("setting to 0 deg");
	pwm_set_angle(0);
	*/
	printf("end of program");
	
}
