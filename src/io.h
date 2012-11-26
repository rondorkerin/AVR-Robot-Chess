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
	Clr,
	Go,
	To,
	From,
	ButtonError
};

enum LED {
	Red = 0,
	Green = 1,
	Blue = 2,
	All = 4
	};

enum Side {
	Player1 = 0,
	Player2
};

// map of current LEDs on
unsigned char red_led_map[9];
unsigned char green_led_map[9];
unsigned char blue_led_map[9];

#define LED_REG_NOOP        0x00
#define LED_REG_DIGIT0      0x01
#define LED_REG_DIGIT1      0x02
#define LED_REG_DIGIT2      0x03
#define LED_REG_DIGIT3      0x04
#define LED_REG_DIGIT4      0x05
#define LED_REG_DIGIT5      0x06
#define LED_REG_DIGIT6      0x07
#define LED_REG_DIGIT7      0x08
#define LED_REG_DECODEMODE  0x09
#define LED_REG_INTENSITY   0x0a
#define LED_REG_SCANLIMIT   0x0b
#define LED_REG_SHUTDOWN    0x0c
#define LED_REG_DISPLAYTEST 0x0f

// public functions
// perform high level actions that need to be exposed to other modules
void init_io();
char* get_player_move(uint8_t side);
void play_sound();
void lcd_message(unsigned char* msg, int side);
void lcd_message_delay(unsigned char* msg, int side, int delay);
uint8_t read_hall_effect_grid();
void led_set(uint8_t posx, uint8_t posy, uint8_t rgb);
void led_clear(uint8_t posx, uint8_t posy, uint8_t rgb);
void move_piece(uint8_t from_posx, uint8_t from_posy, uint8_t to_posx, uint8_t to_posy);
void refresh_led_grid();
void blink_led(uint8_t posx, uint8_t posy, uint8_t rgb);

// private functions (mostly correspond to simple electrical components)
uint8_t get_button_press(uint8_t side);
void lcd2_putchar(unsigned char data, int side);
void lcd2_putstring(unsigned char* pcString, int side);
void led_putchar(unsigned char data);
void led_send(unsigned char reg, unsigned char col, uint8_t rgb);
void delay_ms(int count);
uint8_t* get_positions(char* move);

#endif /* IO_H_ */