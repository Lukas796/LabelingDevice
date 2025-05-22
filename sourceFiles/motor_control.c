/*
 * motor_control.c
 *
 * Created: 15.05.2025 21:45
 * Authors : lukasstrittmatter
 */ 

#include "motor_control.h"
#include "config.h"


void motor_init(void) {
	// X
	STEP_X_DDR |= (1 << STEP_X_PIN);	//set Step X DDR as OUTPUT
	DIR_X_DDR  |= (1 << DIR_X_PIN);		//set Dir X DDR as OUTPUT
	EN_X_DDR   |= (1 << EN_X_PIN);		//set Enable X DDR as OUTPUT
	// Y
	STEP_Y_DDR |= (1 << STEP_Y_PIN);	//set Step Y DDR as OUTPUT
	DIR_Y_DDR  |= (1 << DIR_Y_PIN);		//set Dir Y DDR as OUTPUT
	EN_Y_DDR   |= (1 << EN_Y_PIN);		//set Enable Y DDR as OUTPUT
	// Z
	STEP_Z_DDR |= (1 << STEP_Z_PIN);	//set Step Z DDR as OUTPUT
	//DIR_Z_DDR  |= (1 << DIR_Z_PIN);
	//EN_Z_DDR   |= (1 << EN_Z_PIN);
}

void motor_enable(uint8_t axis) {
	switch (axis) {
		case AXIS_X: EN_X_PORT |= (1 << EN_X_PIN); break;	//SET EN_X_PIN to 1
		case AXIS_Y: EN_Y_PORT |= (1 << EN_Y_PIN); break;	//SET EN_Y_PIN to 1
		//case AXIS_Z: EN_Z_PORT |= (1 << EN_Z_PIN); break;
	}
}

void motor_disable(uint8_t axis) {
	switch (axis) {
		case AXIS_X: EN_X_PORT &= ~(1 << EN_X_PIN); break;	//SET EN_X_PIN to 0
		case AXIS_Y: EN_Y_PORT &= ~(1 << EN_Y_PIN); break;	//SET EN_Y_PIN to 0
		//case AXIS_Z: EN_Z_PORT &= ~(1 << EN_Z_PIN); break;
	}
}

void motor_set_direction(uint8_t axis, uint8_t direction) {
	switch (axis) {
		case AXIS_X:
		if (direction) DIR_X_PORT |= (1 << DIR_X_PIN);	//SET DIR_X_PIN to 1 if DIR_CCW
		else DIR_X_PORT &= ~(1 << DIR_X_PIN);			//SET DIR_X_PIN to 0 if DIR_CW
		break;
		case AXIS_Y:
		if (direction) DIR_Y_PORT |= (1 << DIR_Y_PIN);	//SET DIR_Y_PIN to 1 if DIR_CCW
		else DIR_Y_PORT &= ~(1 << DIR_Y_PIN);			//SET DIR_Y_PIN to 0 if DIR_CW
		break;
		case AXIS_Z:
		//if (direction) DIR_Z_PORT |= (1 << DIR_Z_PIN);
		//else DIR_Z_PORT &= ~(1 << DIR_Z_PIN);
		//break;
	}
}

void motor_step(uint8_t axis) {
	volatile uint8_t* port;
	uint8_t pin;

	switch (axis) {
		case AXIS_X: port = &STEP_X_PORT; pin = STEP_X_PIN; break;
		case AXIS_Y: port = &STEP_Y_PORT; pin = STEP_Y_PIN; break;
		case AXIS_Z: port = &STEP_Z_PORT; pin = STEP_Z_PIN; break;
		default: return;
	}

	*port |= (1 << pin);      // HIGH
	_delay_us(5);             // Mindestens 5 µs Puls
	*port &= ~(1 << pin);     // LOW
}