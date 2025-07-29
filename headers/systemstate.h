/*
 * systemstate.h
 *
 * Created: 12.06.2025 09:19:45
 *  Author: lukasstrittmatter
 */ 


#ifndef SYSTEMSTATE_H_
#define SYSTEMSTATE_H_

#include "config.h" // need to be first

#define MAX_CHAR 128 

// -----------------------------
// Functions in systemstate.c
// Funktionen zum Abfragen und Setzen des Systemzustands
uint8_t is_referenced(void);		
uint8_t is_inStartPos(void);
uint8_t reference_start_requested(void);
uint8_t labeling_start_requested(void);
void set_referenced(uint8_t state);
void set_inStartPos(uint8_t state);
void request_reference_start(uint8_t state);
void request_Labeling_start(uint8_t state);

// Ablauffunktionien für Referenz und Buchstabenschrieben 
void reference_StartPos_control(void);
void start_Laser_Positioning(void);

// handlerfunktionien 
void initHandlers(void);

#endif /* SYSTEMSTATE_H_ */