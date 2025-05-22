/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 */ 

#include <avr/io.h>
#include "motor_control.h"
#include "config.h"


int main(void)
{
    motor_init();
	motor_enable(AXIS_X);
	
    while (1) 
    {
    }
}

