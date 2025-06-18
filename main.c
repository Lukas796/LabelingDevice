
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
#include "laser.h"
#include "systemstate.h"
#include "USART.h"


int main(void)
{
	sei();  // activate global interrupts

	motor_init();
	limit_switch_init();
	motor_init_timer();
	button_init();  
	lcd_init();
	init_ADC();  
	USART_Init(BAUDRATE);
	
	uint8_t messung_aktiv = 0;
	lcd_text("Labeling Device");
	
	while (1)
	{
		buttons();
		reference_StartPos_control();
		start_Laser_Positioning();
		
		USART_ProcessCommands(&messung_aktiv);  // Verarbeitet eingehende Steuerbefehle
		USART_MESSUNG(messung_aktiv);           // Führt Messungen durch, falls aktiv
		_delay_ms(100);
	}
}


	
