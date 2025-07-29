/*
 * lcd_control.c
 *
 * Created: 26.03.2025 17:37:15
 * Author : robin
 */ 
#ifndef LCD_C_
#define LCD_C_

#include "config.h" // need to be first
#include "lcd_control.h"

// LCD-Pins gem‰ﬂ deiner Hardwareverbindung
#define RS PC2  // Register Select
#define EC PC3  // Enable

void lcd_char(char t)  // Zeichen senden                                // Sendet ein Zeichen an das LCD
{                                                                         // Beginn lcd_char
	// Oberes Halbbyte
	PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4));       // Lˆscht obere 4 Datenleitungen
	PORTC |= (t & 0xF0) | (1 << EC) | (1 << RS);                         // Setzt obere Bits, EC und RS
	PORTC &= ~(1 << EC);                                                 // EC zur¸cksetzen
	_delay_ms(1);                                                        

	// Unteres Halbbyte
	PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4));       // Lˆscht obere 4 Datenleitungen
	PORTC |= (t << 4) | (1 << EC) | (1 << RS);                           // Setzt untere Bits, EC und RS
	PORTC &= ~(1 << EC);                                                 // EC zur¸cksetzen
	_delay_ms(1);                                                        
}                                                                       

void lcd_cmd(uint8_t cmd) // Befehl senden                              // Sendet einen Steuerbefehl ans LCD
{                                                                        
	// Oberes Halbbyte
	PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4) | (1 << RS));  // Lˆscht obere Bits und RS
	PORTC |= (cmd & 0xF0) | (1 << EC);                                    // Setzt obere Bits und EC
	PORTC &= ~(1 << EC);                                                  // EC zur¸cksetzen
	_delay_ms(1);                                                         

	// Unteres Halbbyte
	PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4) | (1 << RS));  // Lˆscht obere Bits und RS
	PORTC |= (cmd << 4) | (1 << EC);                                      // Setzt untere Bits und EC
	PORTC &= ~(1 << EC);                                                  // EC zur¸cksetzen
	_delay_ms(1);                                                         // Kurze Verzˆgerung
}                                                                         

void lcd_text(char* text)                                                  // Gibt eine Zeichenkette aus
{                                                                        
	uint8_t j = 0;                                                        // Initialisiert Index j
	while (text[j] != '\0')                                               // Schleife bis Nullterminator
	{                                                                     // Schleifenkˆrper
		lcd_char(text[j]);                                                // Sende aktuelles Zeichen
		j++;                                                              // Index erhˆhen
	}                                                                     
}                                                                         

void lcd_Pos(uint8_t zeile, uint8_t spalte) // Cursor-Position setzen     // Positioniert den Cursor
{                                                                       
	uint8_t spalte_temp = (spalte - 1);                                   // Nullbasiertes Spalten-Offset
	uint8_t zeile_temp = (zeile - 1) * 0x40;                              // Zeilenoffset im DDRAM
	uint8_t pos = (spalte_temp + zeile_temp) | 0x80;                      // Adresse mit Set-Bit
	lcd_cmd(pos);                                                         // Sende Positionsbefehl
}                                                                         

void lcd_num(uint16_t num,char* text)                                     // Wandelt Zahl in ASCII um
{                                                                         // Beginn lcd_num
	char char1;                                                           // Hunderterstelle
	char char2;                                                           // Zehnerstelle

	char1 = num/100 + 0x30;                                               // ASCII Hunderter
	num = num % 100;                                                      // Rest bestimmen

	char2 = num/10 + 0x30;                                                // ASCII Zehner
	num = num % 10;                                                       // Rest bestimmen

	text[0] = char1;                                                      // Setze Hunderter
	text[1] = char2;                                                      // Setze Zehner
	text[2] = num + 0x30;                                                 // Setze Einer
	text[3] = 0x00;                                                       // Nullterminator
}                                                                       

void lcd_init(void)                                                       // Initialisiert das LCD
{                                                                        
	DDRC = 0xFC;                                                          // PC2..PC7 als Ausgang
	_delay_ms(20);                                                        // Warte nach Power-On

	PORTC &= ~((1 << RS) | (1 << PC6) | (1 << PC7));                      // Lˆsche RS und Datenleitungen
	PORTC |= (1 << PC5) | (1 << PC4) | (1 << EC);                         // Erste Pulse senden
	PORTC &= ~(1 << EC);                                                  // EC zur¸cksetzen

	_delay_ms(5);                                                         
	PORTC |= (1 << EC);                                                   // Enable setzen
	PORTC &= ~(1 << EC);                                                  // Enable zur¸cksetzen

	_delay_ms(1);                                                        
	PORTC |= (1 << EC);                                                   // Enable setzen
	PORTC &= ~(1 << EC);                                                  // Enable zur¸cksetzen
	_delay_ms(1);                                                         

	// Wechsel in 4-Bit-Modus
	PORTC &= ~(1 << PC4);                                                 // Halbes Kommando senden
	PORTC |= (1 << EC);                                                   // Enable setzen
	PORTC &= ~(1 << EC);                                                  // Enable zur¸cksetzen
	_delay_ms(1);                                                        

	lcd_cmd(0x28);                                                        // 4-Bit, 2 Zeilen, 5x8
	lcd_cmd(0x08);                                                        // Display aus
	lcd_cmd(0x01);                                                        // Display lˆschen
	_delay_ms(2);                                                         

	lcd_cmd(0x06);                                                        // Cursor inkrement
	lcd_cmd(0x14);                                                        // Cursor verschieben
	lcd_cmd(0x0F);                                                        // Display an, Cursor blinkt
	_delay_ms(2);                                                         
}                                                                         


#endif /*LCD_C_*/