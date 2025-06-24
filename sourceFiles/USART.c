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

#define USART_BUFFER_SIZE 64														// Definiert die Gr��e des Ringpuffers f�r empfangene Daten  

volatile uint8_t usart_rx_buffer[USART_BUFFER_SIZE];								// Ringpuffer zur Speicherung empfangener Daten  
volatile uint8_t usart_rx_head = 0;													// Index f�r den n�chsten Schreibzugriff im Puffer  
volatile uint8_t usart_rx_tail = 0;													// Index f�r den n�chsten Lesezugriff im Puffer  
char LCDstr[17];																	// Puffer f�r LCD-Ausgaben (16 Zeichen + Nullterminator)  

// USART initialisieren  
void USART_Init(uint16_t baud)														// Initialisiert die USART-Schnittstelle  
{  
	UBRR0L = 103;																	// Setzt den niedrigen Teil der Baudrateneinstellung (z.B. 103 f�r ca. 9600 Baud bei 16MHz)  
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);							// Aktiviert Empf�nger, Sender und den RX-Interrupt  
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);										// Konfiguriert das Datenformat: 8 Datenbits, keine Parit�t, 1 Stopbit  
	sei();																			// Aktiviert globale Interrupts  
}  

// Empf�ngt und verarbeitet UART-Befehle  
void USART_ProcessCommands(uint8_t* messung_aktiv, uint8_t* pos_aktiv)									// Verarbeitet empfangene Befehle; "messung_aktiv" zeigt an, ob Messung l�uft  
{  
	if (USART_DataAvailable()) {													// Pr�ft, ob Daten im Empfangspuffer vorhanden sind  
		char buffer[30];															// Tempor�rer Puffer f�r den empfangenen Befehl  
		USART_ReadString(buffer, sizeof(buffer));									// Liest einen String aus dem Ringpuffer und speichert ihn in "buffer"  

		if (strcmp(buffer, "M") == 0) {												// Wenn der Befehl "M" empfangen wurde  
			*messung_aktiv = 1;														// Setze Messung aktiv  
			} else if (strcmp(buffer, "MN") == 0) {									// Wenn der Befehl "MN" empfangen wurde  
			*messung_aktiv = 0;														// Setze Messung inaktiv  
			} else if (strcmp(buffer, "R") == 0) {									// Falls Befehl "R" (Referenzfahrt) erkannt wurde  
			reference_StartPos_control();											// F�hrt Referenzpositions-Kontrolle durch  
			USART_SendString("Referenzfahrt gestartet.\n");							// Sendet entsprechende Best�tigung �ber USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile (Adresse 0xC0)  
			lcd_text("Referenzfahrt!  ");											// Schreibt "Referenzfahrt!  " auf das LCD, mit zus�tzlichen Leerzeichen zum �berschreiben  
			} else if (strcmp(buffer, "STOP") == 0) {								// Falls Befehl "STOP" erkannt wurde  
			USART_SendString("Notstop.\n");											// Sendet "Notstop." �ber USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor wieder auf die zweite Zeile  
			lcd_text("Notstop!        ");											// Schreibt "Notstop!" mit Leerzeichen, um die gesamte Zeile zu �berschreiben  
			} else if (strcmp(buffer, "START") == 0) {								// Falls Befehl "START" erkannt wurde  
			USART_SendString("Wird gestartet.\n");									// Sendet die Nachricht "Wird gestartet." �ber USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile  
			lcd_text("Wird gestartet! ");											// Schreibt "Wird gestartet!" auf das LCD, mit Leerzeichen zum Auff�llen  
			} else if (strstr(buffer, "Beschriftung:") != 0) {						// Falls der Befehl "Beschriftung:" im empfangenen String enthalten ist  
			USART_SendString("Folgender Text wird geschrieben: ");					// Sendet eine Information �ber USART  
			char* text_start = buffer + strlen("Beschriftung:");					// Ermittelt die Position, wo der eigentliche Text beginnt  
			USART_SendString(text_start);											// Sendet den ausgelesenen Textteil  
			USART_SendString("\n");													// Sendet einen Zeilenumbruch  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile  
			char send_Text_buffer[17];												// Puffer f�r die formatierte Ausgabe auf dem LCD (16 Zeichen + Nullterminator)  
			snprintf(send_Text_buffer, sizeof(send_Text_buffer), "%-16s",text_start); // Formatiert den Text linksb�ndig, f�llt ggf. mit Leerzeichen auf  
			lcd_text(send_Text_buffer);												// Zeigt den formatierten Text auf dem LCD an  
			} else if (strstr(buffer, "POS") != 0){									// Wenn der Befehl "POS" empfangen wurde
			*pos_aktiv = 1;															// Setze POS aktiv
			} else if (strcmp(buffer, "NPOS") == 0) {								// Wenn der Befehl "NPOS" empfangen wurde
			*pos_aktiv = 0;
	}  
}  

// Messungsprozess  
void USART_MESSUNG(uint8_t messung_aktiv) {											// F�hrt den Messvorgang durch, wenn die Messung aktiv ist  
	if (messung_aktiv) {															// �berpr�ft, ob Messungen aktiviert sind  
		uint16_t distance = laser_read();											// Liest den Abstandswert vom Lasersensor ein  
		char send_buffer[20];														// Puffer f�r den �ber USART gesendeten String  
		snprintf(send_buffer, sizeof(send_buffer), "%u mm\n", distance);			// Formatiert den Abstandswert (z.B. "123 mm\n")  
		USART_SendString(send_buffer);												// Sendet den formatierten String �ber die serielle Schnittstelle  
		uint16_t i = laser_read();													// Liest den Messwert erneut aus (zur Umwandlung in eine LCD-kompatible Zeichenkette)  
		lcd_cmd (0xC0);																// Setzt den LCD-Cursor auf die zweite Zeile  
		lcd_num (i, LCDstr);														// Konvertiert den Messwert in einen String und speichert diesen in LCDstr  
		char send_Messung_buffer[17];												// Puffer f�r die LCD-Ausgabe, auf 16 Zeichen formatiert (plus Nullterminator)  
		snprintf(send_Messung_buffer, sizeof(send_Messung_buffer), "%-16s",LCDstr); // Formatiert den Inhalt von LCDstr linksb�ndig, f�llt mit Leerzeichen auf  
		lcd_text(send_Messung_buffer);												// Zeigt die formatierte Messung auf dem LCD an  
	}  
}

// Positionsprozess: F�hrt den Positionsprozess durch, wenn die Positionierung aktiv ist

void USART_POSITIONIERUNG(uint8_t pos_aktiv) {  // pos_aktiv steuert, ob die Positionsausgabe aktiv ist
	if (pos_aktiv) {                             // Nur wenn Positionierung aktiviert ist...
		// Erstelle einen String mit den drei Positionswerten und sende ihn via USART
		char send_buffer[40];  // Puffer f�r die serielle Ausgabe
		snprintf(send_buffer, sizeof(send_buffer), "X:%u Y:%u Z:%u\n", act_Pos_x, act_Pos_y, act_Pos_z);
		USART_SendString(send_buffer);           // Sende den formatierten String �ber die serielle Schnittstelle

		// Aktualisiere das LCD-Display: Da das LCD oft nur 16 Zeichen hat, zeigen wir hier z.B. ausschliesslich X und Z an
		lcd_cmd(0xC0);                           // Setzt den LCD-Cursor auf die zweite Zeile
		char lcd_buffer[17];                     // Puffer f�r die LCD-Ausgabe (16 Zeichen + Nullterminator)
		snprintf(lcd_buffer, sizeof(lcd_buffer), "X:%u Z:%u", act_Pos_x, act_Pos_z);  // Formatiert die Anzeige
		lcd_text(lcd_buffer);                    // Zeigt die formatierte Ausgabe auf dem LCD an
	}
}

// Daten senden  
void USART_SendData(uint8_t data) {													// Sendet ein einzelnes Byte �ber die USART-Schnittstelle  
	while (!(UCSR0A & (1 << UDRE0)));												// Wartet, bis das USART-Datenregister leer ist (bereit zur �bertragung)  
	UDR0 = data;																	// Schreibt das Datenbyte in das Datenregister, wodurch die �bertragung gestartet wird  
}  

// String senden  
void USART_SendString(const char *str) {											// Sendet einen nullterminierten String �ber USART  
	while (*str) {																	// Solange das aktuelle Zeichen nicht der Nullterminator ist  
		USART_SendData(*str++);														// Sendet das Zeichen und inkrementiert den Zeiger auf den n�chsten Buchstaben  
	}  
}  

// Empfangs-Interrupt (ISR)  
ISR(USART0_RX_vect) {																// Interrupt-Service-Routine, die aufgerufen wird, wenn ein Zeichen empfangen wurde  
	uint8_t received_data = UDR0;													// Liest das empfangene Zeichen aus dem USART-Datenregister  
	uint8_t next_head = (usart_rx_head + 1) % USART_BUFFER_SIZE;					// Berechnet den n�chsten Index im Ringpuffer (unter Ber�cksichtigung des Wrap-arounds)  
																					// �berpr�fen, ob der Puffer voll ist  
	if (next_head != usart_rx_tail) {												// Pr�ft, ob noch Platz im Buffer ist  
		usart_rx_buffer[usart_rx_head] = received_data;								// Speichert das empfangene Zeichen im Ringpuffer  
		usart_rx_head = next_head;													// Aktualisiert den Schreibindex des Puffers  
		} else {  
																					// Puffer voll - Fehlerhandling (optional)  
		USART_SendString("Fehler: RX-Buffer voll!\n");								// Meldet �ber USART, dass der Empfangspuffer voll ist  
	}  
}  

// Daten aus dem Empfangspuffer lesen  
uint8_t USART_ReadData() {															// Liest ein Zeichen aus dem USART-Ringpuffer  
	if (usart_rx_head == usart_rx_tail) {											// Pr�ft, ob der Puffer leer ist  
		return 0;																	// Gibt 0 zur�ck, da kein neues Zeichen verf�gbar ist  
	}  
	uint8_t data = usart_rx_buffer[usart_rx_tail];									// Holt das Zeichen an der aktuellen Tail-Position aus dem Puffer  
	usart_rx_tail = (usart_rx_tail + 1) % USART_BUFFER_SIZE;						// Erh�ht den Lese-Index und ber�cksichtigt den Wrap-around  
	return data;																	// Gibt das gelesene Zeichen zur�ck  
}  

// Pr�ft, ob Daten im Empfangspuffer vorhanden sind  
uint8_t USART_DataAvailable() {														// Liefert true (ungleich 0) zur�ck, wenn im Ringpuffer Daten vorliegen  
	return (usart_rx_head != usart_rx_tail);										// Vergleicht Schreib- und Leseindex; weicht bei vorhandenen Daten voneinander ab  
}  

// String aus dem Empfangspuffer lesen  
void USART_ReadString(char *buffer, uint8_t max_length) {							// Liest einen String aus dem Empfangspuffer bis zum Zeilenumbruch oder zur maximalen L�nge  
	uint8_t i = 0;																	// Initialisiert den Index f�r den Zielpuffer  
																					// Daten aus dem Ringbuffer holen, bis \n gefunden wird oder max. L�nge erreicht ist  
	while (USART_DataAvailable() && i < max_length - 1) {							// Liest weiter, solange Daten vorhanden sind und Platz im Puffer ist  
		buffer[i] = USART_ReadData();												// Liest ein Zeichen und speichert es im Puffer  
		if (buffer[i] == '\n') {													// Falls das Ende des Befehls (Zeilenumbruch) erreicht wird  
			break;																	// Beendet das Einlesen  
		}  
		i++;																		// Erh�ht den Index f�r das n�chste Zeichen  
	}  
	buffer[i] = '\0';																// F�gt den Nullterminator an, um den String abzuschlie�en  
}