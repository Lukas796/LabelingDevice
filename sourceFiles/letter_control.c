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

const uint16_t speed = 100; // need to be divided by 4
const int16_t diag_steps_x = 100;
const int16_t diag_steps_z = 25;// need to be diag_steps_x /4
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
	move_Z_relative(-crossbar_z_steps-diag_steps_z,speed);
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+60,speed);
	move_X_relative(half_height_x_steps,speed);
	USART_SendString("A\n");
	USART_POSITIONIERUNG(1);
	
}

void draw_B(void) {
	
	move_pen_forward();
	move_X_relative(-height_x_steps-diag_steps_x,speed);	//move up - linker gerader Strich
	move_Z_relative(crossbar_z_steps,speed);				//move right - oberer Strich
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//move down/right. schräger strich rechts oben
	move_X_relative(height_x_steps,speed);					//move down - komplette höhe
	move_Z_relative(-crossbar_z_steps-diag_steps_z,speed);				//move left - unterer Strich
	move_pen_backward();
	move_X_relative(-half_height_x_steps,speed);					//move up - halbe höhe
	move_pen_forward();
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);				//move right - unterer Strich
	move_pen_backward();
	move_X_relative(half_height_x_steps,speed);					//move down - halbe höhe
	move_Z_relative(50,speed);			//move right - unterer Strich
	USART_SendString("B\n");
	USART_POSITIONIERUNG(1);
}

void draw_C(void) {
	
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_forward();										//pen forward
	move_Z_relative(-crossbar_z_steps,speed);				//move left to write
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);	//move up/left. schräger strich links unten
	move_X_relative(-height_x_steps+diag_steps_x,speed);				//move up - linker gerader Strich
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//move up/right. schräger strich links oben
	move_Z_relative(crossbar_z_steps,speed);				//move left to write
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);	//move completly down 
	move_Z_relative(50,speed);				//move right - to next letter
	USART_SendString("C\n");
	USART_POSITIONIERUNG(1);
}

void draw_D(void) {
	
	move_pen_forward();
	move_X_relative(-height_x_steps-diag_steps_x,speed);	//move up - linker gerader Strich
	move_Z_relative(crossbar_z_steps,speed);				//move right - oberer Strich
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//move down/right. schräger strich rechts oben
	move_X_relative(height_x_steps,speed);					//move down - komplette höhe
	move_Z_relative(-crossbar_z_steps-diag_steps_z,speed);				//move left - unterer Strich
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);			//move right - unterer Strich
	move_Z_relative(50,speed);			//move right - unterer Strich
	USART_SendString("D\n");
	USART_POSITIONIERUNG(1);
}

void draw_E(void) {
	
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_forward();										//pen forward
	move_Z_relative(-crossbar_z_steps,speed);				//move left to write
	move_X_relative(-half_height_x_steps-diag_steps_x,speed);			//move up - half Height
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_backward();								
	move_Z_relative(-crossbar_z_steps,speed);				//move left 
	move_pen_forward();
	move_X_relative(-half_height_x_steps,speed);	//move up - half Height
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);	// move completely down 
	move_Z_relative(50,speed);
	USART_SendString("E\n");
	USART_POSITIONIERUNG(1);
}

void draw_F(void) {
	
	move_pen_forward();										//pen forward
	move_X_relative(-half_height_x_steps-diag_steps_x,speed);			//move up - half Height
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_backward();
	move_Z_relative(-crossbar_z_steps,speed);				//move left
	move_pen_forward();
	move_X_relative(-half_height_x_steps,speed);	//move up - half Height
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);	// move completely down
	move_Z_relative(50,speed);
	USART_SendString("F\n");
	USART_POSITIONIERUNG(1);
}

