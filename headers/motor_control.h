/*
 * motor_control.h
 *
 * Created: 15.05.2025 21:58:19
 *  Author: lukasstrittmatter
 */ 


#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <avr/io.h>
#include <stdint.h>

void motor_init(void);
void motor_enable(uint8_t axis);
void motor_disable(uint8_t axis);
void motor_set_direction(uint8_t axis, uint8_t direction);
void motor_step(uint8_t axis);


#endif /* MOTOR_CONTROL_H_ */