/*
 * motor_control.c
 *
 * Created: 15.05.2025 21:45
 * Authors : lukasstrittmatter
 */ 

// -----------------------------------
// --- Icludes -----------------------

#include "config.h"						// generelle Konfig
#include "motor_control.h"				// eigene Header-datei
#include "systemstate.h"				// Systemstate.h für zustandsfnktionen 
#include "laser.h"						// laser.h für laser abstandswert


// -------------------------------------
// --- variables for motor_control.c ---

volatile uint16_t steps_x_done = 0;		// Schrittzähler X	--> muss volatile sein, da durch interrupts jederzeit änderbar
volatile uint16_t steps_y_done = 0;		// Schrittzähler Y
volatile uint16_t steps_z_done = 0;		// Schrittzähler Z

volatile uint16_t steps_x_target = 0;	// Zielwert X für Schrittvergleich  
volatile uint16_t steps_y_target = 0;	// Zielwert X für Schrittvergleich
volatile uint16_t steps_z_target = 0;	// Zielwert X für Schrittvergleich

volatile uint16_t actual_steps_x = 0;	// aktueller Schrittwert X --> Koordinatensystem
volatile uint16_t actual_steps_y = 0;	// aktueller Schrittwert Y --> Koordinatensystem
volatile uint16_t actual_steps_z = 0;	// aktueller Schrittwert Z 


// -------------------------------------
// --- FUNCTIONS -----------------------

// act_Pos_x(): gibt den aktuellen Schrittwert von Koordinate X zurück
uint16_t act_Pos_x(void) {				
	return actual_steps_x;				// Ausgabe der Variable
}

// act_Pos_y(): gibt den aktuellen Schrittwert von Koordinate Y zurück
uint16_t act_Pos_y(void) {				
	return actual_steps_y;				// Ausgabe der Variable
}

// act_Pos_z(): gibt den aktuellen Schrittwert von Koordinate Z zurück
uint16_t act_Pos_z(void) {				
	return actual_steps_z;				// Ausgabe der Variable
}

// motor_init(): initialisiert die Anschlusspins aller Motoren als Ausgänge, 
// ruft die FreigabeFunktion der Motoren auf und setzt eine initiale Richtung für jeden Motor
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
	motor_enable(AXIS_X);				//enable Axis X
	motor_enable(AXIS_Y);				//enable Axis Y
	motor_enable(AXIS_Z);				//enable Axis Z

	//set Diretion to all Axis
	motor_set_direction(AXIS_X, DIR_CW);	//set Direction Axis X to CW
	motor_set_direction(AXIS_Y, DIR_CW);	//set Direction Axis Y to CW
	motor_set_direction(AXIS_Z, DIR_CW);	//set Direction Axis Z to CW
}

// motor_enable(AXIS_X): setzt Motorfreigabe für die entsprechende Achse
// ACHTUNG ACHSE Z IST HIGHT ACTVE
void motor_enable(uint8_t axis) {
	switch (axis) {
		case AXIS_X: EN_X_PORT &= ~(1 << EN_X_PIN); break;	//SET EN_X_PIN to 0 - low active
		case AXIS_Y: EN_Y_PORT &= ~(1 << EN_Y_PIN); break;	//SET EN_Y_PIN to 0 - low active
		case AXIS_Z: EN_Z_PORT |= (1 << EN_Z_PIN); break;	//SET EN_Z_PIN to 1 - high active
	}
}

// motor_disable(AXIS_X): setzt die Motorfreigabe für die entsprechende Achse zurück
// ACHTUNG ACHSE Z IST HIGHT ACTVE
void motor_disable(uint8_t axis) {
	switch (axis) {
		case AXIS_X: EN_X_PORT |= (1 << EN_X_PIN); break;	//SET EN_X_PIN to 1
		case AXIS_Y: EN_Y_PORT |= (1 << EN_Y_PIN); break;	//SET EN_Y_PIN to 1
		case AXIS_Z: EN_Z_PORT &= ~(1 << EN_Z_PIN); break;	//SET EN_Z_PIN to 0
	}
}

