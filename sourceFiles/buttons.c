/*
 * buttons.c
 *
 * Created: 12.06.2025 09:20:17
 *  Author: lukasstrittmatter
 */ 
#include "buttons.h"
#include "motor_control.h"
#include "config.h"

static uint8_t referenced = 0;	//static state variable if Axis are referenced

void button_init(void) {
	// Set PG1 - DPin 40 - Confirmbutton as input
	CONFIRM_BUTTON_DDR &= ~(1 << CONFIRM_BUTTON_PIN);       // Set as input
	CONFIRM_BUTTON_PORT |= (1 << CONFIRM_BUTTON_PIN);       // Enable internal pull-up
	
	// Set PG0 - DPin 41 - Abortbutton as input
	ABORT_BUTTON_DDR &= ~(1 << ABORT_BUTTON_PIN);       // Set as input
	ABORT_BUTTON_PORT |= (1 << ABORT_BUTTON_PIN);       // Enable internal pull-up
		
	// Set PL7 - DPin 42 - Secondbutton as input
	SECOND_BUTTON_DDR &= ~(1 << SECOND_BUTTON_PIN);       // Set as input
	SECOND_BUTTON_PORT |= (1 << SECOND_BUTTON_PIN);       // Enable internal pull-up
			
	// Set PL6 - DPin 43 - Startbutton as input
	START_BUTTON_DDR &= ~(1 << START_BUTTON_PIN);       // Set as input
	START_BUTTON_PORT |= (1 << START_BUTTON_PIN);       // Enable internal pull-up
}
uint8_t is_referenced(void) {
	return referenced;
}

void set_referenced(uint8_t state) {
	referenced = state;
}
void reference_control(void){

	// Wenn Confirm Taster gedrückt UND noch nicht referenziert wurde
	if (!is_referenced())
	{
		if ((CONFIRM_BUTTON_PINREG & (1 << CONFIRM_BUTTON_PIN)))
		{
			start_XY_reference();			 // Referenzfahrt starten
		}
	}
} 

