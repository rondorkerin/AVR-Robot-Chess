/*
 * \file iotest.h
 *
 * \brief Contains prototypes for I/O tests
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#ifndef IOTEST_H_
#define IOTEST_H_

#include <conf.h>

void io_test_init();
void clamp_servo_test();
void button_press_test(uint8_t side);
void x_stepper_test();


#endif /* IOTEST_H_ */