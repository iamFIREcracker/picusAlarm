#include "HWlib.h"


/****************************************************************************
  SECTION 	ISR (Interrupt Service routines)
****************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
	UARTRxInt(1);
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
#if UART_PORTS >= 2
	UARTRxInt(2);
#endif
}

void __attribute__((interrupt, no_auto_psv)) _U3RXInterrupt(void)
{
#if UART_PORTS >= 3
	UARTRxInt(3);
#endif
}

void __attribute__((interrupt, no_auto_psv)) _U4RXInterrupt(void)
{
#if UART_PORTS == 4
	UARTRxInt(4);
#endif
}


void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void)
{
  UARTWrite(1,"!!! Default interrupt handler !!!\r\n" );
  while (1)
  {
	  Nop();
	  Nop();
	  Nop();
  }
}

void __attribute__((interrupt, auto_psv)) _OscillatorFail(void)
{
  UARTWrite(1, "!!! Oscillator Fail interrupt handler !!!\r\n" );
  while (1)
  {
	  Nop();
	  Nop();
	  Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _AddressError(void)
{
  UARTWrite(1,"!!! Address Error interrupt handler !!!\r\n" );
  while (1)
  {
	  Nop();
	  Nop();
	  Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _StackError(void)
{
  UARTWrite(1,"!!! Stack Error interrupt handler !!!\r\n" );
  while (1)
  {
	  Nop();
	  Nop();
	  Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _MathError(void)
{
  UARTWrite(1,"!!! Math Error interrupt handler !!!\r\n" );
  while (1)
  {
	  Nop(); 
	  Nop();
	  Nop();
  }
}
