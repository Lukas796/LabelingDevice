/*
 * motor_control.c
 *
 * Created: 15.05.2025 21:45
 * Authors : lukasstrittmatter
 */ 

#include "motor_control.h"
#include "config.h"
#include "systemstate.h"

// Schrittzähler je Motor
volatile uint16_t steps_x_done = 0;
volatile uint16_t steps_y_done = 0;
volatile uint16_t steps_z_done = 0;

volatile uint16_t steps_x_target = 0;
volatile uint16_t steps_y_target = 0;
volatile uint16_t steps_z_target = 0;

volatile uint16_t actual_steps_x = 0;
volatile uint16_t actual_steps_y = 0;
volatile uint16_t actual_steps_z = 0;

static uint16_t testpos = 130;
static uint16_t testpos2 = 2030;

uint16_t act_Pos_x(void) {
	return actual_steps_x;
}

uint16_t act_Pos_y(void) {
	return actual_steps_y;
}

uint16_t act_Pos_z(void) {
	return actual_steps_z;
}

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
	
	//enable all Axis 
	motor_enable(AXIS_X);
	motor_enable(AXIS_Y);
	motor_enable(AXIS_Z);

	//set Diretion to all Axis
	motor_set_direction(AXIS_X, DIR_CW);
	motor_set_direction(AXIS_Y, DIR_CW);
	motor_set_direction(AXIS_Z, DIR_CW);
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

void limit_switch_interrupt_init(void) {
	// FALLING EDGE (Pull-up aktiv, Schalter schließt nach GND)

	// INT0 ? PD0 ? X_SWITCH_TOP
	EXT_INT_CONTROL_REG |= (1 << ISC01);    // ISC01 = 1, ISC00 = 0 ? falling edge
	EXT_INT_MASK_REG |= (1 << INT0);     // INT0 aktivieren

	// INT1 ? PD1 ? Y_SWITCH_LEFT
	EXT_INT_CONTROL_REG |= (1 << ISC11);    // ISC11 = 1, ISC10 = 0 ? falling edge
	EXT_INT_MASK_REG |= (1 << INT1);     // INT1 aktivieren

	// INT2 ? PD2 ? X_SWITCH_BOTTOM
	EXT_INT_CONTROL_REG |= (1 << ISC21);    // ISC21 = 1, ISC20 = 0 ? falling edge
	EXT_INT_MASK_REG |= (1 << INT2);     // INT2 aktivieren

	// INT3 ? PD3 ? Y_SWITCH_RIGHT
	EXT_INT_CONTROL_REG |= (1 << ISC31);    // ISC31 = 1, ISC30 = 0 ? falling edge
	EXT_INT_MASK_REG |= (1 << INT3);     // INT3 aktivieren
}

