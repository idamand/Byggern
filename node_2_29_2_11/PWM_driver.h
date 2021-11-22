/*
 * PWM_driver.h
 *
 * Created: 01.11.2021 14:52:59
 *  Author: raffaelv
 */ 


#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_


void pwm_init();
void pwm_set_angle(int deg);
void pwm_test_a_few_angles();
void pwm_set_joystick_converted_angle(int joystick_angle);

#endif /* PWM_DRIVER_H_ */