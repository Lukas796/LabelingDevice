/*
 * config.h - Konfigurationsdatei
 *
 * Created: 15.05.2025 21:45
 * Authors : lukasstrittmatter
 */ 
#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define F_CPU 16000000UL  // CPU-Frequenz für Delayfunktionen

// Achsenbezeichnungen
#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

// Richtungsdefinitionen
#define DIR_CW  0   // Rechtslauf
#define DIR_CCW 1   // Linkslauf

// --- Schrittmotor Pins ---

// X-Achse
#define STEP_X_PORT PORTB
#define STEP_X_DDR  DDRB
#define STEP_X_PIN  PB1

#define DIR_X_PORT  PORTB
#define DIR_X_DDR   DDRB
#define DIR_X_PIN   PB0

#define EN_X_PORT   PORTB
#define EN_X_DDR    DDRB
#define EN_X_PIN    PB2

// Y-Achse
#define STEP_Y_PORT PORTB
#define STEP_Y_DDR  DDRB
#define STEP_Y_PIN  PB3

#define DIR_Y_PORT  PORTB
#define DIR_Y_DDR   DDRB
#define DIR_Y_PIN   PB4

#define EN_Y_PORT   PORTB
#define EN_Y_DDR    DDRB
#define EN_Y_PIN    PB5

// Z-Achse (Rotation)
#define STEP_Z_PORT PORTB
#define STEP_Z_DDR  DDRB
#define STEP_Z_PIN  PB6

#define DIR_Z_PORT  PORTB
#define DIR_Z_DDR   DDRB
#define DIR_Z_PIN   PB7

#define EN_Z_PORT   PORTD
#define EN_Z_DDR    DDRD
#define EN_Z_PIN    PD6

// ADC für Lasersensor
#define OutP1 PK6  // z.B. ADC0 für A0
#define OutP2 PK5

// UART
#define BAUDRATE 9600

#endif
