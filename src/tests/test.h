/*
 * \file test.h
 *
 * \brief Contains prototypes for test functionality
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#ifndef TEST_H_
#define TEST_H_

#include <util/delay.h>

static int debug_initialized = 0;

void init_debug();
void debug(char *message);

#endif /* TEST_H_ */