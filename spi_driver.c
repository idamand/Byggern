/*
 * spi_driver.c
 *
 * Created: 12.10.2021 17:07:28
 *  Author: raffaelv
 */ 
#include <avr/io.h>

void SPI_MasterInit(void) {
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DDB5)|(1<<DDB7) | (1<<DDB4) ; // corresponds to DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK); in example program
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_MasterTransmit(char cData) {
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */ while(!(SPSR & (1<<SPIF)));
}