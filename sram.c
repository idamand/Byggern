/*
 * sram.c
 *
 * Created: 14.09.2021 10:53:51
 *  Author: idamand
 */ 
#include <avr/io.h>
#include <stdbool.h>

void SRAM_test()//bool short_write)
{
	//uint16_t ext_ram_size = 0x800; //ram size for sram 
	uint16_t ext_ram_size = 0x100; //JUST TO HAVE LESS OUTPUT TO THE SCREEN, TODO: CHANGE BACK
	
	volatile char *ext_ram = (char *)0x1800; // Start address for the SRAM 
	
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	
	
	printf("Starting SRAM test...\n");
	
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	
	//just to have posssibility of less output during a ram test we expect to crash
	uint16_t loop_len = ext_ram_size;
	//if(short_write){
		//loop_len = 500;
	//}
	
	for (uint16_t i = 0; i < loop_len; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			write_errors++;
		}
	}
	


	
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n\r", write_errors, retrieval_errors);
}

