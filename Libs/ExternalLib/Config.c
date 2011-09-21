#include "WFlib.h"

#include "libpic30.h"


#include "Sensor.h"

#include "Config.h"

extern Sensor sensors[NUM_SENSORS];


int loadConfig() {
	int in1 = 0;
	int *rmem = (int*) sensors;
	int rdoffset;
	int vRead,vRead1;
	long int addr1 = 0x2A002+1024;

	while (in1<sizeof (sensors))
	{
		TBLPAG = (((addr1+in1) & 0x7F0000)>>16);
		rdoffset = ((addr1+in1) & 0x00FFFF);
		asm("tblrdh.w [%1], %0" : "=r"(vRead1)     : "r"(rdoffset));
		asm("tblrdl.w [%1], %0" : "=r"(vRead)     : "r"(rdoffset));	

		*rmem = vRead;
		in1=in1+2;
		rmem = rmem + 1;
	}
	return 0;
}


int saveConfig() {
	_erase_flash(0x2A000+1024);
	int in1=0;
	int *wmem = (int*)sensors;
	unsigned int offset;

	NVMCON = 0x4003; // Initialize NVMCON
	TBLPAG = (0x2A000+1024)>>16; 						// Initialize PM Page Boundary SFR
	offset = (0x2A000+1024) & 0xFFFF; 				// Initialize lower word of address
	__builtin_tblwtl(offset, 0x0A0B);     	// Write to address low word
	
	asm("DISI #5"); 	
	__builtin_write_NVM(); 	

	while (in1<sizeof(sensors))
	{

		unsigned long progAddr = 0x2A002+1024+in1; 			// Address of word to program
		unsigned int progDataL = (unsigned int) *(wmem);			// Data to program lower word

		NVMCON = 0x4003; // Initialize NVMCON
		TBLPAG = progAddr>>16; 						// Initialize PM Page Boundary SFR
		offset = progAddr & 0xFFFF; 				// Initialize lower word of address
		__builtin_tblwtl(offset, progDataL);     	// Write to address low word
		
		asm("DISI #5"); 	
		__builtin_write_NVM(); 	

		in1=in1+2;
		wmem++;
	}
	return 0;
}
