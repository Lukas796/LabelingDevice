/*
 * letter_control.c
 *
 * Created: 18.06.2025 13:36:09
 *  Author: lukasstrittmatter
 */ 

#include "config.h" // need to be first, include generell config 
#include "letter_control.h"	// inkludiere die eigene Header datei
#include "motor_control.h"	// inkludiere die Motor_control- Funktionen 
#include "systemstate.h"	// inkludiere die Systemstate - Funktionen 
#include "USART.h"			// inkludiere die USART Funktionen 

const uint16_t speed = 400;			// need to be divided by 4
const int16_t diag_steps_x = 100;	// diagonal steps in X-Dirction
const int16_t diag_steps_z = 25;	// need to be diag_steps_x /4
const int16_t crossbar_z_steps = 100;	//crossbar z steps
const int16_t height_x_steps = 500;		// height x Steps
const int16_t half_height_x_steps = 250;	// half Height X Steps -> nned to be height_x Steps /2

void draw_A(void) {
	
	if (pos_aktiv){
		USART_SendString("\n");					//Mache neuen Absatz, wenn Positionierung in GUI aktiv
		USART_POSITIONIERUNG(1);}				//sende Anfangs Position, wenn Positionierung in GUI aktiv
	move_pen_forward();							//setzte Stift vor
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_X_relative(-height_x_steps,speed);		//fahre X Hoch 
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	// fahre diagonal hoch und rechts
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_Z_relative(crossbar_z_steps,speed);	//fahre Z rechts
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	// fahre diagonal runter und rechts
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_X_relative(height_x_steps,speed);		// fahre X komplett runter 
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_pen_backward();						//setzte Stift zurück
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_X_relative(-half_height_x_steps,speed);	//fahre zur Hälfte hoch
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_pen_forward();							// setzte Stift nach vorne
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_Z_relative(-crossbar_z_steps-diag_steps_z-diag_steps_z,speed);	// fahre Z komplett nach links
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist	
	move_pen_backward();						// setzte Stift zurück
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_Z_relative(crossbar_z_steps+diag_steps_z+diag_steps_z+50,speed);	//fahre Z nach rechts zum Startpunkt des neuen Buchstaben
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
	move_X_relative(half_height_x_steps,speed);	// fahre X wieder um die  Hälfte runter
	if (pos_aktiv){USART_POSITIONIERUNG(1);}	//sendet aktuelle Position, wenn Positionierung in GUI aktiv ist
}

void draw_B(void) {
	if (pos_aktiv) {
		USART_SendString("\n");
		USART_POSITIONIERUNG(1);}
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps - diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps + diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}


void draw_C(void) {
	if (pos_aktiv) { USART_SendString("\n"); 
		USART_POSITIONIERUNG(1); }
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_X_relative(-height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_X_relative(height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1); 
	move_Z_relative(50, speed);
}

void draw_D(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps-diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps+diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_E(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_F(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_G(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps-diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps+diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps-diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps+diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_H(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_I(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_J(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps+60, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_K(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_L(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps+diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_M(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_N(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(60, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_O(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(diag_steps_z+50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_P(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps+diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps-diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps+diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_Q(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_R(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps-diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps+diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_S(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps-diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps+diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_T(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_X_relative(-height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps + crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps + 50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_U(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_X_relative(-height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_V(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_X_relative(-height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_W(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_X_relative(-height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_X(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-half_height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_Y(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_X_relative(-height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(-height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(height_x_steps + diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_Z(void) {
	if (pos_aktiv) { USART_SendString("\n");
	USART_POSITIONIERUNG(1); }
	move_X_relative(-height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_forward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps + diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps - diag_steps_x, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(-crossbar_z_steps + diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_X_relative(half_height_x_steps, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(crossbar_z_steps + diag_steps_z, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_pen_backward();
	if (pos_aktiv) USART_POSITIONIERUNG(1);
	move_Z_relative(50, speed);
	if (pos_aktiv) USART_POSITIONIERUNG(1);
}

void draw_space(void) {
	
	move_Z_relative(200,speed);
	USART_SendString(" \n");
	USART_POSITIONIERUNG(1);
}

void draw_TEST(void) {
	move_pen_forward();
	move_Z_relative(crossbar_z_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(-crossbar_z_steps,speed);
	move_Z_relative(crossbar_z_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(-crossbar_z_steps,speed);
	move_Z_relative(crossbar_z_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(-crossbar_z_steps,speed);
	move_Z_relative(crossbar_z_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(-crossbar_z_steps,speed);
	move_X_relative(-height_x_steps,speed);
	move_X_relative(height_x_steps,speed);
	move_X_relative(-height_x_steps,speed);
	move_X_relative(height_x_steps,speed);
	move_X_relative(-height_x_steps,speed);
	move_X_relative(height_x_steps,speed);
	move_X_relative(-height_x_steps,speed);
	move_X_relative(height_x_steps,speed);
	}