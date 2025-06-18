/*
 * USART.c
 *
 * Created: 28.05.2025 10:30:44
 *  Author: robin
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "USART.h"
#include "lcd_control.h"
#include "laser.h"

#define USART_BUFFER_SIZE 64

volatile uint8_t usart_rx_buffer[USART_BUFFER_SIZE];
volatile uint8_t usart_rx_head = 0;
volatile uint8_t usart_rx_tail = 0;
char LCDstr[17];

// USART initialisieren
void USART_Init(uint16_t baud) 
{
	UBRR0L = 103;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	sei();
}

// Empfängt und verarbeitet UART-Befehle
void USART_ProcessCommands(uint8_t* messung_aktiv) 
{
	if (USART_DataAvailable()) {
		char buffer[30];
		USART_ReadString(buffer, sizeof(buffer));

		if (strcmp(buffer, "M") == 0) {
			*messung_aktiv = 1;
			} else if (strcmp(buffer, "MN") == 0) {
			*messung_aktiv = 0;
			} else if (strcmp(buffer, "R") == 0) {
			USART_SendString("Referenzfahrt gestartet.\n");
			lcd_cmd(0xC0);
			lcd_text("Referenzfahrt!");
			} else if (strcmp(buffer, "STOP") == 0) {
			USART_SendString("Notstop.\n");
			lcd_cmd(0xC0);
			lcd_text("Notstop!     ");
			} else if (strcmp(buffer, "START") == 0) {
			USART_SendString("Wird gestartet.\n");
			lcd_cmd(0xC0);
			lcd_text("Wird gestartet!");
			} else if (strstr(buffer, "Beschriftung:") != 0) {
			USART_SendString("Folgender Text wird geschrieben: ");
			char* text_start = buffer + strlen("Beschriftung:");
			USART_SendString(text_start);
			USART_SendString("\n");
			lcd_cmd(0xC0);
			lcd_text(text_start);
			}else{
			lcd_cmd (0x01);
			}
	}
}


// Messungsprozess
void USART_MESSUNG(uint8_t messung_aktiv) {
	if (messung_aktiv) {
		uint16_t distance = laser_read();
		char send_buffer[20];
		snprintf(send_buffer, sizeof(send_buffer), "%u mm\n", distance);
		USART_SendString(send_buffer);
		uint16_t i = laser_read();
		lcd_cmd (0xC0);
		lcd_num (i,LCDstr);
		lcd_text(LCDstr,"   ");
	}
}

// Daten senden
void USART_SendData(uint8_t data) 
{
	while (!(UCSR0A & (1 << UDRE0))); // Warten, bis UDR0 bereit ist
	UDR0 = data; // Daten senden
}

// String senden
void USART_SendString(const char *str) 
{
	while (*str) {
		USART_SendData(*str++);
	}
}

// Empfangs-Interrupt (ISR)
ISR(USART0_RX_vect) 
{
	uint8_t received_data = UDR0; // Empfangenes Zeichen holen

	uint8_t next_head = (usart_rx_head + 1) % USART_BUFFER_SIZE;

	// Überprüfen, ob der Puffer voll ist
	if (next_head != usart_rx_tail) {
		usart_rx_buffer[usart_rx_head] = received_data;
		usart_rx_head = next_head; // Schreibindex aktualisieren
		} else {
		// Puffer voll - Fehlerhandling (optional)
		USART_SendString("Fehler: RX-Buffer voll!\n");
	}
}

// Daten aus dem Empfangspuffer lesen
uint8_t USART_ReadData() 
{
	if (usart_rx_head == usart_rx_tail) {
		return 0; // Kein neues Zeichen verfügbar
	}

	uint8_t data = usart_rx_buffer[usart_rx_tail];
	usart_rx_tail = (usart_rx_tail + 1) % USART_BUFFER_SIZE;

	return data;
}

// Prüft, ob Daten im Empfangspuffer vorhanden sind
uint8_t USART_DataAvailable() {
	return (usart_rx_head != usart_rx_tail);  // True, wenn Daten im Puffer vorhanden sind
}

// String aus dem Empfangspuffer lesen
void USART_ReadString(char *buffer, uint8_t max_length) {
	uint8_t i = 0;

	// Daten aus dem Ringbuffer holen, bis \n gefunden wird oder max. Länge erreicht ist
	while (USART_DataAvailable() && i < max_length - 1) {
		buffer[i] = USART_ReadData();
		if (buffer[i] == '\n') {  // Falls Endezeichen gefunden wird
			break;
		}
		i++;
	}

	buffer[i] = '\0';  // Null-Terminierung hinzufügen
}