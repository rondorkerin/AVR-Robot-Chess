/*************************************************************************
 *
 *
 *              Task2: Generic Drivers
 *
 *
 *************************************************************************/

// The avr_compiler.h header file includes the MCU specific header 
// file configured in the project options and it also contains 
// compatibility definitions to be able to compile the same code 
// for both the GCC and the IAR compiler. 
#include "avr_compiler.h"

// The board.h header file defines which IO ports peripherals like
// Switches and LEDs are connected to. The header file is configured
// for use with XMEGA-A1 Xplained.
#include "board.h"

// This function sets all pins to output and uses the "out"
// parameter to set output levels. By having a pointer-to-a-struct
// as a parameter, this function can be re-used for any IO port without
// extra code. This method can also be used for other peripherals such
// as ADCs, DACs, and Timer/Counters. This is possible since all port
// modules have similar internal register layout (same goes for ADCs,
// DACs, Timer/Counters etc)

void SetLEDs( PORT_t * ledPort, unsigned char out )
{
	ledPort->DIR = 0xff; // Make sure all pins are outputs.
	ledPort->OUT = out; // Output requested value.
}


// Same method used on this function, but now for reading
// input values from an IO port.
unsigned char GetSwitches( PORT_t * switchPort, char mask )
{
	switchPort->DIR &= mask; // Make sure used pins are inputs.
	return switchPort->IN & mask; // Read value, mask out unused pins and return.
}


volatile unsigned char value;

int main( void )
{
	// Prepare pointers to the peripheral port modules we want to use for LEDs and switches.
	PORT_t * ledPort = &LEDPORT;
	PORT_t * switchPortL = &SWITCHPORTL;
	PORT_t * switchPortH = &SWITCHPORTH;

	// Enable pullups for switches. MPCMASK will be explained in task 4.
	PORTCFG.MPCMASK = SWITCHPORTL_MASK_gc;
	switchPortL->PIN0CTRL = PORT_OPC_PULLUP_gc;
	
	PORTCFG.MPCMASK = SWITCHPORTH_MASK_gc;
	switchPortH->PIN6CTRL = PORT_OPC_PULLUP_gc;

	// Now copy switch state to LEDs again and again and again...
	// Because the switches are split between two ports on the XMEGA-A1 Xplained board
	// the value statement combines bits from each port and arranges them in the right order.
	while(1) 
    {
		value = GetSwitches( switchPortL, SWITCHPORTL_MASK_gc ) | (GetSwitches( switchPortH, SWITCHPORTH_MASK_gc ) << SWITCHPORTH_OFFSET);
		SetLEDs( ledPort, value );
	}
}
