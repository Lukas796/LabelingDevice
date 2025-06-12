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

//Definition for X_Y_MarkerDirection
#define DIR_X_TOP 0   // X-Axis Positiv --> Top
#define DIR_X_BOTTOM 1   // X-Axis Negativ --> Bottom
#define DIR_Y_LEFT 2   // Y-Axis Positiv --> Left
#define DIR_Y_RIGHT 3   // Y-Axis Negativ --> Right

// ADC für Lasersensor
#define SENSOR_ADC_CHANNEL 0  // z.B. ADC0 für A0

// UART
#define BAUDRATE 115200

#endif