// motot_set_direction(AXIS_X,DIR_CW): für die entsprechende Achse wird die Drehrichtung festgelegt. DIR_CW oder DIR_CCW
void motor_set_direction(uint8_t axis, uint8_t direction) {
	switch (axis) {
		// X-Achse
		case AXIS_X:									
		if (direction) DIR_X_PORT |= (1 << DIR_X_PIN);	//SET DIR_X_PIN to 1 if DIR_CCW
		else DIR_X_PORT &= ~(1 << DIR_X_PIN);			//SET DIR_X_PIN to 0 if DIR_CW
		break; //abbruch Schleife	
		// Y-Achse
		case AXIS_Y:
		if (direction) DIR_Y_PORT |= (1 << DIR_Y_PIN);	//SET DIR_Y_PIN to 1 if DIR_CCW
		else DIR_Y_PORT &= ~(1 << DIR_Y_PIN);			//SET DIR_Y_PIN to 0 if DIR_CW
		break; //abbruch Schleife	
		// Z-Achse
		case AXIS_Z:
		if (direction) DIR_Z_PORT |= (1 << DIR_Z_PIN);	//SET DIR_Z_PIN to 1 if DIR_CCW
		else DIR_Z_PORT &= ~(1 << DIR_Z_PIN);			//SET DIR_Z_PIN to 0 if DIR_CW
		break; //abbruch Schleife	
	}
}


//motor_init_timer: initialisiert die verwendbaren Timer für die Achsen.
//Für jede Achse soll eine Taktfreqeunz am jeweiligen Pin-Ausgang mit dem CTC Mode der Timer umgesetzt werden
//Die Schritte werden mit einem Auslösenden Interrupt on CompareMatch hochgezählt. Es wird für das Zählen der Schritte aller Achsen nur ein Interrupt (von Achse X) verwendet
//Der Timer von Achse X soll also durchlaufen, damit ihn jede Achse immer zum Zählen der Schritte verwenden kann
//Damit der Ausgangspin von Achse X dabei nicht dauerhaft Toggelt, wird er für den Stillstand als normaler Port initialisiert.
void motor_init_timer(void) {
	// --- Timer X: Timer3, OC3B (PE4) ---
	TCCR3A &= ~((1 << COM3B1) | (1 << COM3B0));		// OC3B disconnected (normal port operation)
	//STEP_X_OC_REG |= (1 << STEP_X_OC_BIT);		// Toggle OC3B on compare match - set COM3B0 in TCCR3A
	STEP_X_TCCRB_REG |= (1 << WGM32);				// set CTC - Mode (Clear Timer on Compare Match) - set WGM32 in TCCR3B
	STEP_X_OCR = 100;								// set init Value for OCR3A --> Target Value for Compare Match
	STEP_X_TIMSK_REG |= (1 << STEP_X_OCIE_BIT);     // activate OCIE3B bit in TIMSK3 Register to activate Interrupt on Compare Match
	
	// --- Timer Y: Timer1, OC1B (PB6) ---
	STEP_Y_OC_REG |= (1 << STEP_Y_OC_BIT);			 // Toggle OC1B on compare match-- setze COM1B= in TCCR1A
	STEP_Y_TCCRB_REG |= (1 << WGM12);                // CTC-Modus: setze WGM12 in TCCR1B
	STEP_Y_OCR = 100;								// set init Value for OCR1A --> Target Value for Compare Match
	//STEP_Y_TIMSK_REG |= (1 << STEP_Y_OCIE_BIT);	// only use Timer3 for Interrupts --> dont activate Interrupt for Timer 1
	
	// --- Timer Z: Timer4, OC4B (PH4) ---
	STEP_Z_OC_REG |= (1 << STEP_Z_OC_BIT);			// Toggle OC4B on compare match
	STEP_Z_TCCRB_REG |= (1 << WGM42);               // CTC-Modus: WGM42 = 1 in TCCR4B
	STEP_Z_OCR = 100;								// set init Value for OCR4A --> Target Value for Compare Match
	STEP_Z_TIMSK_REG |= (1 << STEP_Z_OCIE_BIT);		// only use Timer 3 for Interrupt --> dont activate Interrupt for Timer 4

}

//limit switch_init: initialisiere alle Pins der Endlagensensoren als Eingang und setze den interenen Pull-Up-Widerstand
void limit_switch_init(void) {
	
	// X-BOTTOM
	X_SWITCH_BOTTOM_DDR &= ~(1 << X_SWITCH_BOTTOM_PIN);   // Set as input
	X_SWITCH_BOTTOM_PORT |= (1 << X_SWITCH_BOTTOM_PIN);   // Enable internal pull-up

	// X-TOP
	X_SWITCH_TOP_DDR &= ~(1 << X_SWITCH_TOP_PIN);		// Set as input
	X_SWITCH_TOP_PORT |= (1 << X_SWITCH_TOP_PIN);		// Enable internal pull-up

	// X-LEFT
	Y_SWITCH_LEFT_DDR &= ~(1 << Y_SWITCH_LEFT_PIN);		// Set as input
	Y_SWITCH_LEFT_PORT |= (1 << Y_SWITCH_LEFT_PIN);		// Enable internal pull-up

	// X-RIGHT
	Y_SWITCH_RIGHT_DDR &= ~(1 << Y_SWITCH_RIGHT_PIN);	// Set as input
	Y_SWITCH_RIGHT_PORT |= (1 << Y_SWITCH_RIGHT_PIN);	// Enable internal pull-up
}