void draw_G(void) {
	
	move_pen_forward();												//pen forward
	move_X_relative(-height_x_steps-diag_steps_x,speed);			//move up - complete Height
	move_Z_relative(crossbar_z_steps,speed);				//move left
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//move down/right. schräger strich rechts oben
	move_pen_backward();		
	move_X_relative(half_height_x_steps,speed);			//move down- half height
	move_pen_forward();
	move_Z_relative(-crossbar_z_steps-diag_steps_z,speed);				//move left
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);				//move left
	move_pen_forward();
	move_X_relative(half_height_x_steps,speed);			//move down- half height
	move_Z_relative(-crossbar_z_steps-diag_steps_z,speed);				//move left
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);				//move left
	move_Z_relative(50,speed);
	USART_SendString("E\n");
	USART_POSITIONIERUNG(1);
	USART_SendString("G\n");
	USART_POSITIONIERUNG(1);
}

void draw_H(void) {
	
	move_pen_forward();										//pen forward
	move_X_relative(-half_height_x_steps-diag_steps_x,speed);		//move up - half Height
	_delay_ms(20);
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_backward();
	move_Z_relative(-crossbar_z_steps,speed);			//move left
	move_pen_forward();
	move_X_relative(-half_height_x_steps,speed);	//move up - half Height
	move_pen_backward();
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_forward();
	move_X_relative(height_x_steps+diag_steps_x,speed);	//move down - completely
	move_pen_backward();
	move_Z_relative(50,speed);
	_delay_ms(20);
	USART_SendString("H\n");
	USART_POSITIONIERUNG(1);
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
	USART_SendString("I\n");
	USART_POSITIONIERUNG(1);
}

void draw_J(void) {
	
	move_X_relative(-height_x_steps-diag_steps_x,speed);	//ganz hoch 
	move_pen_forward();
	move_Z_relative(crossbar_z_steps,speed);		//oberer strich nach rechts
	_delay_ms(20);
	move_X_relative(height_x_steps,speed);	//runter 
	_delay_ms(20);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);	//rechts unten schräger strich
	_delay_ms(20);
	move_Z_relative(-crossbar_z_steps,speed);		//oberer strich nach rechts
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+60,speed);
	_delay_ms(20);
	USART_SendString("J\n");
	USART_POSITIONIERUNG(1);
}

void draw_K(void) {
	
	move_pen_forward();												//pen forward
	move_X_relative(-height_x_steps-diag_steps_x,speed);			//move up - complete Height
	move_pen_backward();
	move_X_relative(half_height_x_steps+diag_steps_x,speed);		//move down - half height
	move_pen_forward();
	move_Z_relative(crossbar_z_steps,speed);						//move right
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_X_relative(half_height_x_steps-diag_steps_x,speed);						//move down - half height
	move_pen_backward();
	move_X_relative(-half_height_x_steps+diag_steps_x,speed);						//move up - half Height
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);	//rechts unten schräger strich
	move_pen_forward();
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_X_relative(-half_height_x_steps,speed);		//move down - half height
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);			//move up - complete Height
	move_Z_relative(50,speed);
	_delay_ms(20);
	USART_SendString("K\n");
	USART_POSITIONIERUNG(1);
}

void draw_L(void) {

	move_X_relative(-height_x_steps-diag_steps_x,speed);	//ganz hoch
	move_pen_forward();
	move_X_relative(height_x_steps+diag_steps_x,speed);
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);
	move_pen_backward();
	move_Z_relative(50,speed);
	USART_SendString("L\n");
	USART_POSITIONIERUNG(1);
}

