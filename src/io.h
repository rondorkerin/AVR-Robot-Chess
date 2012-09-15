/**
 * \file io.h
 *
 * \brief Declarations related to peripheral control
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#ifndef IO_H_
#define IO_H_

#include <conf.h>
#include <util/delay.h>


enum Button {
	A1 = 0,
	B2,
	C3,
	D4,
	E5,
	F6,
	G7,
	H8,
	From = 4,
	To = 5,
	Go = 6,
	Clr = 7,
	ButtonError = 12
};

enum Side {
	Player1 = 0,
	Player2
};

// public functions
// perform high level actions that need to be exposed to other modules
void init_io();
char* get_player_move(uint8_t side);
void play_sound();
void lcd_message(unsigned char* msg, int side);
void lcd_message_delay(unsigned char* msg, int side, int delay);
uint8_t read_hall_effect_grid();
void activate_led(uint8_t posx, uint8_t posy);
void move_piece(uint8_t from_posx, uint8_t from_posy, uint8_t to_posx, uint8_t to_posy);

// private functions (mostly correspond to simple electrical components)
uint8_t get_button_press(uint8_t side);
void lcd_putchar(unsigned char TXdata, int side);
void lcd_putstring(unsigned char* pcString, int side);
void delay_ms(int count);

#endif /* IO_H_ */