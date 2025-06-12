/*
 * USART.c
 *
 * Created: 28.05.2025 10:30:44
 *  Author: robin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"

#define USART_BUFFER_SIZE 64 // Größe des Ringbuffers

volatile uint8_t usart_rx_buffer[USART_BUFFER_SIZE]; // Empfangspuffer
volatile uint8_t usart_rx_head = 0; // Schreibindex des Puffers
volatile uint8_t usart_rx_tail = 0; // Leseindex des Puffers

// USART initialisieren
void USART_Init(uint16_t baud) 
{
	//uint16_t ubrr_value = (F_CPU / (16UL * baud)) - 1; // UBBR-Wert berechnen bei 16.MIo und 96000 ~ 7

	//UBRR0H = (uint8_t)(ubrr_value >> 8); 
	//UBRR0L = (uint8_t)ubrr_value;
	UBRR0L = 103;

	// RX & TX aktivieren, RX-Interrupt freigeben
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

	// Asynchroner Modus, 8 Datenbits, keine Parität, 1 Stop-Bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	sei(); // Globale Interrupts aktivieren
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