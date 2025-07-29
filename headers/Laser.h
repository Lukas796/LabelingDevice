/*
 * Laser.h
 *
 * Created: 22.05.2025 10:55:33
 *  Author: robin
 */ 


#ifndef LASER_H_
#define LASER_H_

// -- INCLUDES --
#include "config.h" // need to be first

// --- FUNCTIONS --
void init_ADC(void);
uint16_t read_ADC(void);
uint16_t laser_read(void);
void laser_teach(void);

// ADC für Lasersensor
#define OutP1 PK6  // ADC0 auf PK6 
#define OutP2 PK5	// define PK5  


#endif /* INCFILE1_H_ */