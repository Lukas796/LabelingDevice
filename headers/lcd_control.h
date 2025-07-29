
/*
 * lcd_control.h
 *
 * Created: 22.05.2025 10:55:33
 *  Author: robin
 */ 

#ifndef LCD_H_
#define LCD_H_

#include "config.h" // need to be first

#define RS PC2  // Register Select
#define EC PC3  // Enable

// Funktionsdeklarationen
void lcd_char(char t);       // Zeichen senden
void lcd_cmd(uint8_t cmd);   // Befehl senden
void lcd_text(char* text);   // Zeichenkette senden
void lcd_init(void);         // LCD-Initialisierung
void lcd_Pos(uint8_t zeile, uint8_t spalte); //Cursor Pos
void lcd_num(uint16_t num,char* text);		//Zahlenwert senden

#endif /* LCD_H_ */