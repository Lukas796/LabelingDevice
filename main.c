
/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 * Last Change 12-06-2025
 */ 
#define F_CPU 16000000UL  // Mega 2560 läuft mit 16 MHz

#include <avr/io.h>
#include <util/delay.h>

#include "motor_control.h"
#include "lcd_control.h"
#include "config.h"

int main(void)
{
	sei();  // activate global interrupts

	motor_init();
	limit_switch_init();
	motor_init_timer();
	button_init();  
	
	// LCD initialisieren
	lcd_init();


	
	while (1)
	{
		reference_StartPos_control();
		
	}
}


	