void disable_limit_switch_interrupts(void) {
	EXT_INT_MASK_REG &= ~((1 << INT0) | (1 << INT1) | (1 << INT2) | (1 << INT3));
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
		//STEP_X_TIMSK_REG |= (1 << STEP_X_OCIE_BIT);    // INTERRUPT AKTIVIEREN
		STEP_X_TCCRB_REG |= (1 << CS31) | (1 << CS30); // start timer with Prescaler 64
		
		if (steps_x_done >= steps_x_target) {
			motor_stop(AXIS_X);
		}
		break;

		case AXIS_Y:
		steps_y_done = 0;
		steps_y_target = steps * 2;

		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		STEP_Y_TCNT = 0;
		STEP_Y_OCR = ocr_val;
		//STEP_Y_TIMSK_REG |= (1 << STEP_Y_OCIE_BIT);    // INTERRUPT AKTIVIEREN
		STEP_Y_TCCRB_REG |= (1 << CS11) | (1 << CS10);
		
		if (steps_y_done >= steps_y_target) {
			motor_stop(AXIS_Y);
		}
		break;

		case AXIS_Z:
		steps_z_done = 0;
		steps_z_target = steps * 2;

		STEP_Z_TCCRB_REG &= ~((1 << CS42) | (1 << CS41) | (1 << CS40));
		STEP_Z_TCNT = 0;
		STEP_Z_OCR = ocr_val;
		
		STEP_Z_TCCRB_REG |= (1 << CS41) | (1 << CS40);
		
		if (steps_z_done >= steps_z_target) {
			motor_stop(AXIS_Z);
		}
		
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
		steps_x_done = 0;
		
		STEP_X_TCCRB_REG &= ~((1 << CS32) | (1 << CS31) | (1 << CS30)); // stop Timer, to set Velocity
		STEP_X_TCNT = 0;
		STEP_X_OCR = ocr_val;
		STEP_X_TCCRB_REG |= (1 << CS31) | (1 << CS30); // start timer with Prescaler 64
		break;

		case AXIS_Y:
		steps_y_done = 0;
		
		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		STEP_Y_TCNT = 0;
		STEP_Y_OCR = ocr_val;
		STEP_Y_TCCRB_REG |= (1 << CS11) | (1 << CS10);
		break;

		case AXIS_Z:
		steps_z_done = 0;
		
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
		//STEP_X_TIMSK_REG &= ~(1 << STEP_X_OCIE_BIT);
		break;

		case AXIS_Y:
		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		//STEP_Y_TIMSK_REG &= ~(1 << STEP_Y_OCIE_BIT);
		break;

		case AXIS_Z:
		STEP_Z_TCCRB_REG &= ~((1 << CS42) | (1 << CS41) | (1 << CS40));
		//STEP_Z_TIMSK_REG &= ~(1 << STEP_Z_OCIE_BIT);
		break;
	}
}

 //--- ISR: Interrupts on CompareMatch from the Timers ---
 ISR(TIMER3_COMPB_vect) {
	 steps_x_done++;
 }

 ISR(TIMER1_COMPB_vect) {
	 steps_y_done++; 
 }

 ISR(TIMER4_COMPB_vect) {
	 steps_z_done++; 
 }
 
 // Interrupts from Limit Switches
 ISR(INT0_vect) {
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 motor_stop(AXIS_Z);
	 //set_referenced(0); //set reference to 0
 }

 ISR(INT1_vect) {
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 motor_stop(AXIS_Z);
	//set_referenced(0); //set reference to 0
 }

 ISR(INT2_vect) {
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 motor_stop(AXIS_Z);
	 //set_referenced(0); //set reference to 0
 }

 ISR(INT3_vect) {
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 motor_stop(AXIS_Z);
	 //set_referenced(0); //set reference to 0
 }
 
 void set_X_Y_direction(uint8_t direction) {
	 switch (direction) {
		 case DIR_X_TOP: // X positiv  X = CCW, Y = CCW
		 motor_set_direction(AXIS_X, DIR_CCW);
		 motor_set_direction(AXIS_Y, DIR_CCW);
		 break;

		 case DIR_X_BOTTOM: // X negativ  X = CW, Y = CW
		 motor_set_direction(AXIS_X, DIR_CW);
		 motor_set_direction(AXIS_Y, DIR_CW);
		 break;
		 
		 case DIR_Y_LEFT: // Y positiv  X = CW, Y = CCW
		 motor_set_direction(AXIS_X, DIR_CW);
		 motor_set_direction(AXIS_Y, DIR_CCW);
		 break;

		 case DIR_Y_RIGHT: // Y negativ  X = CCW, Y = CW
		 motor_set_direction(AXIS_X, DIR_CCW);
		 motor_set_direction(AXIS_Y, DIR_CW);
		 break;
		 default:
		 break;
	 }
 }
 
