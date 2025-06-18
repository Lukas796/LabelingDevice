/*
 * buttons.c
 *
 * Created: 12.06.2025 09:20:17
 *  Author: lukasstrittmatter
 */ 
#include "systemstate.h"
#include "motor_control.h"
#include "config.h"

static uint8_t referenced = 0;	//static state variable if Axis are referenced	0 = not referenced , 1 = referenced
static uint8_t start_reference = 0;	//static state variable if Axis are referenced	0 = dont start reference , 1 = start reference
static uint8_t inStartPos = 0;	//static state variable if Axis is in startPos
static uint8_t interrupts_initialized = 0;
static uint8_t laser_pos_reached = 0;
static uint8_t start_Labeling = 0;

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

uint8_t is_inStartPos(void) {
	return inStartPos;
}

uint8_t reference_start_requested(void) {
	return start_reference;
}

uint8_t labeling_start_requested(void) {
	return start_Labeling;
}

void set_referenced(uint8_t state) {
	referenced = state;
}

void set_inStartPos(uint8_t state) {
	inStartPos = state;
}

void request_reference_start(uint8_t state) {
	start_reference = state;
}

void request_Labeling_start(uint8_t state) {
	start_Labeling = state;
}

void buttons(void)
{
	if ((CONFIRM_BUTTON_PINREG & (1 << CONFIRM_BUTTON_PIN)))
	{
		_delay_ms(50);  // Entprellen
		request_reference_start(1);
	}
	
	if ((START_BUTTON_PINREG & (1 << START_BUTTON_PIN)))
	{
		_delay_ms(50);  // Entprellen
		request_Labeling_start(1);
	}
	
}

void reference_StartPos_control(void){
	
	// Wenn Refernce Start Requested UND noch nicht referenziert wurde
	if (!is_referenced())
	{
		if (reference_start_requested())
		{
			
			disable_limit_switch_interrupts();
			interrupts_initialized = 0;
			start_XY_reference();			 // Referenzfahrt starten
		}
	}
	
	if ((is_referenced()) && (!is_inStartPos()))
	{
		move_to_position_steps_xy(1800,4300,1000);	// move to startposition 
		set_inStartPos(1);
	}
	
	if ((is_referenced()) && (is_inStartPos()) && !interrupts_initialized)
	{
		limit_switch_interrupt_init(); //init interrupts for limit switches
		interrupts_initialized = 1;
	}

} 

void start_Laser_Positioning (void) {
	if ((labeling_start_requested()) && (is_referenced()) && (is_inStartPos()) && interrupts_initialized)
	{
		if (!laser_pos_reached)
		{
			move_Y_left_until_laser(20,200);
			laser_pos_reached = 1;
		}
	}
		
}
