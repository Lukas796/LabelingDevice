#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor_control.h"
#include "config.h"
// Status-Flag
uint8_t referenced = 0;

int main(void)
{
	sei();  // activate global interrupts

	motor_init();
	limit_switch_init();
	motor_init_timer();
	button_init();  

	motor_enable(AXIS_X);
	motor_enable(AXIS_Y);
	motor_enable(AXIS_Z);

	motor_set_direction(AXIS_X, DIR_CW);
	motor_set_direction(AXIS_Y, DIR_CW);
	motor_set_direction(AXIS_Z, DIR_CW);
	

	while (1)
	{
		// Wenn Taster gedrückt UND noch nicht referenziert wurde
		if ((CONFIRM_BUTTON_PINREG & (1 << CONFIRM_BUTTON_PIN)) && !referenced)
		{
			start_XY_reference();   // Referenzfahrt starten
			referenced = 1;         // Flag setzen, damit nur einmal ausgeführt wird
		}
	}
}