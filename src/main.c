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

#ifdef __GNUC__
# include <util/delay.h>
#else
static inline void _delay_ms(uint16_t t)
{
	uint16_t i;

	for (i = 0; i < t; ++i) {
		__delay_cycles((UINT32_C)(sysclk_get_cpu_hz() / 6000));
	}
}
#endif

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

	// Insert application code here, after the board has been initialized.	
	gpio_set_pin_low(LED0_GPIO);
	
	while (1){		
		
		InitEngine();
		InitGame(PlayerVsAI);

		int gameResult = 0, tookPieceFlag = 0,illegalMoveFlag = 0;
		char resultMove[4];

		write_board_led_mask(0xDD);
		_delay_ms(4000);
		write_board_led_mask(0x00);
		
		char cmd[3][100] = {"a2a4", "a4a5", "a5a6"};
		int i;
		for (i = 0; i < 5; i++)
		{
			PlayerMove(cmd[i], &gameResult, &tookPieceFlag, &illegalMoveFlag);
			if (illegalMoveFlag)
			{
				//printf("illegal move.\n");
				continue;
			}
			AIMove(&gameResult, &tookPieceFlag, resultMove);
					
			if (!tookPieceFlag)
			{
				write_board_led_mask(0x0F);
				_delay_ms(2000);
				write_board_led_mask(0x00);
				_delay_ms(2000);
			}
			else
			{
				for (;;) {
					write_board_led_mask(0xF0);
					_delay_ms(200);
					write_board_led_mask(0x00);
					_delay_ms(200);
							
				}
						
			}
		}


		
		
	}
	
	
}

