/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>								
#include "motor_control.h"
#include "config.h"




int main(void)
{
    motor_init();
	motor_init_timer();
	motor_enable(AXIS_Y);
	motor_enable(AXIS_Z);
	motor_enable(AXIS_X);

	
	motor_set_direction(AXIS_X,DIR_CW);
	motor_set_direction(AXIS_Y,DIR_CW);
	motor_set_direction(AXIS_Z,DIR_CCW);
	
	motor_start_steps(AXIS_X,300);
	motor_start_steps(AXIS_Y,300);
	motor_start_steps(AXIS_Z,300);	
    while (1) 
    {
		//STEP_X_PORT |= (1 << STEP_X_PIN);
		//STEP_Y_PORT |= (1 << STEP_Y_PIN);
		//_delay_ms(5);
		//STEP_X_PORT &= ~(1 << STEP_X_PIN);
		//STEP_Y_PORT &= ~(1 << STEP_Y_PIN);
		//_delay_ms(5);    
		//motor_step(AXIS_X)  ; 

		

    }
}

