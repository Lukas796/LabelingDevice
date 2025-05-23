/*
 * IncFile1.h
 *
 * Created: 22.05.2025 10:55:33
 *  Author: robin
 */ 


#ifndef LASER_H_
#define LASER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void laser_init(void);
uint16_t laser_read(void);
void laser_teach(void);
void laser_display(void);

#endif /* INCFILE1_H_ */