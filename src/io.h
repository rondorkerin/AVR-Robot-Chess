/*
 * io.h
 *
 * Created: 8/11/2012 6:21:41 PM
 *  Author: nul4x3r0000
 */ 


#ifndef IO_H_
#define IO_H_

#define IO_ERROR -1

// NOTE::::::::::::::::: These must be changed to 9-12 once we get the actual human interface
//					     For now they overlap with the other buttons d-g
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
	Clr = 7
};

enum Side {
	Player1 = 0,
	Player2
};

// public functions
void io_init();
char* get_player_move(int side);
void play_sound();
void lcd_message(char* text);
int read_hall_effect_grid();
void activate_led(int position);
void move_piece(int position);
void debug(char *message);

// private functions
int get_button_press(int side);

#endif /* IO_H_ */