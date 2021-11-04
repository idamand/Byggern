/*
 * CAN.c
 *
 * Created: 12.10.2021 11:49:05
 *  Author: pieterjs
 */ 
#include "CAN.h"
#include "mcp2515_driver.h"
#include <util/delay.h>
	
void CAN_init(void){
	
	mcp_init();
	
	/* currently moved to mcp_init
	mcp_bit_modify(MCP_TXB0CTRL, 0b00000011, 0b00000000); //Set T0 to lowest priority
	mcp_bit_modify(MCP_TXB1CTRL, 0b00000011, 0b00000010); //Set T1 to intermediate priority
	mcp_bit_modify(MCP_TXB2CTRL, 0b00000011, 0b00000011); //Set T2 to highest priority
	mcp_bit_modify(MCP_CANINTE,  0b00011100, 0b00011111); //Set the interrupt for all buffers
	*/
	mcp_set_normal_mode();
}

void CAN_send(struct CANmessage message, uint8_t buffer){ //Buffer type is 0bxxxxxnnn with 100 T2, 010 T1 and 001 T0
	//printf("CAN_send called for: %4d, %4d, %4d ", message.data[0], message.data[1], message.data[2]);
	
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
    
    //we had a tendency to loose bus arbitration, to avoid this we set priority of message to zero  
    mcp_bit_modify(MCP_TXB0CTRL, 0xFF, 0x00); 
    mcp_bit_modify(MCP_TXB1CTRL, 0xFF, 0x01); 
    mcp_bit_modify(MCP_TXB2CTRL, 0xFF, 0x02); 
    
	mcp_bit_modify(start_adr-5, 0b11111111, message.IDH);//Set SIDH register
	mcp_bit_modify(start_adr-4, 0b11101011, message.IDL << 5);//Set SIDL register
	mcp_bit_modify(start_adr-1, 0b01001111, message.length);//Set data length in DLC register
	
	for(uint8_t i=0; i < message.length; i++){
		int adr = start_adr+i;
		mcp_bit_modify(adr, 0b11111111, message.data[i]);
	}
	
	mcp_bit_modify(MCP_TXB0CTRL, 0b00001000, 0b00001000); // set txreq bit high (see flowchart page 17 mcp2525 for why this is necessary) 
	//printf("txb0ctrl before rts is %4d\r\n", mcp_read(MCP_TXB0CTRL));
	
    printf("MLOA before RTS: %4d\r\n", mcp_read( MCP_TXB0CTRL) & (1<<5)  ); //should be 0, because this is cleared by setting txreq bit.
    
	mcp_request_to_send(buffer); //Request to send to selected buffer
	printf("can send waiting for interrupt\n");
    
	//Add interrupt when message transmitted
	uint8_t interrupt = mcp_read(MCP_CANINTF);
	printf("canintf is %4d\r\n", interrupt);
	printf("caninte is %4d\r\n", mcp_read(MCP_CANINTE));
	
	if( mcp_read(MCP_TXB0CTRL) == 0){
		printf("txb0ctrl.txreq was cleared after can transmission, sucessful can_send.\r\n");
	}
	else{
		printf("txb0ctrl.txreq was cleared after can transmission, UNsucessful can_send.\r\n");
        if (mcp_read(MCP_CANINTF)&MCP_MERRF)
        {
            printf("message started to transmit, but encountered error\r\n");
        }
        if(mcp_read( MCP_TXB0CTRL) & (1<<5) ){ // bit 5 is MCP_MLOA
          printf( "CAN lost arbitration of bus\r\n");
        }            
        
	}
    
    printf("MLOA after RTS: %4d\r\n", mcp_read(MCP_TXB0CTRL)& (1<<5)  );
	
	//manually clear canintf to reset interrupt condition
	mcp_write(MCP_CANINTF, 0x00); //NB: TODO: might be a good idea to remove if we want interrupts on this node
	
	/*
	while(!( (interrupt  & int_mask) == int_mask))
	;
	mcp_bit_modify(MCP_CANINTF, int_mask, 0b00000000);
	*/
	
	//printf("can send skipped interrupt\n");
}

struct CANmessage CAN_receive(uint8_t buffer){ //Receive buffer of type 0bxxxxxxnn with nn=01 for R0 and nn=10 for R1
	struct CANmessage received;
    /*
	printf("can receive waiting for interrupt\n");
	_delay_ms(1000);
	
	printf("skipping waiting for interrupt (tempporary)\r\n");
    */
    
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
		//printf("%u\r\n", received.data[i]);
	}
	
	return received;
}

// enters loopback mode, does a selftest, and then goes back to normal mode
// returns 1 on sucess and zero on error
int can_loopback_selftest(){
    mcp_set_loopback_mode();
    
    // create a can message to be used for testing
	struct CANmessage can_test_msg; 
	can_test_msg.IDH = 0b1111111;
	can_test_msg.IDL = 0b00000111; // 3 lowest bits are part of 11 bit ID
	can_test_msg.length = 0x06;
	can_test_msg.data[0] = 0x09; can_test_msg.data[1] = 0x08; can_test_msg.data[2] = 0x07;
	
    
    CAN_send(can_test_msg, 0x01); // to send to t1
	struct CANmessage can_test_recieve = CAN_receive(0x01); // to recieve from to r0
	//printf("can test recieve r0 is: %4d, %4d, %4d\r\n", can_test_recieve.data[0], can_test_recieve.data[1], can_test_recieve.data[2]);
    
    if ( (can_test_recieve.data[0] == can_test_msg.data[0]) && (can_test_recieve.data[1] == can_test_msg.data[1]) &&  (can_test_recieve.data[1] == can_test_msg.data[1]) )
    {
       printf( "CAN SELFTEST (loopback) SUCCESSFUL\r\n");
    }
    else{
        printf("CAN SELFTEST FAILURE\r\n");
    }
    
    mcp_set_normal_mode();
    
    return 0;
}

/*
void can_interrupt_init(){
    mcp_bit_modify(MCP_CANINTE, 0xff, 0xff);
    write_bit
}
*/