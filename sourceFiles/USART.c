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
#include "letter_control.h"   

#define USART_BUFFER_SIZE 64														// Definiert die Größe des Ringpuffers für empfangene Daten  

volatile uint8_t usart_rx_buffer[USART_BUFFER_SIZE];								// Ringpuffer zur Speicherung empfangener Daten  
volatile uint8_t usart_rx_head = 0;													// Index für den nächsten Schreibzugriff im Puffer  
volatile uint8_t usart_rx_tail = 0;													// Index für den nächsten Lesezugriff im Puffer  
volatile uint8_t pos_aktiv = 0;

char LCDstr[17];																	// Puffer für LCD-Ausgaben (16 Zeichen + Nullterminator)  
char texto_buffer[50];																// Puffer für Texto
char *texto_start = NULL;
char textu_buffer[50];																// Puffer für Textu
char *textu_start = NULL;

const char* USART_GetTexto(void) {													// Liefert Zeiger auf den Anfang des texto-Puffers
	return texto_start;																// Rückgabe des Pointers texto_start
}

const char* USART_GetTextu(void) {													// Liefert Zeiger auf den Anfang des textu-Puffers
	return textu_start;																// Rückgabe des Pointers textu_start
}


// USART initialisieren  
void USART_Init(uint16_t ubrr_value) {
	// 1) Double Speed-Modus für geringeren Fehler aktivieren
	UCSR0A |= (1 << U2X0);

	// 2) UBRR0 High und Low setzen
	UBRR0H = (uint8_t)(ubrr_value >> 8);
	UBRR0L = (uint8_t)ubrr_value;

	// 3) RX, TX und RX-Interrupt aktivieren
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

	// 4) Frame-Format: 8 Datenbit, gerade Parität, 1 Stopbit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << UPM01);

	sei();  // globale Interrupts an
}

