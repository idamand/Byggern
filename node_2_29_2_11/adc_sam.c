/*
 * adc_sam.c
 *
 * Created: 02.11.2021 15:23:58
 *  Author: raffaelv
 */ 

#include "sam.h"
#include "adc_sam.h" //not our files, contains register definitions
#include "printf-stdarg.h"

void adc_init(){
	// enable adc in power manager. 
	PMC -> PMC_PCER1 = PMC_PCER1_PID37; 
	
	ADC->ADC_MR |= ADC_MR_FREERUN_ON | ADC_MR_SETTLING_AST17; //enble freerun mode, recommended in lab lectures
	
	//channel 7 corresponds to A0 //despite what the table in datashit says.
	ADC->ADC_CHER |= ADC_CHER_CH7; 
		// control_register bit 0 is to software reset, bit 1 is to initialize a sampling. 
	
}

int adc_get_val(){
	
	ADC->ADC_CR |= ADC_CR_START; 
	// after initializing sampling, read out the sample
	int sample_value = 3; //random arbitrary number
	sample_value = ADC->ADC_CDR[7];  
	//printf("sample value is %4d\n\r", sample_value);
	return sample_value; 
}

//need to be global
int ball_count =0; // amount of times ball is passed the ir-sensor.
int ball_in_front_of_ir_flag = 0;

//must bee looped from outside of the function (that is, this function only does one single check.
//threshold should be ish 1500
void adc_print_whether_ball_detected(int threshold, int print_adc_val ){
	
	int adc_val = adc_get_val();
    if((adc_val < threshold) && ball_in_front_of_ir_flag == 0){ //1550 empiracally obtained // if < 1550 -> ball is blocking ir
		printf("BALL DETECTED!"); 
		ball_count += 1; 
		ball_in_front_of_ir_flag = 1; 
		printf(": %4d",ball_count);	
	}
	else{
		if (adc_val > threshold)
		{
			ball_in_front_of_ir_flag = 0;	
		}
	}
    if (print_adc_val)
    {
	    printf("val %4d: \n\r", adc_get_val());

    }
    
	
}
