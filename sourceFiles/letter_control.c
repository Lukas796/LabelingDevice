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
uint8_t pos_aktiv = 0;

void draw_A() {
	const uint16_t speed = 600;
	const int32_t diag_steps_x = 600;
	const int32_t diag_steps_z = 150;
	const int32_t crossbar_z_steps = 150;
	const int32_t height_x_steps = 600;
	
	
	//move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	//move_XZ_diagonal_relative(diag_steps_x,diag_steps_z,speed);
	//move_pen_backward();
	//move_XZ_diagonal_relative(-diag_steps_x/2, -diag_steps_z/2, speed);
	//move_pen_forward();
	//move_Z_relative(-crossbar_z_steps,speed);
	//move_pen_backward();
	//move_XZ_diagonal_relative(diag_steps_x/2, diag_steps_z/2, speed);
	//move_Z_relative(crossbar_z_steps+20,speed);
	
	move_to_position_steps_xy(2300,3000,400);
	USART_SendString("A\n");
	USART_POSITIONIERUNG(1);
	move_to_position_steps_xy(100,100,400);
	USART_SendString("B\n");
	USART_POSITIONIERUNG(1);
	move_to_position_steps_xy(2000,3000,400);
	USART_SendString("C\n");
	USART_POSITIONIERUNG(1);
}