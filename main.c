
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

// Status-Flag
uint8_t referenced = 0;

int main(void)
{
	sei();  // activate global interrupts

	motor_init();
	limit_switch_init();
	motor_init_timer();
	button_init();  

	
	// LCD-Pins als Ausgang setzen
	DDRC = 0xFC;

	// LCD initialisieren
	lcd_init();

	// Text definieren
	char text[] = "Labeling Device Robin Lukas"; // Beispieltext

	// Manuelle Längenberechnung ohne `strlen()`
	uint8_t j = 0;
	
	lcd_Pos(1, 1); // Cursor auf Zeile 1 setzen
	while (text[j] != '\0') // Schleife über den Text
	{
		if (j == 16) // Wenn 16 Zeichen erreicht sind, springe zu Zeile 2
		{
			lcd_Pos(2, 1);
		}
		lcd_char(text[j]);  // Einzelne Zeichen senden
		j++;
	}

	while (1)
	{
		reference_control();
	}
}


	
