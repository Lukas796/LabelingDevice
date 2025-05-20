/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 */ 

#define F_CPU 16000000UL  // Mega 2560 läuft mit 16 MHz

#include <avr/io.h>
#include <util/delay.h>

// LCD-Pins für Arduino Mega 2560
#define RS 30
#define E 31
#define DB4 32
#define DB5 33
#define DB6 34
#define DB7 35

// LED-Pin
#define LED_PIN 9  // Eingebaute LED auf dem Mega 2560

void lcd_char(char t);
void lcd_cmd(uint8_t cmd);
void lcd_init(void);

int main(void) {
	// Setze LCD-Pins als Ausgang
	DDRC = 0xFC;
	lcd_init(); // Initialisiere das LCD
	lcd_char('A'); // Zeige einen Buchstaben auf dem LCD
	
	// Setze LED-Pin als Ausgang
	DDRB |= (1 << LED_PIN);
	
	while (1) {
		PORTB |= (1 << LED_PIN); // LED AN
		_delay_ms(50);
		PORTB &= ~(1 << LED_PIN); // LED AUS
		_delay_ms(500);
	}
}

