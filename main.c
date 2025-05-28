/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "motor_control.h"
#include "lcd_control.h"
#include "config.h"
#include "USART.h"


int main(void) 
{
	USART_Init(BAUDRATE);														// USART mit 9600 Baud initialisieren
	init_ADC();																	// ADC für Laser-Distanzmessung initialisieren

	USART_SendString("Laser-ADC Messung gestartet...\n");

	while (1) {
		uint16_t distance = laser_read();  // Distanz messen
		char buffer[20];                    // Speicher für die Ausgabe

		// Abstand in String umwandeln und senden
		snprintf(buffer, sizeof(buffer), "Distanz: %u mm\n", distance);
		USART_SendString(buffer);

		_delay_ms(1000); // Warte 1 Sekunde
	}

	return 0;
}

