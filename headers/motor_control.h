/*
 * motor_control.h
 *
 * Created: 15.05.2025 21:58:19
 *  Author: lukasstrittmatter
 */ 


#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include "config.h" // need to be first

//-----------------------
//--- FUNKTIONEN -------- 
// genauer in .c beschrieben
uint16_t act_Pos_x(void);
uint16_t act_Pos_y(void);
uint16_t act_Pos_z(void);
void motor_init(void);
void motor_init_timer(void);
void limit_switch_init(void); 
void limit_switch_interrupt_init(void);
void disable_limit_switch_interrupts(void);
void motor_enable(uint8_t axis);
void motor_disable(uint8_t axis);
void motor_set_direction(uint8_t axis, uint8_t direction);
void motor_step(uint8_t axis);
void motor_start_steps(uint8_t axis, uint16_t steps, uint16_t freq_hz);
void motor_start_continous(uint8_t axis, uint16_t freq_hz);
void motor_stop(uint8_t axis);
void set_X_Y_direction(uint8_t direction);
void move_to_position_steps_xy(int32_t target_steps_x, int32_t target_steps_y, uint16_t speed_hz);
void move_to_position_steps_z(int32_t target_steps_z, uint16_t speed_hz);
void move_to_position_steps_xz(int32_t target_steps_x, int32_t target_steps_z, uint16_t speed_hz);
void start_XY_reference(void);
void move_Y_left_until_laser(uint16_t laser_target_mm, uint16_t speed_hz);
void move_pen_backward(void);
void move_pen_forward(void);
void move_X_relative(int32_t rel_steps, uint16_t speed);
void move_Z_relative(int32_t rel_steps, uint16_t speed);
void move_XZ_diagonal_relative(int32_t rel_steps_x, int32_t rel_steps_z, uint16_t speed);


//IO-Port-Mapping for the EndSensors for X and Y
#define X_SWITCH_TOP_PORT       PORTD      // Ausgangsregister für Pull-Up (z.?B. HIGH setzen)
#define X_SWITCH_TOP_IN_REG     PIND       // Eingangsregister, um Pin-Zustand (LOW/HIGH) zu lesen
#define X_SWITCH_TOP_DDR        DDRD       // Data Direction Register – legt fest, ob Pin Ein- oder Ausgang ist
#define X_SWITCH_TOP_PIN        PD0        // Der konkrete Pin (Pin D0 = digitaler Pin 0)

#define X_SWITCH_BOTTOM_PORT    PORTD      // PORTD: für z.?B. internen Pull-Up aktivieren
#define X_SWITCH_BOTTOM_IN_REG  PIND       // PIND: Eingang lesen
#define X_SWITCH_BOTTOM_DDR     DDRD       // DDRD: Richtung (Input/Output) einstellen
#define X_SWITCH_BOTTOM_PIN     PD2        // Angeschlossen an Pin D2

#define Y_SWITCH_LEFT_PORT      PORTD      // PORTD: für Pull-Up etc.
#define Y_SWITCH_LEFT_IN_REG    PIND       // Eingang lesen
#define Y_SWITCH_LEFT_DDR       DDRD       // Richtung festlegen
#define Y_SWITCH_LEFT_PIN       PD1        // Pin D1

#define Y_SWITCH_RIGHT_PORT 	PORTD		// PORTD: für Pull-Up etc.
#define Y_SWITCH_RIGHT_IN_REG	PIND		// Eingangsregister
#define Y_SWITCH_RIGHT_DDR		DDRD		// Datadirection Register
#define Y_SWITCH_RIGHT_PIN		PD3			// Anschluss Pin PD3 für Switch Right

#define EXT_INT_CONTROL_REG       EICRA  // External Interrupt Control Register A
#define EXT_INT_MASK_REG          EIMSK  // External Interrupt Mask Register


//IO-Port-Mapping for each Axis
// X-Axis
#define STEP_X_PORT PORTE	//Output-Port for STEP Axis-X
#define STEP_X_DDR  DDRE	//Data Direction Register for STEP Axis X
#define STEP_X_PIN  PE4		//Pin PE4 - Digital Pin 2 for STEP Axis X

#define DIR_X_PORT  PORTA	//Output-Port for DIR Axis-X
#define DIR_X_DDR   DDRA	//Data Direction Register for DIR Axis-X
#define DIR_X_PIN   PA4		//Pin PA4 - Digital Pin 26 for STEP Axis X

#define EN_X_PORT   PORTA	//Output-Port for ENABLE Axis-X
#define EN_X_DDR    DDRA	//Data Direction Register for ENABLE Axis X
#define EN_X_PIN    PA3		//Pin PA3 - Digital Pin 25 for ENABLE Axis X

