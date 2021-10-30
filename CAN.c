/*
 * CAN.c
 *
 * Created: 12.10.2021 11:49:05
 *  Author: pieterjs
 */ 
#include "CAN.h"
#include "mcp2515_driver.h"
	
void CAN_init(void){
	
	mcp_init();
	mcp_bit_modify(MCP_TXB0CTRL, 0b00000011, 0b00000000); //Set T0 to lowest priority
	mcp_bit_modify(MCP_TXB1CTRL, 0b00000011, 0b00000010); //Set T1 to intermediate priority
	mcp_bit_modify(MCP_TXB2CTRL, 0b00000011, 0b00000011); //Set T2 to highest priority
	mcp_bit_modify(MCP_CANINTE,  0b00011100, 0b00011111); //Set the interrupt for all buffers
	
	mcp_set_loopback_mode();

}

void CAN_send(struct CANmessage message, uint8_t buffer){ //Buffer type is 0bxxxxxnnn with 100 T2, 010 T1 and 001 T0
	
	uint8_t start_adr;
	uint8_t int_mask;
	if( (0b00000111 & buffer) == 0b00000001){ //Check if T0 was selected
		start_adr = 0x36;	
		int_mask = 0b00000100;
	}
	else if( (0b00000111 & buffer) == 0b00000010){ //Check if T1 was selected
		start_adr = 0x46;
		int_mask = 0b00001000;
	}
	else if((0b00000111 & buffer) == 0b00000100){ //Check if T2 was selected
		start_adr = 0x56;	
		int_mask = 0b00010000;
	}
	else{
		printf("Buffer not properly selected");
	}
	mcp_bit_modify(start_adr-5, 0b11111111, message.IDH);//Set SIDH register
	mcp_bit_modify(start_adr-4, 0b11101011, message.IDL << 5);//Set SIDL register
	mcp_bit_modify(start_adr-1, 0b01001111, message.length);//Set data length in DLC register
	
	for(uint8_t i=0; i < message.length; i++){
		int adr = start_adr+i;
		mcp_bit_modify(adr, 0b11111111, message.data[i]);
	}
	
	//mcp_bit_modify(MCP_TXB0CTRL, 0b00001000, 0b00001000);
	//printf("txb0ctrl before rts is %4d\r\n", mcp_read(MCP_TXB0CTRL));
	
	mcp_request_to_send(buffer); //Request to send to selected buffer
	printf("can send waiting for interrupt\n");
	//Add interrupt when message transmitted
	uint8_t interrupt = mcp_read(MCP_CANINTF);
	printf("canintf is %4d\r\n", interrupt);
	printf("caninte is %4d\r\n", mcp_read(MCP_CANINTE));
	printf("txb0ctrl after rts is %4d\r\n", mcp_read(MCP_TXB0CTRL));
	
	
	/*
	while(!( (interrupt  & int_mask) == int_mask))
	;
	mcp_bit_modify(MCP_CANINTF, int_mask, 0b00000000);
	*/
	printf("can send received interrupt\n");
}

struct CANmessage CAN_receive(uint8_t buffer){ //Receive buffer of type 0bxxxxxxnn with nn=01 for R0 and nn=10 for R1
	struct CANmessage received;
	printf("can receive waiting for interrupt\n");
	printf("skipping waiting for interrupt (tempporary)\r\n");
	/*
	uint8_t interrupt = mcp_read(MCP_CANINTF);
	while(!((interrupt & buffer) == buffer))
	;
	printf("can receive received interrupt\n");
	*/
	int start_adr;
	if(0b00000011 & buffer == 0b00000001){ //Check if R0 was selected
		start_adr = 102;
	}
	else if(0b00000011 & buffer == 0b00000010){ //Check if R1 was selected
		start_adr = 118;
	}
	else{
		printf("Buffer not properly selected");
	}
	
	received.IDH = mcp_read(start_adr-5);
	received.IDL = (mcp_read(start_adr-4)>>5) & 0b00000111;
	received.length = mcp_read(start_adr-1)& 0b00001111;
	
	for(int i=0; i < 8; i++){
		int adr = start_adr+i;
		received.data[i] = mcp_read(adr);
		printf("%u\r\n", received.data[i]);
	}
	
	return received;
}
	