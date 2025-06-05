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
	motor_enable(AXIS_Y);
	motor_enable(AXIS_Z);
	motor_disable(AXIS_X);

	
	motor_set_direction(AXIS_X,DIR_CW);
	motor_set_direction(AXIS_Y,DIR_CW);
	motor_set_direction(AXIS_Z,DIR_CW);
	
    while (1) 
    {
		//STEP_X_PORT |= (1 << STEP_X_PIN);
		//STEP_Y_PORT |= (1 << STEP_Y_PIN);
		//_delay_ms(5);
		//STEP_X_PORT &= ~(1 << STEP_X_PIN);
		//STEP_Y_PORT &= ~(1 << STEP_Y_PIN);
		//_delay_ms(5);    
		motor_step(AXIS_Z)  ; 
    }
}

