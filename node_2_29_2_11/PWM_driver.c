/*
 * PWM_driver.c
 *
 * Created: 01.11.2021 14:52:41
 *  Author: raffaelv
 */ 

#include "sam.h"
#include "printf-stdarg.h"

void pwm_init(){
	// disable write protection
	PIOC ->  PIO_WPMR = 0x50494F; // pio in ascii, it is the passcode rquired


	//enable pwm in power manager
	PMC -> PMC_PCER1 = PMC_PCER1_PID36; 
	

	//disable pio and give acess to pins over to pwm 
	PIOC -> PIO_PDR |= PIO_PDR_P18;
	PIOC -> PIO_ABSR |= PIO_ABSR_P18;

	//p18 corresponds to channel 6 and pin 45
	PWM -> PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024;

	PWM -> PWM_CH_NUM[6].PWM_CPRD = 1641;

	PWM -> PWM_CH_NUM[6].PWM_CDTY = 123; // (1.5* 84M/1024) = 123, middle position
	
	// flip the waveform upside down. CPOL = 1, see page 1044 and page 980 of datasheet
	PWM -> PWM_CH_NUM[6].PWM_CMR |= (1<<9);  

	// then remember to enable pwm 
	PWM -> PWM_ENA |= PWM_ENA_CHID6; 
}

// 0 is center position, and it goes clockwise
// we might also want to add scaliing to this.
void pwm_set_joystick_converted_angle(int joystick_angle){
	int converted_angle = 90 - joystick_angle; // subtract to make it clockwise, then it corresponds to joystic. //90 deg to center it
	pwm_set_angle(converted_angle);
}

void pwm_set_angle(int deg){

	int hightime_us = (int)( (deg/180.0*(2.1-0.9) + 0.9) * 1000  );
	printf("angle set to: %4d \n\r", deg);
	printf("hightime is %4d", hightime_us);

	//restrict it to allowed range
	if (hightime_us > 2100)
	{
		printf("hightime too large\n\r");
		hightime_us = 2100;
	}
	if (hightime_us< 900)
	{
		printf("hightime too small\n\r");
		hightime_us = 900;
	}
	
	int CDTY_value = (int)((84/1024.0)*hightime_us);
	
	printf("CDTY_value is %4d\n\r", CDTY_value);	

	PWM -> PWM_CH_NUM[6].PWM_CDTY = CDTY_value;
		
}

void pwm_test_a_few_angles(){
    printf("setting to 90 deg\n\r");
	pwm_set_angle(90);
	
	printf("setting to 27 deg\n\r");
	pwm_set_angle(27);
	
	printf("setting to -27 deg\n\r");
	pwm_set_angle(-27);
	
	printf("setting to 180 deg\n\r");
	pwm_set_angle(180);

	printf("setting to 0 deg\n\r");
	pwm_set_angle(0);
	printf("setting to 90 deg\n\r");
	pwm_set_angle(90);
}