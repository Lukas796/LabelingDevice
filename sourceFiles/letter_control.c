/*
 * letter_control.c
 *
 * Created: 18.06.2025 13:36:09
 *  Author: lukasstrittmatter
 */ 

#include "config.h" // need to be first
#include "letter_control.h"
#include "motor_control.h"
#include "systemstate.h"

const uint16_t speed = 200;
const int16_t diag_steps_x = 50;
const int16_t diag_steps_z = 50;
const int16_t crossbar_z_steps = 100;
const int16_t height_x_steps = 500;
const int16_t half_height_x_steps = 250;

void draw_A(void) {
	
	move_pen_forward();
	move_X_relative(-height_x_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	move_Z_relative(crossbar_z_steps,speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_X_relative(height_x_steps,speed);
	move_pen_backward();
	move_X_relative(-half_height_x_steps,speed);
	move_pen_forward();
	move_Z_relative(-crossbar_z_steps,speed);
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+20,speed);
	move_X_relative(half_height_x_steps,speed);
	
}
