/*
 * IncFile1.h
 *
 * Created: 22.05.2025 10:55:33
 *  Author: robin
 */ 


#ifndef LASER_H_
#define LASER_H_

#include "config.h" // need to be first

void init_ADC(void);
uint16_t read_ADC(void);
uint16_t laser_read(void);
void laser_teach(void);

// ADC f�r Lasersensor
#define OutP1 PK6  // z.B. ADC0 f�r A0
#define OutP2 PK5


#endif /* INCFILE1_H_ */