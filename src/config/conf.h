
/**
 * \file
 *
 * \brief Board-specific example configuration
 *
 * Copyright (c) 2010 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _CONF_EXAMPLE_H
#define _CONF_EXAMPLE_H

#include <asf.h>
#define F_CPU 32000000UL // if we don't define the frequency of the CPU delay.h assumes it's 1 Mhz

//! Motor Controller pins
#define SERVO_PWM_PIN	       IOPORT_CREATE_PIN(PORTC, 0)
#define X_STEPPER_EN_PIN	   IOPORT_CREATE_PIN(PORTC, 5)
#define X_STEPPER_CLK_PIN	   IOPORT_CREATE_PIN(PORTC, 6)
#define X_STEPPER_CW_PIN	   IOPORT_CREATE_PIN(PORTC, 7)
#define Y_STEPPER_EN_PIN	   IOPORT_CREATE_PIN(PORTA, 5)
#define Y_STEPPER_CLK_PIN	   IOPORT_CREATE_PIN(PORTA, 6)
#define Y_STEPPER_CW_PIN	   IOPORT_CREATE_PIN(PORTA, 7)
#define Z_STEPPER_EN_PIN	   IOPORT_CREATE_PIN(PORTC, 1)
#define Z_STEPPER_CLK_PIN	   IOPORT_CREATE_PIN(PORTC, 3)
#define Z_STEPPER_CW_PIN	   IOPORT_CREATE_PIN(PORTC, 4)

//! LED Controller pins

#define LED_CLOCK_PIN	   IOPORT_CREATE_PIN(PORTE, 0)
#define LED_LOAD_PIN	   IOPORT_CREATE_PIN(PORTE, 1)
#define LED_DATA_PIN	   IOPORT_CREATE_PIN(PORTE, 3)

//! HMI for player 1
#define HMI1_LCD_PIN     IOPORT_CREATE_PIN(PORTD, 3)

#define LCD_RS_PIN		IOPORT_CREATE_PIN(PORTE, 4)
#define LCD_ENABLE_PIN		IOPORT_CREATE_PIN(PORTE, 5)
#define LCD_DATA0_PIN		IOPORT_CREATE_PIN(PORTF, 0)
#define LCD_DATA1_PIN		IOPORT_CREATE_PIN(PORTF, 1)
#define LCD_DATA2_PIN		IOPORT_CREATE_PIN(PORTF, 2)
#define LCD_DATA3_PIN		IOPORT_CREATE_PIN(PORTF, 3)
#define LCD_DATA4_PIN		IOPORT_CREATE_PIN(PORTF, 4)
#define LCD_DATA5_PIN		IOPORT_CREATE_PIN(PORTF, 5)
#define LCD_DATA6_PIN		IOPORT_CREATE_PIN(PORTF, 6)
#define LCD_DATA7_PIN		IOPORT_CREATE_PIN(PORTF, 7)

#define HMI1_BUTTON_PRESSED_PIN1     IOPORT_CREATE_PIN(PORTB, 4)
#define HMI1_BUTTON_PIN1	IOPORT_CREATE_PIN(PORTB, 0) //msb
#define HMI1_BUTTON_PIN2	IOPORT_CREATE_PIN(PORTB, 1)
#define HMI1_BUTTON_PIN3	IOPORT_CREATE_PIN(PORTB, 3) //lsb

//! function buttons
#define HMI_FUNCTION_BUTTON_PRESSED_PIN     IOPORT_CREATE_PIN(PORTE, 6)
#define HMI_FUNCTION_BUTTON_PIN1	IOPORT_CREATE_PIN(PORTB, 6) //msb
#define HMI_FUNCTION_BUTTON_PIN2	IOPORT_CREATE_PIN(PORTB, 5)
#define HMI_FUNCTION_BUTTON_PIN3	IOPORT_CREATE_PIN(PORTB, 2) //lsb

//! HMI for player 2
/*
#define HMI2_LCD_PIN     IOPORT_CREATE_PIN(PORTC, 3)
#define HMI2_BUTTON_PRESSED_PIN     IOPORT_CREATE_PIN(PORTE, 4)
#define HMI2_BUTTON_PIN1     IOPORT_CREATE_PIN(PORTE, 5) //msb
#define HMI2_BUTTON_PIN2     IOPORT_CREATE_PIN(PORTE, 6)
#define HMI2_BUTTON_PIN3     IOPORT_CREATE_PIN(PORTE, 7) //lsb
*/

//! function buttons
#define MOTOR_HOME_PIN_X     IOPORT_CREATE_PIN(PORTD, 5)
#define MOTOR_HOME_PIN_Y     IOPORT_CREATE_PIN(PORTD, 6)
#define MOTOR_HOME_PIN_Z     IOPORT_CREATE_PIN(PORTD, 7)

#endif  // _CONF_EXAMPLE_H