// Empfängt und verarbeitet UART-Befehle  
void USART_ProcessCommands(uint8_t* messung_aktiv)				// Verarbeitet empfangene Befehle; "messung_aktiv" zeigt an, ob Messung läuft  
{  
	if (USART_DataAvailable()) {
		char command_buffer[30];												// Prüft, ob Daten im Empfangspuffer vorhanden sind  
		USART_ReadString(command_buffer, sizeof(command_buffer));					// Liest einen String aus dem Ringpuffer und speichert ihn in "buffer"   
		if (strcmp(command_buffer, "M") == 0) {										// Wenn der Befehl "M" empfangen wurde  
			*messung_aktiv = 1;														// Setze Messung aktiv  
			} 
		if (strcmp(command_buffer, "MN") == 0) {							// Wenn der Befehl "MN" empfangen wurde  
			*messung_aktiv = 0;														// Setze Messung inaktiv  
			} 
		if (strcmp(command_buffer, "R") == 0) {							// Falls Befehl "R" (Referenzfahrt) erkannt wurde  
			request_reference_start(1);												// Führt Referenzfahrt durch  
			USART_SendString("Referenzfahrt gestartet.\n");							// Sendet entsprechende Bestätigung über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile (Adresse 0xC0)  
			lcd_text("Referenzfahrt!  ");											// Schreibt "Referenzfahrt!  " auf das LCD, mit zusätzlichen Leerzeichen zum Überschreiben  
			} 
		if (strcmp(command_buffer, "STOP") == 0) {						// Falls Befehl "STOP" erkannt wurde  
			USART_SendString("Notstop.\n");											// Sendet "Notstop." über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor wieder auf die zweite Zeile  
			lcd_text("Notstop!        \n");											// Schreibt "Notstop!" mit Leerzeichen, um die gesamte Zeile zu überschreiben  
			} 
		if (strcmp(command_buffer, "START") == 0) {						// Falls Befehl "START" erkannt wurde  
			request_Labeling_start(1);												//Setzt Flag für Start
			USART_SendString("Wird gestartet.\n");									//Send String
			pos_aktiv = 1;															//Pos-Flag setzen
			USART_SendString("POS\n");									// Sendet die Nachricht "Wird gestartet." über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile  
			lcd_text("Wird gestartet! \n");											// Schreibt "Wird gestartet!" auf das LCD, mit Leerzeichen zum Auffüllen  
			} 
		if (strcmp(command_buffer, "POS") == 0){							// Wenn der Befehl "POS" empfangen wurde
			//pos_aktiv = 1;															// Setze POS aktiv
			USART_SendString("POS\n");													// Send String POS
			} 
		if (strcmp(command_buffer, "NPOS") == 0) {						// Wenn der Befehl "NPOS" empfangen wurde
			//pos_aktiv = 0;
			USART_SendString("NPOS\n");												//Send String NPOS
			} 
		if (strstr(command_buffer, ":Texto")!= 0) {						// Falls der Befehl "Text:" im empfangenen String enthalten ist  
			USART_SendString("Zeile 1 wird geschrieben:\n");
			command_buffer[strcspn(command_buffer, "\r\n")] = '\0';
			command_buffer[strcspn(command_buffer, ":")] = '\0';					// Entfernt alles ab ":"
			strcpy(texto_buffer, command_buffer);								// Kopiert damit durch START nachfolgend nicht überschieben wird
			texto_start = texto_buffer;												// Ermittelt die Position, wo der eigentliche Text beginnt
			//USART_SendString(texto_start);											// Sendet den ausgelesenen Textteil
			//USART_SendString("\n");													// Sendet einen Zeilenumbruch
			lcd_cmd(0x80);															// Setzt den LCD-Cursor auf die zweite Zeile
			char send_Texto_buffer[17];												// Puffer für die formatierte Ausgabe auf dem LCD (16 Zeichen + Nullterminator)
			snprintf(send_Texto_buffer, sizeof(send_Texto_buffer), "%-16s",texto_start);// Formatiert den Text linksbündig, füllt ggf. mit Leerzeichen auf
			lcd_text(send_Texto_buffer);											// Zeigt den formatierten Text auf dem LCD an
			USART_SendString(send_Texto_buffer);
			} 
		if (strstr(command_buffer, ":Textu")!= 0) {						// Falls der Befehl "Text:" im empfangenen String enthalten ist
			USART_SendString("Zeile 2 wird geschrieben:\n");
			command_buffer[strcspn(command_buffer, "\r\n")] = '\0';
			command_buffer[strcspn(command_buffer, ":")] = '\0';					// Entfernt alles ab ":"
			strcpy(textu_buffer, command_buffer);								// Kopiert damit durch START nachfolgend nicht überschieben wird
			textu_start = textu_buffer;												// Ermittelt die Position, wo der eigentliche Text beginnt
			//USART_SendString(textu_start);											// Sendet den ausgelesenen Textteil
			//USART_SendString("\n");													// Sendet einen Zeilenumbruch
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile
			char send_Textu_buffer[17];												// Puffer für die formatierte Ausgabe auf dem LCD (16 Zeichen + Nullterminator)
			snprintf(send_Textu_buffer, sizeof(send_Textu_buffer), "%-16s",textu_start);// Formatiert den Text linksbündig, füllt ggf. mit Leerzeichen auf
			lcd_text(send_Textu_buffer);
			USART_SendString(send_Textu_buffer);
			}
	}  
}  

// Messungsprozess  
void USART_MESSUNG(uint8_t messung_aktiv) {											// Führt den Messvorgang durch, wenn die Messung aktiv ist  
	if (messung_aktiv) {															// Überprüft, ob Messungen aktiviert sind  
		uint16_t distance = laser_read();											// Liest den Abstandswert vom Lasersensor ein  
		char send_buffer[20];														// Puffer für den über USART gesendeten String  
		snprintf(send_buffer, sizeof(send_buffer), "%u mm\r\n", distance);			// Formatiert den Abstandswert (z.B. "123 mm\n")  
		USART_SendString(send_buffer);												// Sendet den formatierten String über die serielle Schnittstelle    
		lcd_cmd (0xC0);																// Setzt den LCD-Cursor auf die zweite Zeile  
		lcd_num (distance, LCDstr);														// Konvertiert den Messwert in einen String und speichert diesen in LCDstr  
		char send_Messung_buffer[17];												// Puffer für die LCD-Ausgabe, auf 16 Zeichen formatiert (plus Nullterminator)  
		snprintf(send_Messung_buffer, sizeof(send_Messung_buffer), "%-16s",LCDstr); // Formatiert den Inhalt von LCDstr linksbündig, füllt mit Leerzeichen auf  
		lcd_text(send_Messung_buffer);												// Zeigt die formatierte Messung auf dem LCD an 
	}  
}

