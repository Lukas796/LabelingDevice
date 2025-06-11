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
	EN_Z_DDR   |= (1 << EN_Z_PIN);		//set Enable Z DDR as OUTPUT
}

void motor_init_timer(void) {
	// --- Timer X: Timer3, OC3B (PE4) ---
	STEP_X_OC_REG |= (1 << STEP_X_OC_BIT);			// Toggle OC3B on compare match - set COM3B0 in TCCR3A
	STEP_X_TCCRB_REG |= (1 << WGM32);				// set CTC - Mode (Clear Timer on Compare Match) - set WGM32 in TCCR3B
	STEP_X_OCR = 100;								// set init Value for OCR3A --> Target Value for Compare Match
	STEP_X_TIMSK_REG |= (1 << STEP_X_OCIE_BIT);     // activate OCIE3B bit in TIMSK3 Register to activate Interrupt on Compare Match
	
	// --- Timer Y: Timer1, OC1B (PB6) ---
	STEP_Y_OC_REG |= (1 << STEP_Y_OC_BIT);			 // Toggle OC1B on compare match-- setze COM1B= in TCCR1A
	STEP_Y_TCCRB_REG |= (1 << WGM12);                // CTC-Modus: setze WGM12 in TCCR1B
	STEP_Y_OCR = 100;
	STEP_Y_TIMSK_REG |= (1 << STEP_Y_OCIE_BIT);
	
	// --- Timer Z: Timer4, OC4B (PH4) ---
	STEP_Z_OC_REG |= (1 << STEP_Z_OC_BIT);       // Toggle OC4B on compare match
	STEP_Z_TCCRB_REG |= (1 << WGM42);                // CTC-Modus: WGM42 = 1
	STEP_Z_OCR = 100;
	STEP_Z_TIMSK_REG |= (1 << STEP_Z_OCIE_BIT);
}

void limit_switch_init(void) {
	// Set all end sensor pins as input with internal pull-up resistors

	// X-BOTTOM
	X_SWITCH_BOTTOM_DDR &= ~(1 << X_SWITCH_BOTTOM_PIN);   // Set as input
	X_SWITCH_BOTTOM_PORT |= (1 << X_SWITCH_BOTTOM_PIN);   // Enable internal pull-up

	// X-TOP
	X_SWITCH_TOP_DDR &= ~(1 << X_SWITCH_TOP_PIN);
	X_SWITCH_TOP_PORT |= (1 << X_SWITCH_TOP_PIN);

	// X-LEFT
	Y_SWITCH_LEFT_DDR &= ~(1 << Y_SWITCH_LEFT_PIN);
	Y_SWITCH_LEFT_PORT |= (1 << Y_SWITCH_LEFT_PIN);

	// X-RIGHT
	Y_SWITCH_RIGHT_DDR &= ~(1 << Y_SWITCH_RIGHT_PIN);
	Y_SWITCH_RIGHT_PORT |= (1 << Y_SWITCH_RIGHT_PIN);
}

