/*
 * motor_control.c
 *
 * Created: 15.05.2025 21:45
 * Authors : lukasstrittmatter
 */ 

#include "motor_control.h"
#include "config.h"

// Beispiel-Pinbelegung (aus deiner Hardwarezuweisung)
#define STEP_X_PORT PORTB
#define STEP_X_DDR  DDRB
#define STEP_X_PIN  PB1
#define DIR_X_PORT  PORTB
#define DIR_X_PIN   PB0
#define EN_X_PORT   PORTB
#define EN_X_PIN    PB2

#define STEP_Y_PORT PORTB
#define STEP_Y_DDR  DDRB
#define STEP_Y_PIN  PB3
#define DIR_Y_PORT  PORTB
#define DIR_Y_PIN   PB4
#define EN_Y_PORT   PORTB
#define EN_Y_PIN    PB5

#define STEP_Z_PORT PORTB
#define STEP_Z_DDR  DDRB
#define STEP_Z_PIN  PB6
#define DIR_Z_PORT  PORTB
#define DIR_Z_PIN   PB7
#define EN_Z_PORT   PORTD
#define EN_Z_PIN    PD6

void motor_init(void) {
	// X
	STEP_X_DDR |= (1 << STEP_X_PIN);
	DIR_X_DDR  |= (1 << DIR_X_PIN);
	EN_X_DDR   |= (1 << EN_X_PIN);
	// Y
	STEP_Y_DDR |= (1 << STEP_Y_PIN);
	DIR_Y_DDR  |= (1 << DIR_Y_PIN);
	EN_Y_DDR   |= (1 << EN_Y_PIN);
	// Z
	STEP_Z_DDR |= (1 << STEP_Z_PIN);
	DIR_Z_DDR  |= (1 << DIR_Z_PIN);
	EN_Z_DDR   |= (1 << EN_Z_PIN);
}

void motor_enable(uint8_t axis) {
	switch (axis) {
		case AXIS_X: EN_X_PORT |= (1 << EN_X_PIN); break;
		case AXIS_Y: EN_Y_PORT |= (1 << EN_Y_PIN); break;
		case AXIS_Z: EN_Z_PORT |= (1 << EN_Z_PIN); break;
	}
}

void motor_disable(uint8_t axis) {
	switch (axis) {
		case AXIS_X: EN_X_PORT &= ~(1 << EN_X_PIN); break;
		case AXIS_Y: EN_Y_PORT &= ~(1 << EN_Y_PIN); break;
		case AXIS_Z: EN_Z_PORT &= ~(1 << EN_Z_PIN); break;
	}
}

void motor_set_direction(uint8_t axis, uint8_t direction) {
	switch (axis) {
		case AXIS_X:
		if (direction) DIR_X_PORT |= (1 << DIR_X_PIN);
		else DIR_X_PORT &= ~(1 << DIR_X_PIN);
		break;
		case AXIS_Y:
		if (direction) DIR_Y_PORT |= (1 << DIR_Y_PIN);
		else DIR_Y_PORT &= ~(1 << DIR_Y_PIN);
		break;
		case AXIS_Z:
		if (direction) DIR_Z_PORT |= (1 << DIR_Z_PIN);
		else DIR_Z_PORT &= ~(1 << DIR_Z_PIN);
		break;
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