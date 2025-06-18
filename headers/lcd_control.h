#ifndef LCD_H_
#define LCD_H_

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>

#define RS PC2  // Register Select
#define EC PC3  // Enable

// Funktionsdeklarationen
void lcd_char(char t);       // Zeichen senden
void lcd_cmd(uint8_t cmd);   // Befehl senden
void lcd_text(char* text);   // Zeichenkette senden
void lcd_init(void);         // LCD-Initialisierung
void lcd_Pos(uint8_t zeile, uint8_t spalte);
void USART_ReadString(char *buffer, uint8_t max_length);

#endif /* LCD_H_ */