void draw_M(void) {
	
	move_pen_forward();
	move_X_relative(-height_x_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_Z_relative(50,speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_Z_relative(50,speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_X_relative(height_x_steps,speed);
	move_pen_backward();
	move_Z_relative(50,speed);
	_delay_ms(20);
	USART_SendString("M\n");
	USART_POSITIONIERUNG(1);
}

void draw_N(void) {
	
	move_pen_forward();
	move_X_relative(-height_x_steps-diag_steps_x,speed);
	move_Z_relative(50,speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_X_relative(height_x_steps,speed);
	move_Z_relative(50,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_X_relative(-height_x_steps,speed);
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);
	move_Z_relative(60,speed);
	_delay_ms(20);
	USART_SendString("N\n");
	USART_POSITIONIERUNG(1);
}

void draw_O(void) {
	
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_forward();										//pen forward
	move_Z_relative(-crossbar_z_steps,speed);				//move left to write
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);	//move up/left. schräger strich links unten
	move_X_relative(-height_x_steps+diag_steps_x,speed);				//move up - linker gerader Strich
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//move up/right. schräger strich links oben
	move_Z_relative(crossbar_z_steps,speed);				//move left to write
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//move up/left. schräger strich links unten
	move_X_relative(height_x_steps-diag_steps_x,speed);				//move up - linker gerader Strich
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);	//move up/right. schräger strich links oben
	move_pen_backward();
	move_Z_relative(diag_steps_z+50,speed);				//move right - to next letter
	USART_SendString("O\n");
	USART_POSITIONIERUNG(1);
}

void draw_P(void) {
	
	move_pen_forward();										//pen forward
	move_X_relative(-height_x_steps-diag_steps_x,speed);	//move up - full Height
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);	//move right
	move_X_relative(half_height_x_steps,speed);				//move down - half Height
	move_Z_relative(-crossbar_z_steps-diag_steps_z,speed);	//move left
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);	//move right
	move_X_relative(half_height_x_steps+diag_steps_x,speed);//move down - half Height
	move_Z_relative(50,speed);
	_delay_ms(20);
	USART_SendString("P\n");
	USART_POSITIONIERUNG(1);
}

void draw_Q(void) {
	
	move_Z_relative(crossbar_z_steps,speed);	//move right
	move_pen_forward();										//pen forward
	move_Z_relative(-crossbar_z_steps,speed);				//move left to write
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);	//move up/left. schräger strich links unten
	move_X_relative(-height_x_steps+diag_steps_x,speed);				//move up - linker gerader Strich
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//move up/right. schräger strich links oben
	move_Z_relative(crossbar_z_steps,speed);				//move left to write
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//move up/left. schräger strich links unten
	move_X_relative(height_x_steps-diag_steps_x,speed);				//move up - linker gerader Strich
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);	//move up/right. schräger strich links oben
	move_pen_backward();
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//move up/right. schräger strich links oben
	move_pen_forward();
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//move up/right. schräger strich links oben
	move_pen_backward();
	move_Z_relative(50,speed);				//move right - to next letter
	USART_SendString("Q\n");
	USART_POSITIONIERUNG(1);
}

void draw_R(void) {
	
	move_pen_forward();												//pen forward
	move_X_relative(-height_x_steps-diag_steps_x,speed);			//move up - complete Height
	move_pen_backward();
	move_X_relative(half_height_x_steps+diag_steps_x,speed);		//move down - half height
	move_pen_forward();
	move_Z_relative(crossbar_z_steps,speed);						//move right
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_X_relative(half_height_x_steps-diag_steps_x,speed);						//move down - half height
	move_pen_backward();
	move_X_relative(-half_height_x_steps+diag_steps_x,speed);						//move up - half Height
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);	//rechts unten schräger strich
	move_pen_forward();
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);	//rechts unten schräger strich
	move_X_relative(-half_height_x_steps,speed);		//move down - half height
	move_Z_relative(-crossbar_z_steps-diag_steps_z,speed);
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);
	move_X_relative(height_x_steps+diag_steps_x,speed);			//move up - complete Height
	move_Z_relative(50,speed);
	_delay_ms(20);
	USART_SendString("R\n");
	USART_POSITIONIERUNG(1);
}

void draw_S(void) {
	
	move_pen_forward();												//pen forward
	move_Z_relative(crossbar_z_steps,speed);
	move_X_relative(-half_height_x_steps-diag_steps_x,speed);			//move up - complete Height
	move_Z_relative(-crossbar_z_steps,speed);
	move_X_relative(-half_height_x_steps,speed);			//move up - complete Height
	move_Z_relative(crossbar_z_steps,speed);
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);			//move up - complete Height
	move_Z_relative(50,speed);
	USART_SendString("S\n");
	USART_POSITIONIERUNG(1);
	
}

