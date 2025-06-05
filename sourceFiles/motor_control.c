/*
 * motor_control.c
 *
 * Created: 15.05.2025 21:45
 * Authors : lukasstrittmatter
 */ 

#include "motor_control.h"
#include "config.h"

// Schrittzähler je Motor
volatile uint16_t steps_x_done = 0;
volatile uint16_t steps_y_done = 0;
volatile uint16_t steps_z_done = 0;

volatile uint16_t steps_x_target = 0;
volatile uint16_t steps_y_target = 0;
volatile uint16_t steps_z_target = 0;

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
	DIR_Z_DDR  |= (1 << DIR_Z_PIN);		//set Step Z DDR as OUTPUT
	EN_Z_DDR   |= (1 << EN_Z_PIN);
}

void motor_enable(uint8_t axis) {
	switch (axis) {
		case AXIS_X: EN_X_PORT &= ~(1 << EN_X_PIN); break;	//SET EN_X_PIN to 0 - low active
		case AXIS_Y: EN_Y_PORT &= ~(1 << EN_Y_PIN); break;	//SET EN_Y_PIN to 0 - low active
		case AXIS_Z: EN_Z_PORT |= (1 << EN_Z_PIN); break;	//SET EN_Z_PIN to 1 - high active
	}
}

void motor_disable(uint8_t axis) {
	switch (axis) {
		case AXIS_X: EN_X_PORT |= (1 << EN_X_PIN); break;	//SET EN_X_PIN to 1
		case AXIS_Y: EN_Y_PORT |= (1 << EN_Y_PIN); break;	//SET EN_Y_PIN to 1
		case AXIS_Z: EN_Z_PORT &= ~(1 << EN_Z_PIN); break;	//SET EN_Z_PIN to 0
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
		if (direction) DIR_Z_PORT |= (1 << DIR_Z_PIN);	//SET DIR_Z_PIN to 1 if DIR_CCW
		else DIR_Z_PORT &= ~(1 << DIR_Z_PIN);			//SET DIR_Z_PIN to 0 if DIR_CW
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

	*port |= (1 << pin);
	_delay_ms(20);
	*port &= ~(1 << pin);
	_delay_ms(20);
}

void motor_init_timer(void) {
	// --- Timer X: Timer3, OC3B (PE4) ---
	STEP_X_OC_REG |= (1 << STEP_X_OC_BIT);			// Toggle OC3B on compare match
	STEP_X_TCCRB_REG |= (1 << WGM32);				// set CTC - Mode (Clear Timer on Compare Match)
	STEP_X_OCR = 100;								// set init Value for OCR3A --> Target Value for Compare Match
	STEP_X_TIMSK_REG |= (1 << STEP_X_OCIE_BIT);     // activate OCIE3A bit in TIMSK3 Register to activate Interrupt on Compare Match
	
	// --- Timer Y: Timer1, OC1B (PB6) ---
	STEP_Y_OC_REG |= (1 << STEP_Y_OC_BIT);       // Toggle OC1B on compare match
	STEP_Y_TCCRB |= (1 << WGM12);                // CTC-Modus: WGM12 = 1
	STEP_Y_OCR = 100;
	STEP_Y_TIMSK |= (1 << STEP_Y_OCIE_BIT);
	
	// --- Timer Z: Timer4, OC4B (PH4) ---
	STEP_Z_OC_REG |= (1 << STEP_Z_OC_BIT);       // Toggle OC4B on compare match
	STEP_Z_TCCRB |= (1 << WGM42);                // CTC-Modus: WGM42 = 1
	STEP_Z_OCR = 100;
	STEP_Z_TIMSK |= (1 << STEP_Z_OCIE_BIT);
}
// --- Timer-based motor Control --
void motor_start_steps(uint8_t axis, uint16_t steps) {
switch(axis) {
	case AXIS_X:
	steps_x_done = 0;
	steps_x_target = steps * 2;

	// Calculation of OCR - Bit: 
	// OCR = Fcpu/(2*PrescalerValue*fmotor) -1
	// example: for f = 1,2kHz --> 16000000/(2*64*1200)-1 = 100
	STEP_X_TCNT = 0;
	STEP_X_OCR = 100;
	STEP_X_TCCRB_REG |= (1 << CS31) | (1 << CS30);             // start Motor with Prescaler 64. 
	
	break;

	case AXIS_Y:
	steps_y_done = 0;
	steps_y_target = steps * 2;

	STEP_Y_TCNT = 0;
	STEP_Y_OCR = 100;
	STEP_Y_OC_REG |= (1 << STEP_Y_OC_BIT);
	STEP_Y_TCCRB |= (1 << WGM12);
	STEP_Y_TIMSK |= (1 << STEP_Y_OCIE_BIT);
	STEP_Y_TCCRB |= (1 << CS11);
	break;

	case AXIS_Z:
	steps_z_done = 0;
	steps_z_target = steps * 2;

	STEP_Z_TCNT = 0;
	STEP_Z_OCR = 100;
	STEP_Z_OC_REG |= (1 << STEP_Z_OC_BIT);
	STEP_Z_TCCRB |= (1 << WGM42);
	STEP_Z_TIMSK |= (1 << STEP_Z_OCIE_BIT);
	STEP_Z_TCCRB |= (1 << CS41);
	break;
}
}

void motor_stop(uint8_t axis) {
	switch(axis) {
		case AXIS_X:
		STEP_X_TCCRB_REG &= ~((1 << CS32) | (1 << CS31) | (1 << CS30));
		STEP_X_TIMSK_REG &= ~(1 << STEP_X_OCIE_BIT);
		break;

		case AXIS_Y:
		STEP_Y_TCCRB &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		STEP_Y_TIMSK &= ~(1 << STEP_Y_OCIE_BIT);
		break;

		case AXIS_Z:
		STEP_Z_TCCRB &= ~((1 << CS42) | (1 << CS41) | (1 << CS40));
		STEP_Z_TIMSK &= ~(1 << STEP_Z_OCIE_BIT);
		break;
	}
}

 //--- ISR: Interrupts on CompareMatch from the Timers ---
 ISR(TIMER3_COMPA_vect) {
	 steps_x_done++;
	 if (steps_x_done >= steps_x_target) {
		 motor_stop(AXIS_X);
	 }
 }

 ISR(TIMER1_COMPA_vect) {
	 steps_y_done++;
	 if (steps_y_done >= steps_y_target) {
		 motor_stop(AXIS_Y);
	 }
 }

 ISR(TIMER4_COMPA_vect) {
	 steps_z_done++;
	 if (steps_z_done >= steps_z_target) {
		 motor_stop(AXIS_Z);
	 }
 }