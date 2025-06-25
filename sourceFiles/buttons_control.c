/*
 * buttons_control.c
 *
 * Created: 23.06.2025 10:46:04
 *  Author: lukasstrittmatter
 */ 
#include "config.h" // need to be first
#include "buttons_control.h"
#include "systemstate.h"
#include "motor_control.h"
#include "letter_control.h"


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

void buttons(void)
{
	if ((CONFIRM_BUTTON_PINREG & (1 << CONFIRM_BUTTON_PIN)))
	{
		request_reference_start(1);
	}
	
	if ((START_BUTTON_PINREG & (1 << START_BUTTON_PIN)))
	{
		request_Labeling_start(1);
	}
	
}