void button_init(void) {
	// Set PG1 as input
	CONFIRM_BUTTON_DDR &= ~(1 << CONFIRM_BUTTON_PIN);       // Set as input
	CONFIRM_BUTTON_PORT |= (1 << CONFIRM_BUTTON_PIN);       // Enable internal pull-up
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


// --- Timer-based motor Control --
void motor_start_steps(uint8_t axis, uint16_t steps, uint16_t freq_hz) {
	const uint16_t prescaler = 64;
	uint16_t ocr_val;

	// set freqeuncy borders
	if (freq_hz < 5) freq_hz = 5;
	if (freq_hz > 2000) freq_hz = 2000;

	// Calculation of OCR - Bit:
	// OCR = Fcpu/(2*PrescalerValue*fmotor) -1
	// example: for f = 1,2kHz --> 16000000/(2*64*1200)-1 = 100
	// 2UL --> unsigned ling intege
	ocr_val = (F_CPU / (2UL * prescaler * freq_hz)) - 1;

	switch(axis) {
		case AXIS_X:
		steps_x_done = 0;
		steps_x_target = steps * 2;

		STEP_X_TCCRB_REG &= ~((1 << CS32) | (1 << CS31) | (1 << CS30)); // stop Timer, to set Velocity
		STEP_X_TCNT = 0;
		STEP_X_OCR = ocr_val;
		STEP_X_TIMSK_REG |= (1 << STEP_X_OCIE_BIT);    // ? INTERRUPT AKTIVIEREN
		STEP_X_TCCRB_REG |= (1 << CS31) | (1 << CS30); // start timer with Prescaler 64
		break;

		case AXIS_Y:
		steps_y_done = 0;
		steps_y_target = steps * 2;

		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		STEP_Y_TCNT = 0;
		STEP_Y_OCR = ocr_val;
		STEP_Y_TIMSK_REG |= (1 << STEP_Y_OCIE_BIT);    // ? INTERRUPT AKTIVIEREN
		STEP_Y_TCCRB_REG |= (1 << CS11) | (1 << CS10);
		break;

		case AXIS_Z:
		steps_z_done = 0;
		steps_z_target = steps * 2;

		STEP_Z_TCCRB_REG &= ~((1 << CS42) | (1 << CS41) | (1 << CS40));
		STEP_Z_TCNT = 0;
		STEP_Z_OCR = ocr_val;
		STEP_Z_TIMSK_REG |= (1 << STEP_Z_OCIE_BIT);    // ? INTERRUPT AKTIVIEREN
		STEP_Z_TCCRB_REG |= (1 << CS41) | (1 << CS40);
		break;

		default:
		break;
	}
}

// --- Timer-based motor Control --
void motor_start_continous(uint8_t axis, uint16_t freq_hz) {
	const uint16_t prescaler = 64;
	uint16_t ocr_val;

	// set freqeuncy borders
	if (freq_hz < 5) freq_hz = 5;
	if (freq_hz > 2000) freq_hz = 2000;

	// Calculation of OCR - Bit:
	// OCR = Fcpu/(2*PrescalerValue*fmotor) -1
	// example: for f = 1,2kHz --> 16000000/(2*64*1200)-1 = 100
	// 2UL --> unsigned ling intege
	ocr_val = (F_CPU / (2UL * prescaler * freq_hz)) - 1;

	switch(axis) {
		case AXIS_X:
		// Interrupt deaktivieren
		STEP_X_TIMSK_REG &= ~(1 << STEP_X_OCIE_BIT);
		STEP_X_TCCRB_REG &= ~((1 << CS32) | (1 << CS31) | (1 << CS30)); // stop Timer, to set Velocity
		STEP_X_TCNT = 0;
		STEP_X_OCR = ocr_val;
		STEP_X_TCCRB_REG |= (1 << CS31) | (1 << CS30); // start timer with Prescaler 64
		break;

		case AXIS_Y:
		STEP_Y_TIMSK_REG &= ~(1 << STEP_Y_OCIE_BIT);
		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		STEP_Y_TCNT = 0;
		STEP_Y_OCR = ocr_val;
		STEP_Y_TCCRB_REG |= (1 << CS11) | (1 << CS10);
		break;

		case AXIS_Z:
		STEP_Z_TIMSK_REG &= ~(1 << STEP_Z_OCIE_BIT);
		STEP_Z_TCCRB_REG &= ~((1 << CS42) | (1 << CS41) | (1 << CS40));
		STEP_Z_TCNT = 0;
		STEP_Z_OCR = ocr_val;
		STEP_Z_TCCRB_REG |= (1 << CS41) | (1 << CS40);
		break;

		default:
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
		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		STEP_Y_TIMSK_REG &= ~(1 << STEP_Y_OCIE_BIT);
		break;

		case AXIS_Z:
		STEP_Z_TCCRB_REG &= ~((1 << CS42) | (1 << CS41) | (1 << CS40));
		STEP_Z_TIMSK_REG &= ~(1 << STEP_Z_OCIE_BIT);
		break;
	}
}

 //--- ISR: Interrupts on CompareMatch from the Timers ---
 ISR(TIMER3_COMPB_vect) {
	 steps_x_done++;
	 if (steps_x_done >= steps_x_target) {
		 motor_stop(AXIS_X);
	 }
 }

 ISR(TIMER1_COMPB_vect) {
	 steps_y_done++;
	 if (steps_y_done >= steps_y_target) {
		 motor_stop(AXIS_Y);
	 }
 }

 ISR(TIMER4_COMPB_vect) {
	 steps_z_done++;
	 if (steps_z_done >= steps_z_target) {
		 motor_stop(AXIS_Z);
	 }
 }
 
 void set_X_Y_direction(uint8_t direction) {
	 switch (direction) {
		 case DIR_X_TOP: // X positiv ? X = CCW, Y = CCW
		 motor_set_direction(AXIS_X, DIR_CCW);
		 motor_set_direction(AXIS_Y, DIR_CCW);
		 break;

		 case DIR_X_BOTTOM: // X negativ ? X = CW, Y = CW
		 motor_set_direction(AXIS_X, DIR_CW);
		 motor_set_direction(AXIS_Y, DIR_CW);
		 break;
		 
		 case DIR_Y_LEFT: // Y positiv ? X = CW, Y = CCW
		 motor_set_direction(AXIS_X, DIR_CW);
		 motor_set_direction(AXIS_Y, DIR_CCW);
		 break;

		 case DIR_Y_RIGHT: // Y negativ ? X = CCW, Y = CW
		 motor_set_direction(AXIS_X, DIR_CCW);
		 motor_set_direction(AXIS_Y, DIR_CW);
		 break;
		 default:
		 break;
	 }
 }
 
 void start_XY_reference(void) {
	 // Set direction: move toward Y_RIGHT first
	 set_X_Y_direction(DIR_Y_RIGHT);
	 motor_start_continous(AXIS_X, 1000);  // beide Motoren starten mit 100 Hz
	 motor_start_continous(AXIS_Y, 1000);  // beide Motoren starten mit 100 Hz

	 // Warte auf Y_RIGHT Endschalter
	 while (Y_SWITCH_RIGHT_IN_REG & (1 << Y_SWITCH_RIGHT_PIN)) {
		 // warte, bis Endschalter aktiv (Low durch Pullup)
	 }

	 // Stop Motoren nach Erreichen von Y_RIGHT
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 _delay_ms(200);  // mechanische Dämpfung

	 // Set direction: move toward X_TOP
	 set_X_Y_direction(DIR_X_TOP);
	 motor_start_continous(AXIS_X, 1000);
	 motor_start_continous(AXIS_Y, 1000);

	 // Warte auf X_TOP Endschalter
	 while (X_SWITCH_TOP_IN_REG & (1 << X_SWITCH_TOP_PIN)) {
		 // warte, bis Endschalter aktiv (Low durch Pullup)
	 }

	 // Stop Motoren nach Erreichen von X_TOP
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 _delay_ms(200);
 }