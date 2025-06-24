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
	const uint16_t speed = 100;
	const int32_t diag_steps_x = 200;
	const int32_t diag_steps_z = 200;
	const int32_t crossbar_z_steps = 100;

	// Ausgangspunkt ? 
	move_to_position_steps_xy(1800,4300,100);
//
	//// Linker Schrägstrich nach oben (?)
	//move_XZ_diagonal(diag_steps_x, diag_steps_z, speed);
//
	//// Stift abheben, zurück zur Mitte
	//move_pen_up();
	//move_XZ_diagonal(-diag_steps_x / 2, -diag_steps_z / 2, speed);
//
	//// Querstrich (Z-Achse, ohne X)
	//move_pen_down();
	//move_Z(crossbar_z_steps, speed);
//
	//// Zurück zur Mitte, Stift heben
	//move_pen_up();
	//move_Z(-crossbar_z_steps, speed);
//
	//// Rechter Schrägstrich nach unten (?)
	//move_XZ_diagonal(diag_steps_x / 2, -diag_steps_z / 2, speed);
//
	//move_pen_up();
}