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

static inline void write_board_led_mask(uint8_t mask)
{
	LED_IOPORT.OUT = ~mask;
}

int main (void)
{
	board_init();

	// Insert application code here, after the board has been initialized.	
	gpio_set_pin_low(LED0_GPIO);
	
	while (1){		
	}
	
}

