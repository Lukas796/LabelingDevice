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
const int16_t diag_steps_x = 100;
const int16_t diag_steps_z = 100;
const int16_t crossbar_z_steps = 100;
const int16_t height_x_steps = 500;
const int16_t half_height_x_steps = 250;

void draw_A(void) {
	
	move_pen_forward();
	move_X_relative(-height_x_steps,speed);
	_delay_ms(20);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	_delay_ms(20);
	move_Z_relative(crossbar_z_steps,speed);
	_delay_ms(20);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	_delay_ms(20);
	move_X_relative(height_x_steps,speed);
	_delay_ms(20);
	move_pen_backward();
	move_X_relative(-half_height_x_steps,speed);
	_delay_ms(20);
	move_pen_forward();
	move_Z_relative(-crossbar_z_steps-diag_steps_z,speed);
	_delay_ms(20);
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+50,speed);
	_delay_ms(20);
	move_X_relative(half_height_x_steps,speed);
	_delay_ms(20);
	
}

void draw_I(void) {
	
	move_pen_forward();
	move_X_relative(-height_x_steps-diag_steps_x,speed);
	_delay_ms(20);
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);
	_delay_ms(20);
	move_Z_relative(50,speed);
	_delay_ms(20);
}

void draw_U(void) {
	
	move_X_relative(-height_x_steps-diag_steps_x,speed);
	_delay_ms(20);
	move_pen_forward();
	move_X_relative(height_x_steps,speed);
	_delay_ms(20);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	_delay_ms(20);
	move_Z_relative(crossbar_z_steps,speed);
	_delay_ms(20);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	_delay_ms(20);
	move_X_relative(-height_x_steps,speed);
	_delay_ms(20);
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);
	_delay_ms(20);
	move_Z_relative(50,speed);
	_delay_ms(20);
}
