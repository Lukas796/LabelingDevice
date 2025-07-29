
/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 * Last Change 12-06-2025
 */ 

// --- INCLUDES ---
#include "config.h" // need to be first
#include "motor_control.h"
#include "lcd_control.h"
#include "laser.h"
#include "systemstate.h"
#include "USART.h"
#include "buttons_control.h"

// main - Function 
int main(void)
{
	sei();  // activate global interrupts
	
	// --- Init-Functions ---
	motor_init();				// init motorpins 
	limit_switch_init();		// init endlagensensoren  
	motor_init_timer();			// init Timer für Motor 
	button_init();				// init Buttons 
	lcd_init();					// init LCD Display 
	init_ADC();					// init ADC for Laser 
	USART_Init(UBRR_VAL);		// init USART 
	initHandlers();				// init Handlerfunction
	
	
	uint8_t messung_aktiv = 0;		// variable für aktive Messung --> für main 
	lcd_cmd(0x80);					// Cursor setzen auf anfang erste Zeile 
	lcd_text("Labeling Device");	// LAbeling Device auf Dsiplay schreibn 
	
	
	// WHILE (1) Scheleife 
	while (1)
	{
		buttons();						// button- Abfrage 
		reference_StartPos_control();	// Prozedere Refernzfahrt
		start_Laser_Positioning();		// Prozedere LaserPositionierung Buchstabenschreiben 
		
		if (!labeling_start_requested()) {	// USARt Commands auswertung nur wenn noch nicht final gestaret wurde ausführen 
		USART_ProcessCommands(&messung_aktiv);  // Verarbeitet eingehende Steuerbefehle
		}
		USART_MESSUNG(messung_aktiv);           // Führt Messungen durch, falls aktiv
		
		_delay_ms(10);
	}
}


	
