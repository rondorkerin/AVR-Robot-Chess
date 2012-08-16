/**
 * \file
 *
 * \brief Empty user application template
 *
 */
#define LED_IOPORT            PORTE
/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <gpio.h>
#include <stdio.h>
#include "chessengine.h"

static inline void write_board_led_mask(uint8_t mask)
{
	LED_IOPORT.OUT = ~mask;
}
/*
 * TEST MAIN: 
 * Issues a sequence of pawn movements and blinks an LED if a piece was taken. 
 * sequence: a2-a4, a4-a5, a5-a6
 *
 */ 

int main (void)
{
	board_init();
6
	// Insert application code here, after the board has been initialized.	
	gpio_set_pin_low(LED0_GPIO);
	
	while (1){		
		InitEngine();
		InitGame(PlayerVsAI);

		int gameResult = 0, tookPieceFlag = 0,illegalMoveFlag = 0;
		char resultMove[4];

		for (;;) {
			char cmd[100] = "a2a4";
			PlayerMove(cmd, &gameResult, &tookPieceFlag, &illegalMoveFlag);
			if (illegalMoveFlag)
			{
				//printf("illegal move.\n");
				continue;
			}
			AIMove(&gameResult, &tookPieceFlag, resultMove);
		}
		
	}
	
	
}

