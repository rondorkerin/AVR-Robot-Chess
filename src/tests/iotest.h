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
void xy_stepper_test();
void x_stepper_test();
void y_stepper_test();
void z_stepper_test();
void lcd_test();
void led_test();

#endif /* IOTEST_H_ */