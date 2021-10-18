/*
 * spi_driver.h
 *
 * Created: 12.10.2021 17:07:56
 *  Author: raffaelv
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_
#define  SPI_SLAVE_SELECT_PIN PB4

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
char SPI_MasterRead();

// we do not need any functions for slave select and deselect, that is handled automatically

#endif /* SPI_DRIVER_H_ */