// limit_switch_interrupt_init: initialisiere die Interrupts für Endlagensesnoren
void limit_switch_interrupt_init(void) {
	// INTERRUPT BEI FALLING EDGE (Pull-up aktiv, Schalter schließt nach GND)

	// INT0 für PD0  X_SWITCH_TOP
	EXT_INT_CONTROL_REG |= (1 << ISC01);    // ISC01 = 1, ISC00 = 0 --> Interrupt on falling edge
	EXT_INT_MASK_REG |= (1 << INT0);     // INT0 aktivieren

	// INT1 für PD1  Y_SWITCH_LEFT
	EXT_INT_CONTROL_REG |= (1 << ISC11);    // ISC11 = 1, ISC10 = 0 --> Interrupt on falling edge
	EXT_INT_MASK_REG |= (1 << INT1);     // INT1 aktivieren

	// INT2 für PD2  X_SWITCH_BOTTOM
	EXT_INT_CONTROL_REG |= (1 << ISC21);    // ISC21 = 1, ISC20 = 0 --> Interrupt on falling edge
	EXT_INT_MASK_REG |= (1 << INT2);     // INT2 aktivieren

	// INT3 für PD3  Y_SWITCH_RIGHT
	EXT_INT_CONTROL_REG |= (1 << ISC31);    // ISC31 = 1, ISC30 = 0 --> Interrupt on falling edge
	EXT_INT_MASK_REG |= (1 << INT3);     // INT3 aktivieren
}

// limit_switch_interrupt_init: deaktiviere die Interrupts für alle Endlagensensoren
void disable_limit_switch_interrupts(void) {
	EXT_INT_MASK_REG &= ~((1 << INT0) | (1 << INT1) | (1 << INT2) | (1 << INT3));	// dekativiere INT0 - INT3
}



//motor_start_steps(AXIS_X, 1000, 1000): Funktion startet die entsprechende Achse mit der entsprechenden Frequenz 
// Mit den übergebenden Steps wird in der Funktion der Zielwert verdoppellt, sodass dieser in anderen Funktionen verwendet werden kann
// FUNKTION STOPPT DIE ACHSE NICHT BEIM ERREICHEN DES ZIELWERTS
void motor_start_steps(uint8_t axis, uint16_t steps, uint16_t freq_hz) {
	const uint16_t prescaler = 64;			// deklariere Variable für prescaler fest auf 64 --> Frequenzbereich theretisch von 2Hz bis 125000Hz
	uint16_t ocr_val;						// deklariere lokale Variable für zu berechenenden OCR-Valule									

	// set freqeuncy borders
	if (freq_hz < 5) freq_hz = 5;			// Frequenz unter 5 Hz vermeiden 
	if (freq_hz > 2000) freq_hz = 2000;		// Freqeunz über 2000 Hz ebenfalls vermeiden

	// Berechnung des OCR - Bit:
	// OCR = Fcpu/(2*PrescalerValue*fmotor) -1
	// example: for f = 1,2kHz --> 16000000/(2*64*1200)-1 = 100
	// 2UL --> unsigned ling intege
	ocr_val = (F_CPU / (2UL * prescaler * freq_hz)) - 1;

	switch(axis) {
		// X-Achse
		case AXIS_X:
		steps_x_done = 0;												// Rücksetzen der gemachten Schritte für X
		steps_x_target = steps * 2;										// Verdopple die gewünschte Schrittanzahl, da zwei Flanken für ein Schritt gebraucht werden

		// Besonderheit X-Achse: "Toggle OC3B on comparematch" muss bei jedem start gesetzt werden
		STEP_X_OC_REG |= (1 << STEP_X_OC_BIT);							// Toggle OC3B on compare match - set COM3B0 in TCCR3A 
		STEP_X_TCCRB_REG &= ~((1 << CS32) | (1 << CS31) | (1 << CS30)); // stop Timer, to set Velocity
		STEP_X_TCNT = 0;												// Rücksetzen des aktuellen Zählerwerts auf 0
		STEP_X_OCR = ocr_val;											// Übergeben des berechnten OCR-Values
		STEP_X_TCCRB_REG |= (1 << CS31) | (1 << CS30);					// start timer with Prescaler 64 --> Berechnung baut darauf auf
		break;	//abbruch Schleife											
		
		// Y-Achse
		case AXIS_Y:
		steps_y_done = 0;												// Rücksetzen der gemachten Schritte für Y									
		steps_y_target = steps * 2;										// Verdopple die gewünschte Schrittanzahl, da zwei Flanken für ein Schritt gebraucht werden

		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));	// stop Timer, to set Velocity
		STEP_Y_TCNT = 0;												// Rücksetzen des aktuellen Zählerwerts auf 0
		STEP_Y_OCR = ocr_val;											// Übergeben des berechnten OCR-Values
		STEP_Y_TCCRB_REG |= (1 << CS11) | (1 << CS10);					// start timer with Prescaler 64 --> Berechnung baut darauf auf
		break; //abbruch Schleife	
		
		// Z-Achse
		case AXIS_Z:
		steps_z_done = 0;												// Rücksetzen der gemachten Schritte für Z
		steps_z_target = steps * 2;										// Verdopple die gewünschte Schrittanzahl, da zwei Flanken für ein Schritt gebraucht werden

		STEP_Z_TCCRB_REG &= ~((1 << CS42) | (1 << CS41) | (1 << CS40)); // stop Timer, to set Velocity
		STEP_Z_TCNT = 0;												// Rücksetzen des aktuellen Zählerwerts auf 0
		STEP_Z_OCR = ocr_val;											// Übergeben des berechnten OCR-Values
		STEP_Z_TCCRB_REG |= (1 << CS41) | (1 << CS40);					// start timer with Prescaler 64 --> Berechnung baut darauf auf
		
		break; //abbruch Schleife	
		default: //Fallback
		break; //abbruch Schleife	
	}
}

