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
char texto_buffer[50];
char *texto_start = NULL;
char textu_buffer[50];
char *textu_start = NULL;

const char* USART_GetTexto(void) {
	return texto_start;
}

const char* USART_GetTextu(void) {
	return textu_start;
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
			} else if (strcmp(command_buffer, "MN") == 0) {							// Wenn der Befehl "MN" empfangen wurde  
			*messung_aktiv = 0;														// Setze Messung inaktiv  
			} else if (strcmp(command_buffer, "R") == 0) {							// Falls Befehl "R" (Referenzfahrt) erkannt wurde  
			request_reference_start(1);												// Führt Referenzfahrt durch  
			USART_SendString("Referenzfahrt gestartet.\n");							// Sendet entsprechende Bestätigung über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile (Adresse 0xC0)  
			lcd_text("Referenzfahrt!  ");											// Schreibt "Referenzfahrt!  " auf das LCD, mit zusätzlichen Leerzeichen zum Überschreiben  
			} else if (strcmp(command_buffer, "STOP") == 0) {						// Falls Befehl "STOP" erkannt wurde  
			USART_SendString("Notstop.\n");											// Sendet "Notstop." über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor wieder auf die zweite Zeile  
			lcd_text("Notstop!        \n");											// Schreibt "Notstop!" mit Leerzeichen, um die gesamte Zeile zu überschreiben  
			} else if (strcmp(command_buffer, "START") == 0) {						// Falls Befehl "START" erkannt wurde  
			request_Labeling_start(1);
			USART_SendString("Wird gestartet.\n");									// Sendet die Nachricht "Wird gestartet." über USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile  
			lcd_text("Wird gestartet! \n");											// Schreibt "Wird gestartet!" auf das LCD, mit Leerzeichen zum Auffüllen  
			} else if (strcmp(command_buffer, "POS") == 0){							// Wenn der Befehl "POS" empfangen wurde
			pos_aktiv = 1;															// Setze POS aktiv
			USART_SendString("POS\n");
			} else if (strcmp(command_buffer, "NPOS") == 0) {						// Wenn der Befehl "NPOS" empfangen wurde
			pos_aktiv = 0;
			USART_SendString("NPOS\n");
			} else if (strstr(command_buffer, ":Texto")!= 0) {						// Falls der Befehl "Text:" im empfangenen String enthalten ist  
			USART_SendString("Zeile 1 wird geschrieben:\n");
			command_buffer[strcspn(command_buffer, "\r\n")] = '\0';
			command_buffer[strcspn(command_buffer, ":")] = '\0';					// Entfernt alles ab ":"
			strcpy(texto_buffer, command_buffer);									// Kopiert damit durch START nachfolgend nicht überschieben wird
			texto_start = texto_buffer;												// Ermittelt die Position, wo der eigentliche Text beginnt
			//USART_SendString(texto_start);										// Sendet den ausgelesenen Textteil
			//USART_SendString("\n");												// Sendet einen Zeilenumbruch
			lcd_cmd(0x80);															// Setzt den LCD-Cursor auf die zweite Zeile
			char send_Texto_buffer[17];												// Puffer für die formatierte Ausgabe auf dem LCD (16 Zeichen + Nullterminator)
			snprintf(send_Texto_buffer, sizeof(send_Texto_buffer), "%-16s",texto_start);// Formatiert den Text linksbündig, füllt ggf. mit Leerzeichen auf
			lcd_text(send_Texto_buffer);											// Zeigt den formatierten Text auf dem LCD an
			USART_SendString(send_Texto_buffer);
			} else if (strstr(command_buffer, ":Textu")!= 0) {						// Falls der Befehl "Text:" im empfangenen String enthalten ist
			USART_SendString("Zeile 2 wird geschrieben:\n");
			command_buffer[strcspn(command_buffer, "\r\n")] = '\0';
			command_buffer[strcspn(command_buffer, ":")] = '\0';					// Entfernt alles ab ":"
			strcpy(textu_buffer, command_buffer);									// Kopiert damit durch START nachfolgend nicht überschieben wird
			textu_start = textu_buffer;												// Ermittelt die Position, wo der eigentliche Text beginnt
			//USART_SendString(textu_start);										// Sendet den ausgelesenen Textteil
			//USART_SendString("\n");												// Sendet einen Zeilenumbruch
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
		lcd_num (distance, LCDstr);													// Konvertiert den Messwert in einen String und speichert diesen in LCDstr  
		char send_Messung_buffer[17];												// Puffer für die LCD-Ausgabe, auf 16 Zeichen formatiert (plus Nullterminator)  
		snprintf(send_Messung_buffer, sizeof(send_Messung_buffer), "%-16s",LCDstr); // Formatiert den Inhalt von LCDstr linksbündig, füllt mit Leerzeichen auf  
		lcd_text(send_Messung_buffer);												// Zeigt die formatierte Messung auf dem LCD an 
	}  
}

// Positionsprozess: Führt den Positionsprozess durch, wenn die Positionierung aktiv ist

void USART_POSITIONIERUNG(uint8_t state) {  // pos_aktiv steuert, ob die Positionsausgabe aktiv ist
	uint16_t xPos = act_Pos_x();
	uint16_t yPos = act_Pos_y();
	uint16_t zPos = act_Pos_z();
		// Erstelle einen String mit den drei Positionswerten und sende ihn via USART
	char send_buffer_pos[20];  // Puffer für die serielle Ausgabe
	snprintf(send_buffer_pos, sizeof(send_buffer_pos), "X:%u Y:%u Z:%u\r\n",xPos , yPos, zPos);
	USART_SendString(send_buffer_pos);           // Sende den formatierten String über die serielle Schnittstelle
		//USART_POSITIONIERUNG(0);
		//request_position_send(0);
	//}
}

