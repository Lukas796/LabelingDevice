/*
 * config.h - Config Data for IO-Mapping
 *
 * Created: 15.05.2025 21:45
 * Authors : lukasstrittmatter
 */ 
#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define F_CPU 16000000UL  // CPU-Frequency for Delay-Functions
// --------------------------------------
// --- Motor Control --------------------

// Definition of AXIS
#define AXIS_X 0	//set AXIS_X as 0
#define AXIS_Y 1	//set AXIS_Y as 1
#define AXIS_Z 2	//set AXIS_Z as 2

//Definition for Motor direction
#define DIR_CW  0   // Counterwise
#define DIR_CCW 1   // CounterClockwise

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
#define TIMER_Y          1
#define STEP_Y_OC_PIN    PB6        
#define STEP_Y_OC_REG    TCCR1A
#define STEP_Y_OC_BIT    COM1B0
#define STEP_Y_OCR       OCR1A
#define STEP_Y_TCNT      TCNT1
#define STEP_Y_TIMSK_REG TIMSK1
#define STEP_Y_OCIE_BIT  OCIE1B
#define STEP_Y_TCCRB_REG TCCR1B

// Z-Achse (Takt)
#define STEP_Z_PORT PORTH	//Output-Port for DIR Axis-X
#define STEP_Z_DDR  DDRH	//Data Direction Register for DIR Axis-X 
#define STEP_Z_PIN  PH4		//Pin PH4 - Digital Pin 7 for STEP Axis X

// Z-Achse (Drehrichtung)
#define DIR_Z_PORT  PORTL
#define DIR_Z_DDR   DDRL
#define DIR_Z_PIN   PL2

// Z-Achse (Enable)
#define EN_Z_PORT  PORTL
#define EN_Z_DDR   DDRL
#define EN_Z_PIN   PL3

//  Z-Axis uses for STEP PH4 --> Timer OC4B 
#define TIMER_Z          4
#define STEP_Z_OC_PIN    PH4        // OC4B
#define STEP_Z_OC_REG    TCCR4A
#define STEP_Z_OC_BIT    COM4B0
#define STEP_Z_OCR       OCR4A
#define STEP_Z_TCNT      TCNT4
#define STEP_Z_TIMSK_REG TIMSK4
#define STEP_Z_OCIE_BIT  OCIE4B
#define STEP_Z_TCCRB_REG TCCR4B

// ADC für Lasersensor
#define SENSOR_ADC_CHANNEL 0  // z.B. ADC0 für A0

// UART
#define BAUDRATE 115200

#endif
