/*
 * USART.h
 *
 * Created: 28.05.2025 10:30:28
 *  Author: robin
 */ 


#ifndef USART_H_
#define USART_H_

#include "config.h" // need to be first

void USART_Init(uint16_t ubrr_value);               // Initialisiert die USART-Schnittstelle mit dem UBRR-Wert für die Baudrate
void USART_ProcessCommands(uint8_t* messung_aktiv); // Liest und verarbeitet eingehende Kommandos, steuert messung_aktiv
void USART_MESSUNG(uint8_t messung_aktiv);          // Führt die Messroutine aus, wenn messung_aktiv gesetzt ist
void USART_SendData(uint8_t data);                  // Sendet ein einzelnes Byte über USART
void USART_SendString(const char *str);             // Sendet eine Null-terminierte Zeichenkette über USART
uint8_t USART_ReadData(void);                       // Liest ein empfangenes Byte aus dem USART-Datenregister
uint8_t USART_DataAvailable(void);                  // Prüft, ob im USART-Empfangspuffer Daten verfügbar sind
ISR(USART0_RX_vect);                                // Deklaration der Interrupt-Service-Routine für USART0-Empfang

extern volatile uint8_t pos_aktiv;                  // Flag to indicate if a measurement or position is active
extern char texto_buffer[50];                       // Puffer für alle eingehenden Zeilen (texto)
extern char *texto_start;                           // Zeiger auf den reinen Text im text o -Puffer
const char* USART_GetTexto(void);                   // Gibt den Zeiger auf texto_start zurück

extern char textu_buffer[50];                       // Puffer für alle eingehenden Zeilen (textu)
extern char *textu_start;                           // Zeiger auf den reinen Text im text u -Puffer
const char* USART_GetTextu(void);                   // Gibt den Zeiger auf textu_start zurück





#endif /* USART_H_ */