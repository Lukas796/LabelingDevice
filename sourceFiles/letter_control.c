/*
 * letter_control.c
 *
 * Created: 18.06.2025 13:36:09
 *  Author: lukasstrittmatter
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "motor_control.h"
#include "config.h"
#include "systemstate.h"
#include "letter_control.h"

void draw_A() {
	const uint16_t speed = 400;
	const int32_t diag_steps_x = 200;
	const int32_t diag_steps_z = 50;
	const int32_t crossbar_z_steps = 50;
	const int32_t height_x_steps = 200;
	
	
	//move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	//move_pen_backward();
	//move_pen_forward();
	//move_XZ_diagonal_relative(diag_steps_x,diag_steps_z,speed);
	//move_pen_backward();
	//move_XZ_diagonal_relative(-diag_steps_x/2, -diag_steps_z/2, speed);
	//move_pen_forward();
	//move_Z_relative(-crossbar_z_steps,speed);
	//move_pen_backward();
	//move_XZ_diagonal_relative(diag_steps_x/2, diag_steps_z/2, speed);
	//_delay_ms(20);
	//move_Z_relative(crossbar_z_steps+20,speed);

}