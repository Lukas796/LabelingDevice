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
#include "systemstate.h" 
#include "motor_control.h"       

#define USART_BUFFER_SIZE 64														// Definiert die Größe des Ringpuffers für empfangene Daten  

volatile uint8_t usart_rx_buffer[USART_BUFFER_SIZE];								// Ringpuffer zur Speicherung empfangener Daten  
volatile uint8_t usart_rx_head = 0;													// Index für den nächsten Schreibzugriff im Puffer  
volatile uint8_t usart_rx_tail = 0;													// Index für den nächsten Lesezugriff im Puffer  
char LCDstr[17];																	// Puffer für LCD-Ausgaben (16 Zeichen + Nullterminator)  

// USART initialisieren  
void USART_Init(uint16_t baud)														// Initialisiert die USART-Schnittstelle  
{  
	UBRR0L = 103;																	// Setzt den niedrigen Teil der Baudrateneinstellung (z.B. 103 für ca. 9600 Baud bei 16MHz)  
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);							// Aktiviert Empfänger, Sender und den RX-Interrupt  
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);										// Konfiguriert das Datenformat: 8 Datenbits, keine Parität, 1 Stopbit  
	sei();																			// Aktiviert globale Interrupts  
}  

// Empfängt und verarbeitet UART-Befehle  
void USART_ProcessCommands(uint8_t* messung_aktiv, uint8_t* pos_aktiv)									// Verarbeitet empfangene Befehle; "messung_aktiv" zeigt an, ob Messung läuft  
{  
	if (USART_DataAvailable()) {													// Prüft, ob Daten im Empfangspuffer vorhanden sind  
		char buffer[30];															// Temporärer Puffer für den empfangenen Befehl  
		USART_ReadString(buffer, sizeof(buffer));									// Liest einen String aus dem Ringpuffer und speichert ihn in "buffer"  

		if (strcmp(buffer, "M") == 0) {												// Wenn der Befehl "M" empfangen wurde  
			*messung_aktiv = 1;														// Setze Messung aktiv  
			} else if (strcmp(buffer, "MN") == 0) {									// Wenn der Befehl "MN" empfangen wurde  
			*messung_aktiv = 0;														// Setze Messung inaktiv  
			} else if (strcmp(buffer, "R") == 0) {									// Falls Befehl "R" (Referenzfahrt) erkannt wurde  
			reference_StartPos_control();											// Führt Referenzpositions-Kontrolle durch  
			USART_SendString("Referenzfahrt gestartet.\n");							// Sendet entsprechende Bestätigung über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile (Adresse 0xC0)  
			lcd_text("Referenzfahrt!  ");											// Schreibt "Referenzfahrt!  " auf das LCD, mit zusätzlichen Leerzeichen zum Überschreiben  
			} else if (strcmp(buffer, "STOP") == 0) {								// Falls Befehl "STOP" erkannt wurde  
			USART_SendString("Notstop.\n");											// Sendet "Notstop." über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor wieder auf die zweite Zeile  
			lcd_text("Notstop!        ");											// Schreibt "Notstop!" mit Leerzeichen, um die gesamte Zeile zu überschreiben  
			} else if (strcmp(buffer, "START") == 0) {								// Falls Befehl "START" erkannt wurde  
			USART_SendString("Wird gestartet.\n");									// Sendet die Nachricht "Wird gestartet." über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile  
			lcd_text("Wird gestartet! ");											// Schreibt "Wird gestartet!" auf das LCD, mit Leerzeichen zum Auffüllen  
			} else if (strstr(buffer, "Beschriftung:") != 0) {						// Falls der Befehl "Beschriftung:" im empfangenen String enthalten ist  
			USART_SendString("Folgender Text wird geschrieben: ");					// Sendet eine Information über USART  
			char* text_start = buffer + strlen("Beschriftung:");					// Ermittelt die Position, wo der eigentliche Text beginnt  
			USART_SendString(text_start);											// Sendet den ausgelesenen Textteil  
			USART_SendString("\n");													// Sendet einen Zeilenumbruch  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile  
			char send_Text_buffer[17];												// Puffer für die formatierte Ausgabe auf dem LCD (16 Zeichen + Nullterminator)  
			snprintf(send_Text_buffer, sizeof(send_Text_buffer), "%-16s",text_start); // Formatiert den Text linksbündig, füllt ggf. mit Leerzeichen auf  
			lcd_text(send_Text_buffer);												// Zeigt den formatierten Text auf dem LCD an  
			} else if (strstr(buffer, "POS") != 0){									// Wenn der Befehl "POS" empfangen wurde
			*pos_aktiv = 1;															// Setze POS aktiv
			} else if (strcmp(buffer, "NPOS") == 0) {								// Wenn der Befehl "NPOS" empfangen wurde
			*pos_aktiv = 0;
	}  
}  

// Messungsprozess  
void USART_MESSUNG(uint8_t messung_aktiv) {											// Führt den Messvorgang durch, wenn die Messung aktiv ist  
	if (messung_aktiv) {															// Überprüft, ob Messungen aktiviert sind  
		uint16_t distance = laser_read();											// Liest den Abstandswert vom Lasersensor ein  
		char send_buffer[20];														// Puffer für den über USART gesendeten String  
		snprintf(send_buffer, sizeof(send_buffer), "%u mm\n", distance);			// Formatiert den Abstandswert (z.B. "123 mm\n")  
		USART_SendString(send_buffer);												// Sendet den formatierten String über die serielle Schnittstelle  
		uint16_t i = laser_read();													// Liest den Messwert erneut aus (zur Umwandlung in eine LCD-kompatible Zeichenkette)  
		lcd_cmd (0xC0);																// Setzt den LCD-Cursor auf die zweite Zeile  
		lcd_num (i, LCDstr);														// Konvertiert den Messwert in einen String und speichert diesen in LCDstr  
		char send_Messung_buffer[17];												// Puffer für die LCD-Ausgabe, auf 16 Zeichen formatiert (plus Nullterminator)  
		snprintf(send_Messung_buffer, sizeof(send_Messung_buffer), "%-16s",LCDstr); // Formatiert den Inhalt von LCDstr linksbündig, füllt mit Leerzeichen auf  
		lcd_text(send_Messung_buffer);												// Zeigt die formatierte Messung auf dem LCD an  
	}  
}

// Positionsprozess: Führt den Positionsprozess durch, wenn die Positionierung aktiv ist

void USART_POSITIONIERUNG(uint8_t pos_aktiv) {  // pos_aktiv steuert, ob die Positionsausgabe aktiv ist
	if (pos_aktiv) {                             // Nur wenn Positionierung aktiviert ist...
		// Erstelle einen String mit den drei Positionswerten und sende ihn via USART
		char send_buffer[40];  // Puffer für die serielle Ausgabe
		snprintf(send_buffer, sizeof(send_buffer), "X:%u Y:%u Z:%u\n", act_Pos_x, act_Pos_y, act_Pos_z);
		USART_SendString(send_buffer);           // Sende den formatierten String über die serielle Schnittstelle

		// Aktualisiere das LCD-Display: Da das LCD oft nur 16 Zeichen hat, zeigen wir hier z.B. ausschliesslich X und Z an
		lcd_cmd(0xC0);                           // Setzt den LCD-Cursor auf die zweite Zeile
		char lcd_buffer[17];                     // Puffer für die LCD-Ausgabe (16 Zeichen + Nullterminator)
		snprintf(lcd_buffer, sizeof(lcd_buffer), "X:%u Z:%u", act_Pos_x, act_Pos_z);  // Formatiert die Anzeige
		lcd_text(lcd_buffer);                    // Zeigt die formatierte Ausgabe auf dem LCD an
	}
}

// Daten senden  
void USART_SendData(uint8_t data) {													// Sendet ein einzelnes Byte über die USART-Schnittstelle  
	while (!(UCSR0A & (1 << UDRE0)));												// Wartet, bis das USART-Datenregister leer ist (bereit zur Übertragung)  
	UDR0 = data;																	// Schreibt das Datenbyte in das Datenregister, wodurch die Übertragung gestartet wird  
}  

// String senden  
void USART_SendString(const char *str) {											// Sendet einen nullterminierten String über USART  
	while (*str) {																	// Solange das aktuelle Zeichen nicht der Nullterminator ist  
		USART_SendData(*str++);														// Sendet das Zeichen und inkrementiert den Zeiger auf den nächsten Buchstaben  
	}  
}  

// Empfangs-Interrupt (ISR)  
ISR(USART0_RX_vect) {																// Interrupt-Service-Routine, die aufgerufen wird, wenn ein Zeichen empfangen wurde  
	uint8_t received_data = UDR0;													// Liest das empfangene Zeichen aus dem USART-Datenregister  
	uint8_t next_head = (usart_rx_head + 1) % USART_BUFFER_SIZE;					// Berechnet den nächsten Index im Ringpuffer (unter Berücksichtigung des Wrap-arounds)  
																					// Überprüfen, ob der Puffer voll ist  
	if (next_head != usart_rx_tail) {												// Prüft, ob noch Platz im Buffer ist  
		usart_rx_buffer[usart_rx_head] = received_data;								// Speichert das empfangene Zeichen im Ringpuffer  
		usart_rx_head = next_head;													// Aktualisiert den Schreibindex des Puffers  
		} else {  
																					// Puffer voll - Fehlerhandling (optional)  
		USART_SendString("Fehler: RX-Buffer voll!\n");								// Meldet über USART, dass der Empfangspuffer voll ist  
	}  
}  

// Daten aus dem Empfangspuffer lesen  
uint8_t USART_ReadData() {															// Liest ein Zeichen aus dem USART-Ringpuffer  
	if (usart_rx_head == usart_rx_tail) {											// Prüft, ob der Puffer leer ist  
		return 0;																	// Gibt 0 zurück, da kein neues Zeichen verfügbar ist  
	}  
	uint8_t data = usart_rx_buffer[usart_rx_tail];									// Holt das Zeichen an der aktuellen Tail-Position aus dem Puffer  
	usart_rx_tail = (usart_rx_tail + 1) % USART_BUFFER_SIZE;						// Erhöht den Lese-Index und berücksichtigt den Wrap-around  
	return data;																	// Gibt das gelesene Zeichen zurück  
}  

// Prüft, ob Daten im Empfangspuffer vorhanden sind  
uint8_t USART_DataAvailable() {														// Liefert true (ungleich 0) zurück, wenn im Ringpuffer Daten vorliegen  
	return (usart_rx_head != usart_rx_tail);										// Vergleicht Schreib- und Leseindex; weicht bei vorhandenen Daten voneinander ab  
}  

// String aus dem Empfangspuffer lesen  
void USART_ReadString(char *buffer, uint8_t max_length) {							// Liest einen String aus dem Empfangspuffer bis zum Zeilenumbruch oder zur maximalen Länge  
	uint8_t i = 0;																	// Initialisiert den Index für den Zielpuffer  
																					// Daten aus dem Ringbuffer holen, bis \n gefunden wird oder max. Länge erreicht ist  
	while (USART_DataAvailable() && i < max_length - 1) {							// Liest weiter, solange Daten vorhanden sind und Platz im Puffer ist  
		buffer[i] = USART_ReadData();												// Liest ein Zeichen und speichert es im Puffer  
		if (buffer[i] == '\n') {													// Falls das Ende des Befehls (Zeilenumbruch) erreicht wird  
			break;																	// Beendet das Einlesen  
		}  
		i++;																		// Erhöht den Index für das nächste Zeichen  
	}  
	buffer[i] = '\0';																// Fügt den Nullterminator an, um den String abzuschließen  
}