// motor_start_continious(AXIS_X,100): startet eine Achse kontinuierlich ohne ein Zielwert zu berechnen --> Achse muss anschliessend gestoppt werden 
void motor_start_continous(uint8_t axis, uint16_t freq_hz) {
	const uint16_t prescaler = 64;							// deklariere Variable für prescaler fest auf 64 --> Frequenzbereich theretisch von 2Hz bis 125000Hz
	uint16_t ocr_val;

	// set freqeuncy borders
	if (freq_hz < 5) freq_hz = 5;				// Frequenz unter 5 Hz vermeiden 
	if (freq_hz > 2000) freq_hz = 2000;			// Freqeunz über 2000 Hz ebenfalls vermeiden
	

	// Calculation of OCR - Bit:
	// OCR = Fcpu/(2*PrescalerValue*fmotor) -1
	// example: for f = 1,2kHz --> 16000000/(2*64*1200)-1 = 100
	// 2UL --> unsigned ling intege
	ocr_val = (F_CPU / (2UL * prescaler * freq_hz)) - 1;

	switch(axis) {
		case AXIS_X:
		steps_x_done = 0;													// Rücksetzen der gemachten Schritte für X
		
		// Besonderheit X-Achse: "Toggle OC3B on comparematch" muss bei jedem start gesetzt werden
		STEP_X_OC_REG |= (1 << STEP_X_OC_BIT);								// Toggle OC3B on compare match - set COM3B0 in TCCR3A
		STEP_X_TCCRB_REG &= ~((1 << CS32) | (1 << CS31) | (1 << CS30));		// stop Timer, to set Velocity
		STEP_X_TCNT = 0;													// Rücksetzen des aktuellen Timerwerts
		STEP_X_OCR = ocr_val;												// Übergeben des berechneten OCR-Values
		STEP_X_TCCRB_REG |= (1 << CS31) | (1 << CS30);						// start timer with Prescaler 64
		break;

		case AXIS_Y:
		steps_y_done = 0;													// Rücksetzen der gemachten Schritte für Y
		
		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));		// stop Timer, to set Velocity
		STEP_Y_TCNT = 0;													// Rücksetzen des aktuellen Timerwerts
		STEP_Y_OCR = ocr_val;												// Übergeben des berechneten OCR-Values
		STEP_Y_TCCRB_REG |= (1 << CS11) | (1 << CS10);						// start timer with Prescaler 64
		break;

		case AXIS_Z:
		steps_z_done = 0;													// Rücksetzen der gemachten Schritte für Y
		
		STEP_Z_TCCRB_REG &= ~((1 << CS42) | (1 << CS41) | (1 << CS40));		// stop Timer, to set Velocity
		STEP_Z_TCNT = 0;													// Rücksetzen des aktuellen Timerwerts
		STEP_Z_OCR = ocr_val;												// Übergeben des berechneten OCR-Values
		STEP_Z_TCCRB_REG |= (1 << CS41) | (1 << CS40);						// start timer with Prescaler 64
		break;

		default:
		break;
	}
}

