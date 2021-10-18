/*
 * mcp2515_driver.c
 *
 * Created: 12.10.2021 18:43:03
 *  Author: raffaelv
 */ 

#include "mcp2515_driver.h"

// the spi interface is described at page 66 of the mcp datasheet

// writes data into adress of mcp 
void mcp2515_write(uint8_t adress, uint8_t data ){

	//set slave select low
	PORTB &= ~(1<<SPI_SLAVE_SELECT_PIN);

	// write command is ...
	SPI_MasterTransmit(MCP_WRITE_INSTRUCTION); // p66 datasheet, write-command
	SPI_MasterTransmit(adress);	
	SPI_MasterTransmit(data);

	// set slave select high.
	PORTB |= (1<<SPI_SLAVE_SELECT_PIN);

}

uint8_t mcp2515_read(uint8_t adress){
	//set slave select low
	PORTB &= ~(1<<SPI_SLAVE_SELECT_PIN);

	SPI_MasterTransmit(MCP_READ_INSTRUCTION); // read-command
	SPI_MasterTransmit(adress);	
	uint8_t return_value = SPI_MasterRead();

	// set slave select high.
	PORTB |= (1<<SPI_SLAVE_SELECT_PIN);
	return return_value;
}

void mcp2515_reset(){
	SPI_MasterTransmit(MCP_RESET_INSTRUCTION); // transmit reset instruction 
}

void mcp2515_load_tx_buffer(uint8_t data){
	uint8_t buf_num = 0x00; //can be a number between 0b0000 and 0b0111 see datasheet p.68
	SPI_MasterTransmit(MCP_LOAD_TX_INSTRUCTION|buf_num); //tx load buffer
	SPI_MasterTransmit(data); 
}

uint8_t mcp2515_read_rx_buffer(){
	uint8_t buf_num = 0x00;
	SPI_MasterTransmit(MCP_READ_RX_INSTRUCTION|(buf_num<<1)); // Bitshift because format for instruction is 0b10010nm0
	return SPI_MasterRead();
}

void mcp2515_set_loopback_mode(){
	 //can_ctrl_reg 0xfh, REQP (bit 7-5 of can ctrl reg, 5 is lsb)
	uint8_t can_ctrl_reg_addr = 0x0f;
	
	printf("in setting loopback\r\n");

	uint8_t can_ctrl_content = 8; //arbitrary start value
	can_ctrl_content =  mcp2515_read(can_ctrl_reg_addr);
	printf("the initial loopback content of canctrl is %4d: \r\n", can_ctrl_content);
	

	// loopback mode (p.60 mcp datasheet) is reqp = 010 (that is we set the uppermost three bits to 101
	uint8_t loopback_command = can_ctrl_content | (1<<6);
	loopback_command &= 0b01011111;
	printf("the new loopback to be written to canctrl is %4d: \r\n", loopback_command);

	mcp2515_write(can_ctrl_reg_addr, loopback_command);
	printf("canctrl is written to %4d: \r\n", mcp2515_read(can_ctrl_reg_addr));
	

	// read canstat.opmode
	uint8_t canstat_reg_addres = 0x0e; 
	// opmode is bit 7-5 
	uint8_t canstat =  mcp2515_read(canstat_reg_addres);
	uint8_t opmode = canstat >> 5; // the bitshift is in order to only see the uppermost three bits
	printf("canstat when setting loopback mode is %4d: \r\n", canstat);

	if(opmode == 2){ // 3 corresponds to looback command, 0b010
		printf("sucessfully set loopback mode\r\n");
	}
	else{
		printf("setting loopback mode failed\r\n");
	}
}