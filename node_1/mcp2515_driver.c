/*
 * mcp2515_driver.c
 *
 * Created: 12.10.2021 18:43:03
 *  Author: raffaelv
 */ 

#include "mcp2515_driver.h"

// the spi interface is described at page 66 of the mcp datasheet

void mcp_init(){
	SPI_MasterInit();
	
	// set T0 to T2 with increasing priority, t0 =000, t1 = 010, t2=011	
	mcp_bit_modify(MCP_TXB0CTRL, 0b00000011, 0x00); //t0
	mcp_bit_modify(MCP_TXB1CTRL, 0b00000011, 0b00000010); //t1
	mcp_bit_modify(MCP_TXB2CTRL, 0b00000011, 0b00000011); // t2
	
	//set interrupt for all buffers
	mcp_bit_modify(MCP_CANINTE, 0b11100000, 0b01000000);
		
}

void mcp_slave_select(){
	//set slave select low
	PORTB &= ~(1<<SPI_SLAVE_SELECT_PIN);
}

void mcp_slave_deselect(){
	// set slave select high.
	PORTB |= (1<<SPI_SLAVE_SELECT_PIN);

}
// writes data into adress of mcp 
void mcp_write(uint8_t adress, uint8_t data ){

	mcp_slave_select();

	// write command is ...
	SPI_MasterTransmit(MCP_WRITE); // p66 datasheet, write-command
	SPI_MasterTransmit(adress);	
	SPI_MasterTransmit(data);
	
	mcp_slave_deselect();
}

uint8_t mcp_read(uint8_t adress){
	
	mcp_slave_select();

	SPI_MasterTransmit(MCP_READ); // read-command
	SPI_MasterTransmit(adress);	
	uint8_t return_value = SPI_MasterRead();

	mcp_slave_deselect();
	
	return return_value;
}

void mcp_reset(){
	mcp_slave_select();
	SPI_MasterTransmit(MCP_RESET); // transmit reset instruction 
	mcp_slave_deselect();
}

//Buffer type is 0bxxxxxnnn with 100 T2, 010 T1 and 001 T0
void mcp_request_to_send(uint8_t buffer){ 
	mcp_slave_select();
	
	SPI_MasterTransmit(0b1000000|buffer);
	
	mcp_slave_deselect();
}

void mcp_load_tx_buffer(uint8_t data){
	uint8_t buf_num = 0x00; //can be a number between 0b0000 and 0b0111 see datasheet p.68
	mcp_slave_select();
	SPI_MasterTransmit(MCP_LOAD_TX0|buf_num); //tx load buffer
	SPI_MasterTransmit(data); 
	mcp_slave_deselect();
}

uint8_t mcp_read_rx_buffer(){
	mcp_slave_select();
	uint8_t buf_num = 0x00;
	SPI_MasterTransmit(MCP_READ_RX0|(buf_num<<1)); // Bitshift because format for instruction is 0b10010nm0
	return SPI_MasterRead();
	mcp_slave_deselect();
}

void mcp_bit_modify(uint8_t adress, uint8_t mask, uint8_t data){
	mcp_slave_select();
	SPI_MasterTransmit(MCP_BITMOD); 
	SPI_MasterTransmit(adress);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
	mcp_slave_deselect();
}

void mcp_set_loopback_mode(){
	 //can_ctrl_reg 0xfh, REQP (bit 7-5 of can ctrl reg, 5 is lsb)
	//uint8_t can_ctrl_reg_addr = 0x0f;
	
	printf("in setting loopback\r\n");
	
	mcp_bit_modify(MCP_CANCTRL, 0b11100000, 0b01000000); //Set loopback mode
	
	// read canstat.opmode
	// opmode is bit 7-5 
	uint8_t canstat =  mcp_read(MCP_CANSTAT);
	uint8_t opmode = canstat >> 5; // the bitshift is in order to only see the uppermost three bits

	if(opmode == 2){ // 3 corresponds to looback command, 0b010
		printf("sucessfully set loopback mode\r\n");
	}
	else{
		printf("setting loopback mode failed\r\n");
	}
} 

void mcp_set_normal_mode(){
	 //can_ctrl_reg 0xfh, REQP (bit 7-5 of can ctrl reg, 5 is lsb)
	//uint8_t can_ctrl_reg_addr = 0x0f;
	
	printf("in setting normal mode\r\n");
	
	mcp_bit_modify(MCP_CANCTRL, 0b11100000, 0b00000000); //Set normal mode (0 is normal mode)
	
	// read canstat.opmode
	// opmode is bit 7-5 
	uint8_t canstat =  mcp_read(MCP_CANSTAT);
	uint8_t opmode = canstat >> 5; // the bitshift is in order to only see the uppermost three bits

	if(opmode == 0){ // 0 corresponds to normal mode
		printf("sucessfully set CAN normal mode\r\n");
	}
	else{
		printf("setting CAN normal mode failed\r\n");
	}
} 