// motor_stop(AXIS_X): funktion stoppt den entsprchenden Motor
void motor_stop(uint8_t axis) {
	switch(axis) {
		case AXIS_X:
		// Besonderheit Achse X: Stoppe nicht den Timer, sondern verwende den Pin als normaler Ausgang. 
		TCCR3A &= ~((1 << COM3B1) | (1 << COM3B0));		// OC3B disconnected (normal port operation)
		//STEP_X_TCCRB_REG &= ~((1 << CS32) | (1 << CS31) | (1 << CS30));	don't stop Timer3 --> wird gebraucht für das Zählen der Schritte der anderen Achsen
		//STEP_X_TIMSK_REG &= ~(1 << STEP_X_OCIE_BIT);
		break;

		case AXIS_Y:
		STEP_Y_TCCRB_REG &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // stoppe den Timer 1
		break;

		case AXIS_Z:
		STEP_Z_TCCRB_REG &= ~((1 << CS42) | (1 << CS41) | (1 << CS40)); // stoppe den Timer 4
		break;
	}
}

 //--- ISR: Interrupts on CompareMatch from the Timers ---
 // Interrupt von Timer 3 --> zählt Schritte für alle Motoren hoch
 ISR(TIMER3_COMPB_vect) {
	 steps_x_done++;	// zähle steps_x_done hoch
	 steps_y_done++;	// zähle steps_y_done hoch
	// steps_z_done++;	// zähle steps_z_done hoch
 }

//--> die anderen Interrupts nicht mehr verwenden
 //ISR(TIMER1_COMPB_vect) {
	 //steps_y_done++; 
 //}
//
 ISR(TIMER4_COMPB_vect) {
	 steps_z_done++; 
 }
 
 // --- ISR: Interrupts from Limit Switches
 // Stoppe alle Achsen wenn ein Interrupt ausgelöst wird
 ISR(INT0_vect) {
	 motor_stop(AXIS_X);	// stoppe Achse X
	 motor_stop(AXIS_Y);	// stoppe Achse Y
	 motor_stop(AXIS_Z);	// stoppe Achse Z
 }

 ISR(INT1_vect) {
	 motor_stop(AXIS_X);	// stoppe Achse X
	 motor_stop(AXIS_Y);	// stoppe Achse Y
	 motor_stop(AXIS_Z);	// stoppe Achse Z
 }

 ISR(INT2_vect) {
	 motor_stop(AXIS_X);	// stoppe Achse X
	 motor_stop(AXIS_Y);	// stoppe Achse Y
	 motor_stop(AXIS_Z);	// stoppe Achse Z
 }

 ISR(INT3_vect) {
	 motor_stop(AXIS_X);	// stoppe Achse X
	 motor_stop(AXIS_Y);	// stoppe Achse Y
	 motor_stop(AXIS_Z);	// stoppe Achse Z
 }
 
 // set_X_Y_direction(DIR_X_TOP): Diese Funktion setzt je nach gewünschter Bewegungsrichtung nach X oder Y der "Stiftkoordinate" für die motoren X und Y die Drehrichtungen. 
 // Für jede Bewegungsrichtung nach X oder Y in dem "Stiftkoordinaten"-System werden beide Motoren gebraucht.
 void set_X_Y_direction(uint8_t direction) {
	 switch (direction) {
		 case DIR_X_TOP: // X negativ  X = CCW, Y = CCW
		 motor_set_direction(AXIS_X, DIR_CCW);		// set Direction from Axis X to CCW
		 motor_set_direction(AXIS_Y, DIR_CCW);		// set Direction from Axis Y to CCW
		 break;

		 case DIR_X_BOTTOM: // X positiv  X = CW, Y = CW
		 motor_set_direction(AXIS_X, DIR_CW);		// set Direction from Axis X to CW
		 motor_set_direction(AXIS_Y, DIR_CW);		// set Direction from Axis Y to CW
		 break;
		 
		 case DIR_Y_LEFT: // Y positiv  X = CW, Y = CCW
		 motor_set_direction(AXIS_X, DIR_CW);		// set Direction from Axis X to CW
		 motor_set_direction(AXIS_Y, DIR_CCW);		// set Direction from Axis Y to CCW
		 break;

		 case DIR_Y_RIGHT: // Y negativ  X = CCW, Y = CW
		 motor_set_direction(AXIS_X, DIR_CCW);		// set Direction from Axis X to CCW
		 motor_set_direction(AXIS_Y, DIR_CW);		// set Direction from Axis Y to CW
		 break;
		 default:
		 break;
	 }
 }
 
 // move_to_position_steps_xy(1000,1000,1000): Mit dieser Funktion kann im "Stiftkoordinatensystem" ein Zielposition für X und Y und eine Sollgeschwindigkeit in Hz vorgegeben werden. Die Motoren werden darin entsprechend kommandiert, bis der Stift die X und Y Position erreicht hat. 