//void USART_Text(uint8_t text_aktiv){
	//if (text_aktiv){
		//char buffer[30];
		//USART_ReadLine(buffer, sizeof(buffer));
		//char* text_start = buffer + strlen("Text:");								// Ermittelt die Position, wo der eigentliche Text beginnt
		//USART_SendString(text_start);												// Sendet den ausgelesenen Textteil
		//USART_SendString("\n");														// Sendet einen Zeilenumbruch
		//lcd_cmd(0xC0);																// Setzt den LCD-Cursor auf die zweite Zeile
		//char send_Text_buffer[17];													// Puffer für die formatierte Ausgabe auf dem LCD (16 Zeichen + Nullterminator)
		//snprintf(send_Text_buffer, sizeof(send_Text_buffer), "%-16s",text_start);	// Formatiert den Text linksbündig, füllt ggf. mit Leerzeichen auf
		//lcd_text(send_Text_buffer);													// Zeigt den formatierten Text auf dem LCD an
		//*text_aktiv = 0;															// Stellt Text aktiv wieder auf 0
	//}
//}


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
ISR(USART0_RX_vect) {																// ISR aufgerufen, wenn ein Byte empfangen wurde
	uint8_t status = UCSR0A;														// Status-Register auslesen (FE0, DOR0, UPE0)
	uint8_t data   = UDR0;															// Datenregister lesen und RXC0-Flag löschen

	if (status & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))) {						// Fehler-Flags prüfen
		USART_SendString("UART Error: ");											// Fehlermeldung starten
		if (status & (1 << UPE0)) USART_SendString("Parity ");						// Parity-Fehler melden
		if (status & (1 << FE0))  USART_SendString("Frame ");						// Frame-Fehler melden
		if (status & (1 << DOR0)) USART_SendString("Overrun ");						// Overrun-Fehler melden
		USART_SendString("error\n");												// Fehlermeldung abschließen
		return;																		// Byte verwerfen, nicht in Puffer
	}

	uint8_t next = (usart_rx_head + 1) % USART_BUFFER_SIZE;							// nächsten Puffer-Index berechnen
	if (next != usart_rx_tail) {													// nur speichern, wenn Puffer nicht voll
		usart_rx_buffer[usart_rx_head] = data;										// empfangenes Byte im Ringpuffer ablegen
		usart_rx_head = next;														// Head-Zeiger vorwärts bewegen
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
//void USART_ReadString(char *buffer, uint8_t max_length) {							// Liest einen String aus dem Empfangspuffer bis zum Zeilenumbruch oder zur maximalen Länge  
	//uint8_t i = 0;																	// Initialisiert den Index für den Zielpuffer  
																					//// Daten aus dem Ringbuffer holen, bis \n gefunden wird oder max. Länge erreicht ist  
	//while (USART_DataAvailable() && i < max_length - 1) {							// Liest weiter, solange Daten vorhanden sind und Platz im Puffer ist  
		//buffer[i] = USART_ReadData();												// Liest ein Zeichen und speichert es im Puffer  
		//if (buffer[i] == '\n') {													// Falls das Ende des Befehls (Zeilenumbruch) erreicht wird  
			//break;																	// Beendet das Einlesen  
		//}  
		//i++;																		// Erhöht den Index für das nächste Zeichen  
	//}  
	//buffer[i] = '\0';																// Fügt den Nullterminator an, um den String abzuschließen  
//}

void USART_ReadString(char *buffer, uint8_t max_length) { // liest eine LF-terminierte Zeichenkette in buffer ein
	uint8_t i = 0;                                      // Schreibindex für buffer initialisieren
	char c;                                             // Variable zum Zwischenspeichern des gelesenen Zeichens
	do {                                                // Schleife, bis ein Zeilenende (‘\n’) empfangen wurde
		while (!USART_DataAvailable()) {}              // Warte, bis mindestens ein Byte im SW-Ringpuffer liegt
		c = USART_ReadData();                          // Lese das nächste Byte aus dem Ringpuffer
		if (c == '\r') continue;                       // Wenn Carriage Return, überspringe und lese weiter
		if (i < max_length-1) buffer[i++] = c;         // Wenn noch Platz im buffer, speichere das Zeichen und inkrementiere i
	} while (c != '\n');                               // Wiederhole, solange das gelesene Zeichen nicht Line Feed ist
	buffer[i-1] = '\0';                                // Ersetze das abschließende '\n' durch Terminator '\0'
}


//void USART_ReadLine(char *buffer, uint8_t max_length) {							// Liest einen String aus dem Empfangspuffer bis zum Zeilenumbruch oder zur maximalen Länge
	//uint8_t i = 0;																	// Initialisiert den Index für den Zielpuffer
	//char c;
	//// Daten aus dem Ringbuffer holen, bis \n gefunden wird oder max. Länge erreicht ist
	//do {																			// 1) Warte blockierend aufs nächste Byte
		//while (!USART_DataAvailable()) {;}											// busy?wait, bis ein Zeichen da ist
		//c = USART_ReadData();														// 2) Hol das Byte aus dem Puffer
		//if (c == '\r') {continue;}													// 3) Optional: CR ignorieren
		//if (i < max_length - 1) {buffer[i++] = c;}									// 4) Abspeichern, wenn noch Platz ist
	//} while (c != '\n');															// 5) Wenn '\n' gelesen, Abbruch (Zeilenende)
	//buffer[i] = '\0';																// 6) Null-Terminator setzen
//}