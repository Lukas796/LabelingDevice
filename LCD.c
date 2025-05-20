/*
 * GccApplication2.c
 *
 * Created: 26.03.2025 17:37:15
 * Author : robin
 */ 


#define F_CPU 16000000UL  // 16 MHz Taktfrequenz
#include <avr/io.h>
#include <util/delay.h>

// LCD-Pins gem‰ﬂ neuer Hardwareverbindung
#define RS 30
#define E 31
#define DB4 32
#define DB5 33
#define DB6 34
#define DB7 35

void lcd_char(char t) 
{
	// Oberes Halbbyte
	PORTC &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	PORTC |= (t & 0xF0) | (1 << E) | (1 << RS);
	PORTC &= ~(1 << E);
	_delay_ms(1);

	// Unteres Halbbyte
	PORTC &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	PORTC |= (t << 4) | (1 << E) | (1 << RS);
	PORTC &= ~(1 << E);
	_delay_ms(1);
}

void lcd_cmd(uint8_t cmd) 
{
	// Oberes Halbbyte
	PORTC &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4) | (1 << RS));
	PORTC |= (cmd & 0xF0) | (1 << E);
	PORTC &= ~(1 << E);
	_delay_ms(1);

	// Unteres Halbbyte
	PORTC &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4) | (1 << RS));
	PORTC |= (cmd << 4) | (1 << E);
	PORTC &= ~(1 << E);
	_delay_ms(1);
}

void lcd_init(void) 
{
	_delay_ms(20);
	PORTC &= ~((1 << RS) | (1 << DB6) | (1 << DB7));
	PORTC |= (1 << DB5) | (1 << DB4) | (1 << E);
	PORTC &= ~(1 << E);

	_delay_ms(5);
	PORTC |= (1 << E);
	PORTC &= ~(1 << E);

	_delay_ms(1);
	PORTC |= (1 << E);
	PORTC &= ~(1 << E);
	_delay_ms(1);

	// Wechsel in den 4-Bit-Modus
	PORTC &= ~(1 << DB4);
	PORTC |= (1 << E);
	PORTC &= ~(1 << E);
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