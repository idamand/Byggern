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

void pwm_set_angle(int deg){

	int hightime_us = (int)( (deg/180.0*(2.1-0.9) + 0.9) * 1000  );
	
	printf("hightime is %4d", hightime_us);

	//restrict it to allowed range
	if (hightime_us > 2100)
	{
		printf("hightime too large");
		hightime_us = 2100;
	}
	if (hightime_us< 900)
	{
		printf("hightime too small");
		hightime_us = 900;
	}
	
	int CDTY_value = (int)((84/1024.0)*hightime_us);
	
	printf("CDTY_value is %4d", CDTY_value);	

	PWM -> PWM_CH_NUM[6].PWM_CDTY = CDTY_value;
		
}

void pwm_test_a_few_angles(){
    printf("setting to 90 deg");
	pwm_set_angle(90);

	printf("setting to 180 deg");
	pwm_set_angle(180);

	printf("setting to 0 deg");
	pwm_set_angle(0);
}