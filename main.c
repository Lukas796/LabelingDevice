/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "config.h"
#include "motor_control.h"
#include "lcd_control.h"
#include "USART.h"


int main(void) 
{
	USART_Init(BAUDRATE);														// USART mit 9600 Baud initialisieren
	init_ADC();
	lcd_init();																	// ADC für Laser-Distanzmessung initialisieren

	uint8_t messung_aktiv = 0;  // Zustand der Messung speichern
	DDRC = 0xFC;

	while (1) 
	{
		if (USART_DataAvailable()) 
		{
			char buffer[30];  // Speicher für empfangene Nachricht
			USART_ReadString(buffer, sizeof(buffer));  // Ganze Nachricht lesen

			// Prüfen, ob es ein Steuerbefehl ist
			if (strcmp(buffer, "M") == 0) 
			{
				messung_aktiv = 1;  // Messung aktivieren
			}
			else if (strcmp(buffer, "MN") == 0) 
			{
				messung_aktiv = 0;  // Messung stoppen
			}
			else if (strcmp(buffer, "R") == 0) 
			{
				USART_SendString("Referenzfahrt gestartet.\n");
				lcd_text("Referenzfahrt gestartet");
			}
			else if (strcmp(buffer, "STOP") == 0)
			{
				USART_SendString("Notstop.\n");
				lcd_text("Notstop");
			}
			else if (strcmp(buffer, "START") == 0)
			{
				USART_SendString("Wird gestartet.\n");
			}
			else if (strstr(buffer, "Beschriftung:") != 0)  // Prüft, ob "Beschriftung:" enthalten ist
			{
				USART_SendString("Folgender Text wird geschrieben: ");
				char* text_start = buffer + strlen("Beschriftung:");  // Dynamische Berechnung der Startposition
				USART_SendString(text_start);
				USART_SendString("\n");
				lcd_text(text_start);  // Text an das LCD senden
			}

			else
			{
				
			}
	}
	
		// Nur Messwerte senden, wenn die Messung aktiv ist
		if (messung_aktiv) 
		{
			uint16_t distance = laser_read();  // Abstand messen
			char send_buffer[20];
			snprintf(send_buffer, sizeof(send_buffer), "%u mm\n", distance);
			USART_SendString(send_buffer);
		}

		_delay_ms(2000);  // Warte zwei Sekunde zwischen Messungen
	}

	return 0;
}

