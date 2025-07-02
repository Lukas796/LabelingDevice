/*
 * buttons_control.c
 *
 * Created: 23.06.2025 10:46:04
 *  Author: lukasstrittmatter
 */ 

#include "config.h"			// need to be first. include generel config
#include "buttons_control.h"	// inkludiere alle eigenen Funktionen 
#include "systemstate.h"		// inkludiere die Systemstate Funktionen 
#include "motor_control.h"		// inkludiere die motor_control funktionen 
#include "letter_control.h"		// inkludiere die letter control funktionen 

// button_init(): mit dieser Funktion werden die vier Druckbuttons der Hardware-Box als EIngänge initialisert 
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

// buttons(): mit dieser Funktion wird der Zustand des Start_Refernzfahrt- Buttons und des Start_Labling Buttons überprüft und die Anfragen an die Systemstate werden gesetzt
void buttons(void)
{
	if ((CONFIRM_BUTTON_PINREG & (1 << CONFIRM_BUTTON_PIN)))	// ist auf dem Pin 40 für Starte_Refernzfahrt eine 1
	{
		request_reference_start(1);		// set state_variable start_reference = 1 --> geht zu system_state.c
	}
	
	if ((START_BUTTON_PINREG & (1 << START_BUTTON_PIN)))	// ist auf dem Pin 43 für Starte_Labeling eine 1
	{
		request_Labeling_start(1);		// set state_variable start_Labeling = 1 --> geht zu system_state.c
	}
	
}