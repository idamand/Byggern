/*
 * spi_driver.h
 *
 * Created: 12.10.2021 17:07:56
 *  Author: raffaelv
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_


void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

#endif /* SPI_DRIVER_H_ */