// X-Axis uses for STEP the Pin PE4, that can also serve as an External Interrupt source for the Timer/Counter 3 - OC3B -
#define TIMER_X				3
#define STEP_X_OC_PIN		PE4         // PE4 is the Pin for OutputCompare Timer OC3B
#define STEP_X_OC_REG		TCCR3A		// TCCR3A is the Timer/Counter Control Register A for Timer3 --> Output Control for Timer 3
#define STEP_X_OC_BIT		COM3B0		// COM3BO in the TCCR3A Register: Toggle OC3B on compare Match
#define STEP_X_OCR			OCR3A		// OCR3A is the Output Compare Register for A for Timer 3 --> Target Value for Timer/Counter
#define STEP_X_TCNT			TCNT3		// TCNT3 is the actual Value of of the Timer/Counter 3
#define STEP_X_TIMSK_REG	TIMSK3		// TIMSK3 is the Timer Interrupt Mask Register for Timer3 / activates Interrupts for Timer3
#define STEP_X_OCIE_BIT		OCIE3B		// OCIE3B is the Output Compare Interrupt Enable B for Timer 3 --> Timer set an interrupt
#define STEP_X_TCCRB_REG	TCCR3B		// TCCR3B is the Timer/Counter Control Register B for Timer 3 // Important Bits: WGM32 / CS30 / CS31 / CS32

// Y-Achse
#define STEP_Y_PORT PORTB	//Output-Port for STEP Axis-Y
#define STEP_Y_DDR  DDRB	//Data Direction Register for STEP Axis Y
#define STEP_Y_PIN  PB6		//Pin PB6 - Digital Pin 12 for STEP Axis Y

#define DIR_Y_PORT  PORTA	//Output-Port for DIR Axis-Y
#define DIR_Y_DDR   DDRA	//Data Direction Register for DIR Axis-Y
#define DIR_Y_PIN   PA1		//Pin PA1 - Digital Pin 23 for STEP Axis Y

#define EN_Y_PORT   PORTA	//Output-Port for DIR Axis-Y
#define EN_Y_DDR    DDRA	//Data Direction Register for DIR Axis-Y
#define EN_Y_PIN    PA0		//Pin PA0 - Digital Pin 22 for STEP Axis Y

// Y-Axis uses for STEP PB6 --> Timer OC1B
#define TIMER_Y          1			// Verwendung von Timer 1 für die Y-Achse
#define STEP_Y_OC_PIN    PB6		// Der STEP-Ausgang der Y-Achse ist auf Pin PB6 (OC1B)
#define STEP_Y_OC_REG    TCCR1A		// Steuerregister A von Timer 1 (für Compare Match Einstellungen)
#define STEP_Y_OC_BIT    COM1B0		// Bit in TCCR1A, das das Verhalten von OC1B (PB6) steuert (Toggle bei Compare Match)
#define STEP_Y_OCR       OCR1A		// Vergleichswertregister für den Timer 1 ? bestimmt die Frequenz
#define STEP_Y_TCNT      TCNT1		// Aktueller Zählerstand des Timers 1 
#define STEP_Y_TIMSK_REG TIMSK1		// Interrupt-Maskenregister für Timer 1 (zur Aktivierung von Compare-Match-Interrupts)
#define STEP_Y_OCIE_BIT  OCIE1B		// Interrupt Enable Bit für Compare Match B (wird in TIMSK1 gesetzt)
#define STEP_Y_TCCRB_REG TCCR1B		// Steuerregister B von Timer 1 (wird u.?a. für den Prescaler genutzt)

// Z-Achse (Takt)
#define STEP_Z_PORT PORTH	//Output-Port for STEP Axis-Z
#define STEP_Z_DDR  DDRH	//Data Direction Register for DIR Axis-Z
#define STEP_Z_PIN  PH4		//Pin PH4 - Digital Pin 7 for STEP Axis Z

// Z-Achse (Drehrichtung)
#define DIR_Z_PORT  PORTL	//Output-Port for DIR Axis-Z
#define DIR_Z_DDR   DDRL	//Data Direction Register for DIR_Axis-Z
#define DIR_Z_PIN   PL2		// Pin PL2 for DIR-AXIS Z

// Z-Achse (Enable)
#define EN_Z_PORT  PORTL	//Output-Port for EN Axis-Z
#define EN_Z_DDR   DDRL		//Data Direction Register for EN-Z
#define EN_Z_PIN   PL3		// Pin PL3 for DIR-AXIS Z

//  Z-Axis uses for STEP PH4 --> Timer OC4B
#define TIMER_Z          4             // Verwendung von Timer 4 für die Z-Achse
#define STEP_Z_OC_PIN    PH4           // STEP-Ausgang der Z-Achse ist Pin H4 (OC4B)
#define STEP_Z_OC_REG    TCCR4A        // Steuerregister A für Timer 4 (für Compare Match-Konfiguration)
#define STEP_Z_OC_BIT    COM4B0        // Compare Match Output Mode Bit B – OC4B toggeln bei Match
#define STEP_Z_OCR       OCR4A         // Output Compare Register A von Timer 4 (legt Frequenz fest)
#define STEP_Z_TCNT      TCNT4         // Zählerstand des Timer 4 (wird bei Start zurückgesetzt)
#define STEP_Z_TIMSK_REG TIMSK4        // Interrupt-Maskenregister von Timer 4 (zum Aktivieren von Interrupts)
#define STEP_Z_OCIE_BIT  OCIE4B        // Interrupt Enable Bit für Compare Match B (für OC4B)
#define STEP_Z_TCCRB_REG TCCR4B        // Steuerregister B von Timer 4 (enthält Prescaler-Einstellungen)


#endif /* MOTOR_CONTROL_H_ */