void move_to_position_steps_xy(int32_t target_steps_x, int32_t target_steps_y, uint16_t speed_hz)
{
	int32_t delta_steps_x = target_steps_x - actual_steps_x;		// berechene Positionsunterschied von der Zielposition zur aktuellen Position in X
	int32_t delta_steps_y = target_steps_y - actual_steps_y;		// berechene Positionsunterschied von der Zielposition zur aktuellen Position in Y

	// ========================
	// --- X-Achse bewegen ---
	// ========================
	if (delta_steps_x != 0) {									// checke ob X-Richtung verfahren werden muss
		if (delta_steps_x > 0) {								// wenn ja, dann chceke ob x positiv verfahren werden muss
			// Positive X-Richtung 
			set_X_Y_direction(DIR_X_BOTTOM);					// setzte Richtung der Motoren X und Y für positive X -Richtung (runter)
			} else {
			// Negative X-Richtung 
			set_X_Y_direction(DIR_X_TOP);						// setzte Richtung der Motoren X und Y für negative X -Richtung (hoch)
			delta_steps_x = -delta_steps_x;						// positiv machen für Bewegung
		}

		steps_x_done = 0;		//setzte aktuell gemachte Schritte zurück auf 0 für X-Achse
		steps_y_done = 0;		//setzte aktuell gemachte Schritte zurück auf 0 für X-Achse

		motor_start_steps(AXIS_X, delta_steps_x, speed_hz); //starte Motor X mit Schrittanzahl in X-Richtung
		motor_start_steps(AXIS_Y, delta_steps_x, speed_hz);  //starte Motor Y mit Schrittanzahl in X-Richtung

		while (steps_x_done < steps_x_target || steps_y_done < steps_y_target);  //warten bis Motoren ihre Schritte gemacht haben

		motor_stop(AXIS_X);		// Stoppe Motor X
		motor_stop(AXIS_Y);		// Stoppe Motor Y
		
		actual_steps_x = target_steps_x;	// Schreibe den gewünschten Wert von der StiftKoordinate X auf den aktuellen Wert von X-Koordinate
	}

	// ========================
	// --- Y-Achse bewegen ---
	// ========================
	if (delta_steps_y != 0) {								// checke ob in Y-Richtung verfahren werden muss
		if (delta_steps_y > 0) {							// checke ob positoiv verfahren werden muss
			// Positive Y-Richtung 
			set_X_Y_direction(DIR_Y_LEFT);					// setzte X-Y Diection
			} else {
			// Negative Y-Richtung 
			set_X_Y_direction(DIR_Y_RIGHT);					// setzte X-Y Diection
			delta_steps_y = -delta_steps_y;					// positiv machen für Bewegung
		}

		steps_x_done = 0;								//rücksetzen der gemachten Schritte
		steps_y_done = 0;								//rücksetzen der gemachten Schritte
		
		motor_start_steps(AXIS_X, delta_steps_y, speed_hz);		// starte Motor X mit den Schritten in Y-Richtung
		motor_start_steps(AXIS_Y, delta_steps_y, speed_hz);		// starte Motor Y mit den Schritten in Y-Richtung

		while (steps_x_done < steps_x_target || steps_y_done < steps_y_target);  // warten bis fertig
		
		motor_stop(AXIS_X);		// stoppe Motor X
		motor_stop(AXIS_Y);		// stoppe Motor Y
		
		actual_steps_y = target_steps_y;		// schriebe den Zielwetr für Stiftkoordinate auf den Aktuellen Wert für Y
		_delay_ms(20);							// Daempfung
	}
}

// move_to_position_steps_z(1000,1000): Diese Funktion kommandiert die Z-Achse auf eine gewünschte Schrittposition mit einer gewünshten Geschwindigkeit in Hz
void move_to_position_steps_z(int32_t target_steps_z, uint16_t speed_hz)
{
	int32_t delta_steps_z = target_steps_z - actual_steps_z;	//berechne den aktuellen Positionsunterschied zwischen Ist und Sollposition 
	
	// ========================
	// --- Z-Achse bewegen ---
	// ========================
	if (delta_steps_z != 0) {						// gibt es einen Positionsunterschied?
		if (delta_steps_z > 0) {					// ist er positiv
			// Positive Z-Richtung --> CW
			motor_set_direction(AXIS_Z, DIR_CW);	// setzte CW als positive Z Richtung
			} else {
			// Negative Z-Richtung --> CCW
			motor_set_direction(AXIS_Z, DIR_CCW);	// setzte CCW als negative Z-Richtung
			delta_steps_z = -delta_steps_z;  // positiv machen für Bewegung
		}

		steps_z_done = 0;				// Rücksetzen der gemachten Schritte in Z

		motor_start_steps(AXIS_Z, delta_steps_z, speed_hz);	// starte den Motor  mit der berechneten Schrittdifferenz und er gewünschten Geschwindigkeit

		while (steps_z_done < steps_z_target);  // warten bis fertig
		motor_stop(AXIS_Z);						// Z-Achse Stoppen
		actual_steps_z = target_steps_z;		// aktuelle Position mit Zielposition überschrieben 
		_delay_ms(20);							// Daempfung
	}
}


