/*
 * adc_sam.h
 *
 * Created: 02.11.2021 15:24:45
 *  Author: raffaelv
 */ 


#ifndef ADC_SAM_H_
#define ADC_SAM_H_


void adc_init();
int adc_get_val();
void adc_print_whether_ball_detected(int threshold, int print_adc_val);


#endif /* ADC_SAM_H_ */