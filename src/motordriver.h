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
#define STEPS_PER_SQUARE 3
#define Z_STEPPER_STEPS 30

#include <conf.h>

// public functions
void init_motordriver();

// private functions
void open_clamp();
void close_clamp();
void move_x_stepper(uint8_t steps, uint8_t direction);
void move_y_stepper(uint8_t steps, uint8_t direction);
void move_z_stepper();
void center_at_origin();

int current_pos; ///< current location of the crane


#endif /* MOTORDRIVER_H_ */