//move_to_position_steps_xz(1000,1000,1000): Funktion verfährt in X-Richtung (Stiftkoordinate) und Z-Richtung gleichzeitig, mit gewünschter Zielposition für X und Z - Koordinate und gewünschter Geschwindigkeit in Hz
void move_to_position_steps_xz(int32_t target_steps_x, int32_t target_steps_z, uint16_t speed_hz)
{
	int32_t delta_steps_x = target_steps_x - actual_steps_x;	//berechne Positionsunterschied in X
	int32_t delta_steps_z = target_steps_z - actual_steps_z;	// berechene Positionsunterschied in Z
	
	// ================================
	// --- X-Achse und Z-Achse bewegen
	// ================================
	if ((delta_steps_x != 0) && (delta_steps_z != 0)) {			// führe Bewegung nur aus wenn beide Richtungen Positionsunterschiede aufweisen 
		
		// Richtung für X setzen
		if (delta_steps_x > 0) {								// checke erst ob X positiv verfahren muss 
			set_X_Y_direction(DIR_X_BOTTOM);					// setzte Positive Richtung für X
			} else {
			set_X_Y_direction(DIR_X_TOP);						// setzte negative Richtung für X
			delta_steps_x = -delta_steps_x;						// Positionsunterscheid positiv machen für Bewegung
		}

		// Richtung für Z setzen
		if (delta_steps_z > 0) {								// checke ob ZAche positiv fahren muss
			motor_set_direction(AXIS_Z, DIR_CW);				// Positiv -> CW
			} else {
			motor_set_direction(AXIS_Z, DIR_CCW);				// negativ in Z --> CCW
			delta_steps_z = -delta_steps_z;						// Schritte positiv machen 
		}

		// alle Schrittzähler zurücksetzen
		steps_x_done = 0;
		steps_y_done = 0;
		steps_z_done = 0;

		// alle Motoren starten 
		motor_start_steps(AXIS_X, delta_steps_x, speed_hz);		// starte X-Achse mit positionsunterschied in X
		motor_start_steps(AXIS_Y, delta_steps_x, speed_hz);		// starte Y-Achse mit positionsunterschied in X
		motor_start_steps(AXIS_Z, delta_steps_z, speed_hz/4);   // starte Z-Achse mit positionsunterschied in Z und viermal langsamer geschwidnigkeit für steilere Flanke

		
		while (1) {
			if (steps_x_done >= steps_x_target) motor_stop(AXIS_X);
			if (steps_y_done >= steps_y_target) motor_stop(AXIS_Y);
			if (steps_z_done >= steps_z_target) motor_stop(AXIS_Z);

			if (steps_x_done >= steps_x_target &&
			steps_y_done >= steps_y_target &&
			steps_z_done >= steps_z_target) {
				break;
			}
		}
		
		actual_steps_x = target_steps_x;	//setzte die Aktuelle position von X (Zielwert als aktuelle Position)
		actual_steps_z = target_steps_z;	// setzte aktuelle Position von Z (Zielwert von Z)
		_delay_ms(20);						// Daempfung
	}
}
 
 //start_XY_refernce(): Startet die Refernzfahrt für ALLE Motoren - XY zuerst nach Rechts, dann nach Oben
 void start_XY_reference(void) {
	 // Set direction: move Y_RIGHT first
	 set_X_Y_direction(DIR_Y_RIGHT);		// setzte Y_Drehrichtung nach Rechts
	 motor_start_continous(AXIS_X, 1000);	// beide Motoren starten mit 1000 Hz
	 motor_start_continous(AXIS_Y, 1000);	// beide Motoren starten mit 1000 Hz
	
	//start Z
	 motor_set_direction(AXIS_Z, DIR_CW);	// Drehrichtung für Z = CW
	 motor_start_continous(AXIS_Z, 500);	// starte ZMotor mit 500Hz

	 // Warte auf Y_RIGHT Endschalter
	 while (Y_SWITCH_RIGHT_IN_REG & (1 << Y_SWITCH_RIGHT_PIN)) {
		 // warte, bis Endschalter aktiv (Low durch Pullup)
	 }

	 // Stopp von allem Motoren nach Erreichen von Y_RIGHT
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 motor_stop(AXIS_Z);
	 
	 _delay_ms(200);  // mechanische Dämpfung

	// Nun Fahrt nach Oben bis Endschalter
	 set_X_Y_direction(DIR_X_TOP);	// Set direction: move X_TOP		
	 motor_start_continous(AXIS_X, 1000);	// starte X-Achse kontinuierlich mit 1000Hz
	 motor_start_continous(AXIS_Y, 1000);	// starte Y-Achse kontinuierlich mit 1000Hz

	 // Warte auf X_TOP Endschalter
	 while (X_SWITCH_TOP_IN_REG & (1 << X_SWITCH_TOP_PIN)) {
		 // warte, bis Endschalter aktiv (Low durch Pullup)
	 }

	 // Stop Motoren X,Y nach Erreichen von X_TOP
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 
	 // REFERNCE CONTROL FOR SYSTEMSTATE
	 set_referenced(1);			//set reference state to 1 
	 request_reference_start(0); //reset request bit
	 
	 actual_steps_x = 0;	//set actual steps to 0
	 actual_steps_y = 0;	//set actual steps to 0
	 actual_steps_z = 0;	//set actual steps to 0
	 _delay_ms(200);	// mechanische Daempfung
	
 }
 
 //move_Y_left_until_laser(20,1000): ´Der Stift fährt so lange nach links bis der Laser den Wert von "laser_target_mm" erreicht. Dann werden die Motoren gestoppt
 void move_Y_left_until_laser(uint16_t laser_target_mm, uint16_t speed_hz)
 {
	 // Richtung setzen: Y-Richtung links X = CW, Y = CCW
	 set_X_Y_direction(DIR_Y_LEFT);

	 // Starte beide Motoren kontinuierlich
	 motor_start_continous(AXIS_X, speed_hz);
	 motor_start_continous(AXIS_Y, speed_hz);

	 while (laser_read() > laser_target_mm) {		//mit laser_read() Funktion bekommt man aktuellen Laser wert
		//wait until laser is smaller than target
	 }

	 // Stoppe beide Motoren
	 motor_stop(AXIS_X);
	 motor_stop(AXIS_Y);
	 
	actual_steps_y = actual_steps_y + (steps_y_done/2);	// setzte aktuelleb Y-Positionwert --> gemachte Schritte/2, da immer die doppelten Schritte gezält werden
	 _delay_ms(200);	// mechanische Dämpfung
	 
 }
 
 //--------------------------------
 //--- AXIS CONTROL FOR LETTERS ---
 
 //move_pen_backward(): Der Stift wird um 40 Schritte vom Blatt weg gefahren
 void move_pen_backward() {
	 move_to_position_steps_xy(actual_steps_x, actual_steps_y - 40, 100);	//start X,Y mit Zielwerten für Y-Offset von -40 Schritten
 }

 //move_pen_backward(): Der Stift wird um 40 Schritte zum Blatt hin gefahren
 void move_pen_forward() {
	 move_to_position_steps_xy(actual_steps_x, actual_steps_y + 40, 100); //start X,Y mit Zielwerten für Y-Offset von 40 Schritten
 }

