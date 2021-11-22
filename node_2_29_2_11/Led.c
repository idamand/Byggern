/*
 * Led.c
 *
 * Created: 03.11.2021 11:36:37
 *  Author: idamand
 */ 


#include "sam.h"

void Led_init(){
    
    PIOA ->PIO_PER = PIO_PA19; // enable pin as output
    PIOA -> PIO_OER = PIO_PA19; // set as output
    PIOA -> PIO_PUDR = PIO_PA19; // pull up disable
}

void Led_blink_once_quickly(){
    PIOA -> PIO_SODR = PIO_PA19; // turn led high
    PIOA -> PIO_CODR = PIO_PA19; // turn led low
}