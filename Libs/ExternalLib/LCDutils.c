#include "LCDutils.h"
#include "HWlib.h"

void ClearScreen(void){
	char mess[] = {0x7C, 0x00, 0x00};
	UARTWrite(2, mess);
}

void Demo(void) {
	char mess[] = {0x7C, 0x04, 0x00};
	UARTWrite(2, mess);
}

void Reverse(void) {
	char mess[] = {0x7C, 0x12, 0x00};
	UARTWrite(2, mess);
}

void LCDWrite(char mess[]) {
	UARTWrite(2, mess);
}

