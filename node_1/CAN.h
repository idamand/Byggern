/*
 * CAN.h
 *
 * Created: 12.10.2021 11:49:20
 *  Author: pieterjs
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct CANmessage {uint8_t IDH; uint8_t IDL; uint8_t length; uint8_t data[8];} ;

#ifndef CAN_H_
#define CAN_H_

void CAN_init(void);
void CAN_send(struct CANmessage message, uint8_t buffer);
void CAN_test(void);
struct CANmessage CAN_receive(uint8_t buffer);
int can_loopback_selftest();


#endif /* CAN_H_ */