// Positionsprozess: Führt den Positionsprozess durch, wenn die Positionierung aktiv ist

void USART_POSITIONIERUNG(uint8_t state) {  // pos_aktiv steuert, ob die Positionsausgabe aktiv ist
	
	uint16_t xPos = act_Pos_x();	// aktuelle XPosition von Motorsteuerung holen 
	uint16_t yPos = act_Pos_y();	// aktuelle YPosition von Motorsteuerung holen 
	uint16_t zPos = act_Pos_z();	// aktuelle ZPosition von Motorsteuerung holen 
	
	// Erstelle einen String mit den drei Positionswerten und sende ihn via USART
	char send_buffer_pos[20];  // Puffer für die serielle Ausgabe
	snprintf(send_buffer_pos, sizeof(send_buffer_pos), "X:%u Y:%u Z:%u",xPos , yPos, zPos);
	
	USART_SendString(send_buffer_pos);           // Sende den formatierten String über die serielle Schnittstelle
	USART_SendString("\r\n");	
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
ISR(USART0_RX_vect) {														// ISR für empfangene Daten an USART0
	uint8_t status = UCSR0A;												// Lese Statusregister, um Fehlerflags zu prüfen
	uint8_t data   = UDR0;													// Lese UDR0, um das RxC-Flag zurückzusetzen

	if (status & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))) {				// Prüfe auf Frame-, Overrun- oder Paritätsfehler
		// Frame Error FE0, Overrun Error DOR0 oder Parity Error UPE0 aufgetreten
		// hier z.B. verwerfen oder zurückmelden:
		USART_SendString("UART Error: ");									// Sende Fehlermeldungspräfix
		if (status & (1 << UPE0)) USART_SendString("Parity ");				// Meldung: Parity-Fehler
		if (status & (1 << FE0))  USART_SendString("Frame ");				// Meldung: Frame-Error
		if (status & (1 << DOR0)) USART_SendString("Overrun ");				// Meldung: Overrun-Error
		USART_SendString("error\n");										// Abschließende Fehlermeldung mit Zeilenumbruch
		return;																// Beende ISR ohne Pufferung bei Fehler
	}                                                                                      

	// sonst ins Ringbuffer:
	uint8_t next = (usart_rx_head + 1) % USART_BUFFER_SIZE; // Berechnet Index der nächsten Pufferstelle im Ringpuffer
	if (next != usart_rx_tail) {							// Prüft, ob der Puffer nicht überläuft (Head ≠ Tail)
		usart_rx_buffer[usart_rx_head] = data;				// Legt das empfangene Byte an der aktuellen Head-Position ab
		usart_rx_head = next;								// Setzt den Head-Zeiger auf die soeben beschriebene Position
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


void USART_ReadString(char *buffer, uint8_t max_length) {							// Liest eine Zeichenkette von der USART-Schnittstelle ein
	uint8_t i = 0;																	// Initialisiert den Puffer-Index
	char c;																			// Variable für das gelesene Zeichen
	do {																			// Beginnt Schleife zum Einlesen bis '\n'
		while (!USART_DataAvailable()) {}											// Warte auf das nächste empfangene Byte
		c = USART_ReadData();														// Lese das nächste Byte aus dem USART
		if (c == '\r') continue;													// Überspringe Carriage Return (CR)
		if (i < max_length-1) buffer[i++] = c;										// Speichere Zeichen im Puffer, solange Platz ist
	} while (c != '\n');															// Wiederhole, bis Line Feed (LF) empfangen wurde
	buffer[i-1] = '\0';																// Ersetze das abschließende '\n' durch Null-Terminator
}																					// Ende der Funktion