void move_to_position_steps_xy(int32_t target_steps_x, int32_t target_steps_y, uint16_t speed_hz)
{
	int32_t delta_steps_x = target_steps_x - actual_steps_x;
	int32_t delta_steps_y = target_steps_y - actual_steps_y;

	// ========================
	// --- X-Achse bewegen ---
	// ========================
	if (delta_steps_x != 0) {
		if (delta_steps_x > 0) {
			// Positive X-Richtung ? beide Motoren gleichsinnig (z.?B. CW)
			set_X_Y_direction(DIR_X_BOTTOM);
			} else {
			// Negative X-Richtung ? beide Motoren gleichsinnig (z.?B. CCW)
			set_X_Y_direction(DIR_X_TOP);
			delta_steps_x = -delta_steps_x;  // positiv machen für Bewegung
		}

		steps_x_done = 0;
		steps_y_done = 0;

		motor_start_steps(AXIS_X, delta_steps_x, speed_hz);
		motor_start_steps(AXIS_Y, delta_steps_x, speed_hz);

		while (steps_x_done < steps_x_target || steps_y_done < steps_y_target);  // warten bis fertig

		actual_steps_x = target_steps_x;
	}

	// ========================
	// --- Y-Achse bewegen ---
	// ========================
	if (delta_steps_y != 0) {
		if (delta_steps_y > 0) {
			// Positive Y-Richtung ? Motoren gegensinnig
			set_X_Y_direction(DIR_Y_LEFT);
			} else {
			// Negative Y-Richtung ? Motoren gegensinnig
			set_X_Y_direction(DIR_Y_RIGHT);
			delta_steps_y = -delta_steps_y;
		}

		steps_x_done = 0;
		steps_y_done = 0;
		
		motor_start_steps(AXIS_X, delta_steps_y, speed_hz);
		motor_start_steps(AXIS_Y, delta_steps_y, speed_hz);

		while (steps_x_done < steps_x_target || steps_y_done < steps_y_target);  // warten bis fertig

		actual_steps_y = target_steps_y;
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
	 
	 set_referenced(1);			//set reference state to1
	 request_reference_start(0); //reset request bit
	 actual_steps_x = 0;	//set actual steps to 0
	 actual_steps_y = 0;	//set actual steps to 0
	 _delay_ms(200);
	
 }
 
 
 void move_Y_left_until_laser(uint16_t laser_target_mm, uint16_t speed_hz)
 {
	 // Richtung setzen: Y-Richtung links ? X = CW, Y = CCW
	 set_X_Y_direction(DIR_Y_LEFT);


	 // Starte beide Motoren kontinuierlich
	 motor_start_continous(AXIS_X, speed_hz);
	 motor_start_continous(AXIS_Y, speed_hz);

	 while (laser_read() > laser_target_mm) {
		//wait until laser is smaller than target
	 }

	 // Stoppe beide Motoren
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 
	actual_steps_y = actual_steps_y + steps_y_done;
	 _delay_ms(200);
	 
 }
 
 // Axis Control for letters
 void move_pen_backward() {
	
	 move_to_position_steps_xy(actual_steps_x, actual_steps_y - 20, 100);
 }

 void move_pen_forward() {
	 move_to_position_steps_xy(actual_steps_x, actual_steps_y + 20, 100);
 }

 void move_X(int32_t steps, uint16_t speed) {
	 move_to_position_steps_xy(actual_steps_x + steps, actual_steps_y, speed);
 }

 void move_Z(int32_t steps, uint16_t speed) {
	 motor_set_direction(AXIS_Z, (steps > 0) ? DIR_CW : DIR_CCW);
	 motor_start_steps(AXIS_Z, abs(steps), speed);
	 while (steps_z_done < steps_z_target);
	 actual_steps_z += steps;
 }

 void move_XZ_diagonal(int32_t dx, int32_t dz, uint16_t speed) {
	 motor_set_direction(AXIS_X, dx > 0 ? DIR_CW : DIR_CCW);
	 motor_set_direction(AXIS_Z, dz > 0 ? DIR_CW : DIR_CCW);
	 motor_start_steps(AXIS_X, abs(dx), speed);
	 motor_start_steps(AXIS_Z, abs(dz), speed);
	 while (steps_x_done < steps_x_target || steps_z_done < steps_z_target);
	 actual_steps_x += dx;
	 actual_steps_z += dz;
 }