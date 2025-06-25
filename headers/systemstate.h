/*
 * systemstate.h
 *
 * Created: 12.06.2025 09:19:45
 *  Author: lukasstrittmatter
 */ 


#ifndef SYSTEMSTATE_H_
#define SYSTEMSTATE_H_

#include <avr/io.h>
#include <stdint.h>

void reference_control(void);
void set_referenced(uint8_t state);
uint8_t is_referenced(void);

void set_inStartPos(uint8_t state);
uint8_t is_inStartPos(void);


#endif /* SYSTEMSTATE_H_ */