// move_X_relative(100, 1000): Der Stift wird um eine gewünschte Schrittanzahl relativ nach X verfahren
 void move_X_relative(int32_t rel_steps, uint16_t speed) {
	 int32_t target_x = actual_steps_x + rel_steps;					// berechne neuen Zielwert. relative Schritte zur aktuellen Position
	 move_to_position_steps_xy(target_x, actual_steps_y, speed);	// starte X,Y Motoren mit neuem Zielwert für X
 }

// move_Z_relative(100,1000): Die Z-Achse wird um gewünschte Schrittanzahl relativ zur aktuellen z-Position verfahren 
 void move_Z_relative(int32_t rel_steps, uint16_t speed) {
	 int32_t target_z = actual_steps_z + rel_steps;				// berenechne neuen Zielwert
	 move_to_position_steps_z(target_z, speed);					// kommandiere Z-Achse
 }

// move_XZ_diagonal_relative(100,25,1000); Die Z- und X-Achse werden um die jeweiligen relativen Werte verfahren. Der relative Wert von Z muss viermal kleiner sein als der von X
 void move_XZ_diagonal_relative(int32_t rel_steps_x, int32_t rel_steps_z, uint16_t speed) {
	 int32_t target_x = actual_steps_x + rel_steps_x;		// berechne Zielwert X
	 int32_t target_z = actual_steps_z + rel_steps_z;		// berechne Zielwert Z
	 move_to_position_steps_xz(target_x, target_z, speed);	// komanndiere alle motoren 
 }
 
