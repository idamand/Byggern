/*
 * mcp2515_driver.h
 *
 * Created: 12.10.2021 18:42:43
 *  Author: raffaelv
 */ 


#ifndef MCP2515_DRIVER_H_
#define MCP2515_DRIVER_H_

#include "spi_driver.h"
#include <stdio.h>
#include <avr/io.h>

uint8_t mcp2515_read(uint8_t adress); // to read a register
void mcp2515_write(uint8_t adress, uint8_t data); // to write to a register
void mcp2515_reset();
void mcp2515_set_loopback_mode();
void mcp2515_load_tx_buffer(uint8_t data); // todo: can be extended so that we can choose which tx buffer to load 
uint8_t mcp2515_read_rx_buffer();

// p66 of datasheet
#define MCP_RESET_INSTRUCTION 0x0a
#define MCP_READ_INSTRUCTION 0x03
#define MCP_WRITE_INSTRUCTION 0x02
#define MCP_LOAD_TX_INSTRUCTION 0x40
#define MCP_READ_RX_INSTRUCTION 0x90

#endif /* MCP2515_DRIVER_H_ */