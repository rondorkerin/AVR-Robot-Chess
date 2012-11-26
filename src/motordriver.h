/**
 * \file motordriver.h
 *
 * \brief Declaration of motor driving routines 
 * \note Includes servos, stepper motors, and various sensors
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

// calibration numbers for stepper motors steps/square and 
// z stepper steps between reaching bottom and top
#define STEPS_PER_SQUARE 241
#define Y_STEPS_PER_SQUARE 124
#define Z_STEPPER_STEPS 2700
#define Y_CALIBRATION_OFFSET 17
#define X_CALIBRATION_OFFSET 0

#define GRAVEYARD_XPOS 1
//#define GRAVEYARD_YPOS 3

#define NUM_SQUARES_X 14
#define NUM_SQUARES_Y 8

enum Direction {
	CW = 0,
	CCW = 1,
	Up = 0,
	Down = 1
};

#include <conf.h>

// public functions
void init_motordriver();

// private functions
void open_clamp();
void close_clamp();
Bool move_x_stepper(uint8_t squares, uint8_t direction);
Bool move_y_stepper(uint8_t squares, uint8_t direction);
Bool move_z_stepper(uint8_t direction);
Bool move_crane(uint8_t to_posx, uint8_t to_posy);
void center_at_origin();

int posx; ///< current location of the crane
int posy;

#endif /* MOTORDRIVER_H_ */