void draw_T(void) {
	
	move_X_relative(-height_x_steps-diag_steps_x,speed);			//move up - complete Height
	move_pen_forward();												//pen forward
	move_Z_relative(crossbar_z_steps+crossbar_z_steps,speed);
	move_pen_backward();
	move_Z_relative(-crossbar_z_steps,speed);
	move_pen_forward();	
	move_X_relative(height_x_steps+diag_steps_x,speed);			//move up - complete Height
	move_pen_backward();
	move_Z_relative(crossbar_z_steps+50,speed);
	USART_SendString("T\n");
	USART_POSITIONIERUNG(1);
	
}

void draw_U(void) {
	
	move_X_relative(-height_x_steps-diag_steps_x,speed);
	move_pen_forward();
	move_X_relative(height_x_steps,speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(crossbar_z_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	move_X_relative(-height_x_steps,speed);
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);
	move_Z_relative(50,speed);
	USART_SendString("U\n");
	USART_POSITIONIERUNG(1);
}

void draw_V(void) {
	
	move_X_relative(-height_x_steps-diag_steps_x,speed);
	move_pen_forward();
	move_X_relative(height_x_steps,speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(crossbar_z_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	move_X_relative(-height_x_steps,speed);
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);
	move_Z_relative(50,speed);
	USART_SendString("V\n");
	USART_POSITIONIERUNG(1);
}

void draw_W(void) {
	
	move_X_relative(-height_x_steps-diag_steps_x,speed);
	move_pen_forward();
	move_X_relative(height_x_steps,speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(50,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(50,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	move_X_relative(-height_x_steps,speed);
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);
	move_Z_relative(50,speed);
	USART_SendString("W\n");
	USART_POSITIONIERUNG(1);
}

void draw_X(void) {
	
	move_pen_forward();
	move_X_relative(-half_height_x_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	move_XZ_diagonal_relative(-diag_steps_x, -diag_steps_z, speed);
	move_X_relative(-half_height_x_steps+diag_steps_x,speed);
	move_pen_backward();
	move_X_relative(half_height_x_steps-diag_steps_x,speed);
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_pen_forward();
	move_Z_relative(crossbar_z_steps,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	move_X_relative(-half_height_x_steps+diag_steps_x,speed);
	move_pen_backward();
	move_X_relative(half_height_x_steps-diag_steps_x,speed);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	move_pen_forward();
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_X_relative(half_height_x_steps,speed);
	move_pen_backward();
	move_Z_relative(50,speed);
	USART_SendString("X\n");
	USART_POSITIONIERUNG(1);
}

void draw_Y(void) {
	
	move_X_relative(-height_x_steps-diag_steps_x,speed);
	move_pen_forward();
	move_XZ_diagonal_relative(diag_steps_x, diag_steps_z, speed);
	move_Z_relative(50,speed);
	move_X_relative(height_x_steps,speed);
	move_pen_backward();
	move_X_relative(-height_x_steps,speed);
	move_pen_forward();
	move_Z_relative(50,speed);
	move_XZ_diagonal_relative(-diag_steps_x, diag_steps_z, speed);
	move_pen_backward();
	move_X_relative(height_x_steps+diag_steps_x,speed);
	move_Z_relative(50,speed);
	USART_SendString("Y\n");
	USART_POSITIONIERUNG(1);
}

void draw_Z(void) {
	
	move_X_relative(-height_x_steps-diag_steps_x,speed);
	move_pen_forward();
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	move_X_relative(half_height_x_steps-diag_steps_x,speed);
	move_Z_relative(-crossbar_z_steps+diag_steps_z,speed);
	move_X_relative(half_height_x_steps,speed);
	move_XZ_diagonal_relative(diag_steps_x, -diag_steps_z, speed);
	move_Z_relative(crossbar_z_steps+diag_steps_z,speed);
	move_pen_backward();
	move_Z_relative(50,speed);
	USART_SendString("Z\n");
	USART_POSITIONIERUNG(1);
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