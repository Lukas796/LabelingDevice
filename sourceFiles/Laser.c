/*
 * Laser.c
 *
 * Created: 22.05.2025 10:55:16
 *  Author: robin
 */ 

// --- Includes ---
#include "config.h" // need to be first
#include "Laser.h" // own header

// --- Functions ---
void init_ADC() {
	ADMUX |= (1 << REFS0);												// REFS0 1 und REFS1 0 --> AVCC with external capacitor at AREF Pin -->5V Versorgung (Standardmessung)
	ADMUX &= ~(1 << REFS1);												// ADC Multiplexer Selection Register ADMUX
	
	ADMUX |= (1 << MUX2) | (1 << MUX0);									// ADC Single-Ended Input ADC 13/OutP2 setzen (Mux-Konfiguration) (MUX5:0 --> 100101)
	ADMUX &= ~((1 << MUX3) | (1 << MUX1) | (1 << MUX4));				// Setze MUX5, MUX2, MUX0 auf 1 und MUX1, MUX3, MUX4 auf 0
	ADCSRB |= (1 << MUX5);												// ADC Control and Status Register B ADCSRB
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Setze ADEN auf 1 --> Enable ADC; Setze ADPS2, ADPS1 und ADPS0 auf 1 --> Division Factor 128 (XTAL/Input Clock) 125kHz
																		// ADC Control and Status Register A ADCSRA
	
	DDRK &= ~(1 << OutP2);												// Eingänge setzen: OutP2
	PORTK &= ~(1 << OutP2);												// Wenn Port als Eingang gesetzt ist, wird der Pull-up-Widerstand an OutP2 deaktiviert
	
	_delay_ms(10);														// Warte 10ms
}

uint16_t read_ADC() {
	ADCSRA |= (1 << ADSC);												// Setze ADSC auf 1, um die Wandlung zu starten
	while (ADCSRA & (1 << ADSC));										// Warte bis die Wandlung abgeschlossen ist (Hardware setzt ADCSRA automatisch auf 0)
	
	return ADC;															// Gib den ADC-Wert zurück
}

uint16_t laser_read(void)
{
	uint16_t rawValue = read_ADC();										// Neueste ADC-Wandlung starten und auslesen
	int distance_mm = (rawValue / 1023.0) * 120;						// Skalierung auf 120 mm Messbereich
	return (uint16_t)distance_mm;
}

void laser_teach(void)													//Zum neu kalibrieren							
{			
	DDRK |= (1 << OutP2);
	PORTK |= (1 << OutP2);												// Teach-In starten
	_delay_ms(500);
	PORTK &= ~(1 << OutP2);												// Teach-In beenden
}


