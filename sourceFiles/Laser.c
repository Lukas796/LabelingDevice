/*
 * CFile1.c
 *
 * Created: 22.05.2025 10:55:16
 *  Author: robin
 */ 
#include "Laser.h"
#include "config.h"


void setup() {
	Serial.begin(9600); // Serielle Verbindung starten mit 9600 Baud
}
 
void laser_init(void)
{
	DDRK &= ~((1 << OP1) | (1 << OP2)); // OP1 und OP2 als Eingänge setzen
}

uint16_t laser_read(void)
{
	uint16_t rawValue = ADC; // Direkt aus dem ADC-Register lesen
	int distance_mm = (rawValue / 1023.0) * 120;  // Skalierung auf 120 mm Messbereich
	return (uint16_t)distance_mm;
}

void laser_teach(void)
{
	PORTK |= (1 << OP2);  // Teach-In starten
	_delay_ms(500);
	PORTK &= ~(1 << OP2); // Teach-In beenden
}

void laser_display(void)
{
	int distance = laser_read(); // Abstand messen

	String("Abstand: ");
	Serial.print(distance);
	Serial.println(" mm"); // Neue Zeile für saubere Anzeige
}

