/*
 * lcd_control.c
 *
 * Created: 26.03.2025 17:37:15
 * Author : robin
 */ 
#ifndef LCD_C_
#define LCD_C_

#define F_CPU 16000000UL  // 16 MHz Taktfrequenz
#include <avr/io.h>
#include <util/delay.h>

#include "lcd_control.h"

// LCD-Pins gem‰ﬂ deiner Hardwareverbindung
#define RS PC2  // Register Select
#define EC PC3  // Enable

void lcd_char(char t)  // Zeichen senden
{
	// Oberes Halbbyte
	PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4));
	PORTC |= (t & 0xF0) | (1 << EC) | (1 << RS);
	PORTC &= ~(1 << EC);
	_delay_ms(1);

	// Unteres Halbbyte
	PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4));
	PORTC |= (t << 4) | (1 << EC) | (1 << RS);
	PORTC &= ~(1 << EC);
	_delay_ms(1);
}

void lcd_cmd(uint8_t cmd) // Befehl senden
{
	// Oberes Halbbyte
	PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4) | (1 << RS));
	PORTC |= (cmd & 0xF0) | (1 << EC);
	PORTC &= ~(1 << EC);
	_delay_ms(1);

	// Unteres Halbbyte
	PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4) | (1 << RS));
	PORTC |= (cmd << 4) | (1 << EC);
	PORTC &= ~(1 << EC);
	_delay_ms(1);
}

void lcd_text(char* text)
{
	uint8_t j = 0;
	while (text[j] != '\0') 
	{
		lcd_char(text[j]);  // Einzelne Zeichen senden
		j++;
	}
}

void lcd_Pos(uint8_t zeile, uint8_t spalte) // Cursor-Position setzen
{
	uint8_t spalte_temp = (spalte - 1);          // Spalte anpassen, da LCDs bei 0 starten
	uint8_t zeile_temp = (zeile - 1) * 0x40;     // Zeile berechnen (LCD-Zeilen haben feste Speicheradressen)
	uint8_t pos = (spalte_temp + zeile_temp) | 0x80; // Adresse berechnen und DB7-Bit setzen
	lcd_cmd(pos);  // Position an das LCD senden
}

void lcd_num(uint16_t num,char* text)
{
	char char1;			// variable for 100s
	char char2;			// variable for 10s
	
	char1 = num/100 + 0x30; // charachter for 100s
	num = num % 100;		// remainder
	
	char2 = num/10 + 0x30;	// character for 10s
	num = num % 10;			// remainder
	
	text[0] = char1;		//
	text[1] = char2;		//
	text[2] = num + 0x30;	//
	text[3] = 0x00;			// end of string
}

void lcd_init(void)
{
	DDRC = 0xFC;
	_delay_ms(20);
	PORTC &= ~((1 << RS) | (1 << PC6) | (1 << PC7));
	PORTC |= (1 << PC5) | (1 << PC4) | (1 << EC);
	PORTC &= ~(1 << EC);

	_delay_ms(5);
	PORTC |= (1 << EC);
	PORTC &= ~(1 << EC);

	_delay_ms(1);
	PORTC |= (1 << EC);
	PORTC &= ~(1 << EC);
	_delay_ms(1);

	// Wechsel in den 4-Bit-Modus
	PORTC &= ~(1 << PC4);
	PORTC |= (1 << EC);
	PORTC &= ~(1 << EC);
	_delay_ms(1);

	lcd_cmd(0x28);
	lcd_cmd(0x08);
	lcd_cmd(0x01);
	_delay_ms(2);

	lcd_cmd(0x06);
	lcd_cmd(0x14);
	lcd_cmd(0x0F);
	_delay_ms(2);
}

#endif /*LCD_C_*/