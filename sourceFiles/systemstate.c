/*
 * buttons.c
 *
 * Created: 12.06.2025 09:20:17
 *  Author: lukasstrittmatter
 */ 
#include "config.h" // need to be first
#include "systemstate.h"
#include "motor_control.h"
#include "letter_control.h"

static uint8_t referenced = 0;	//static state variable if Axis are referenced	0 = not referenced , 1 = referenced
static uint8_t start_reference = 0;	//static state variable if Axis are referenced	0 = dont start reference , 1 = start reference
static uint8_t inStartPos = 0;	//static state variable if Axis is in startPos
static uint8_t interrupts_initialized = 0;
static uint8_t laser_pos_reached = 0;
static uint8_t start_Labeling = 0;
static uint8_t Labeling_finfished = 0;


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
		move_to_position_steps_xy(2300,5000,1000);	// move to startposition 
		
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
			
				move_Y_left_until_laser(20,100);
				//move_to_position_steps_xy(500,500,1000);
				
				//move_to_position_steps_xz(1100, 150, 500);
				//move_to_position_steps_xz(500, 0, 500);
				//move_to_position_steps_xz(1100, 150, 500);
				//move_to_position_steps_xz(500, 0, 500);
				//move_Z_relative(100,500);
				//move_X_relative(100,500);
				//move_Z_relative(-100,500);
				//move_X_relative(-100,500);
				//move_to_position_steps_xy(2000,2000,1000);
				//move_to_position_steps_xz(1600, 300, 500);
				//move_to_position_steps_xy(2000,2000,1000);	// move to startposition
				//move_to_position_steps_z(100,400);
				//move_Z_relative(100,500);
				//move_to_position_steps_xy(500,500,1000);	// move to startposition
				//move_X_relative(100,500);
				//move_to_position_steps_xz(2600 , 150, 500);
				//move_Z_relative(200,500);
				//move_X_relative(200,500);
				//move_Y_left_until_laser(20,500);
				//move_pen_backward();
				//move_to_position_steps_xy(2300,4300,1000);	// move to startposition
				//move_pen_backward();
				//move_Z_relative(200,500);
				//move_X_relative(200,500);
				//move_X_relative(-200,500);
				//move_pen_forward();
				//move_to_position_steps_xz(2100 , 100, 500);
				//move_to_position_steps_z(200,400);
				//move_pen_backward();
				//move_pen_forward();
				//move_Y_left_until_laser(20,500);
				//move_pen_backward();
				//move_pen_forward();
				//move_to_position_steps_xy(100,100,500);	// move to startposition
				//move_to_position_steps_z(300,500);
				//move_to_position_steps_xy(2300,4300,1000);	// move to startposition
				//move_to_position_steps_xz(2100 , 350, 500);
				//move_to_position_steps_z(100,500);
				//move_Y_left_until_laser(20,500);
				//move_to_position_steps_xy(200,200,300);	// move to startposition
				//move_to_position_steps_z(600,500);
				//move_to_position_steps_xy(2300,4300,400);	// move to startposition
				//move_Y_left_until_laser(20,500);
				//move_to_position_steps_xy(100,100,1000);	// move to startposition
				//move_to_position_steps_xy(2300,4300,1000);	// move to startposition
				//move_Y_left_until_laser(20,500);
				//move_to_position_steps_xy(100,100,1000);	// move to startposition
			laser_pos_reached = 1;
		}
		
		if ((laser_pos_reached) && (!Labeling_finfished))
		{
			
			draw_A();
			_delay_ms(100);
			draw_A();
			_delay_ms(100);
			draw_A();
			_delay_ms(100);
			draw_A();
			
			Labeling_finfished = 1;
		}
	}
		
}
