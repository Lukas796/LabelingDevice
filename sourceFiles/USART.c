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

#define USART_BUFFER_SIZE 64														// Definiert die Gr��e des Ringpuffers f�r empfangene Daten  

volatile uint8_t usart_rx_buffer[USART_BUFFER_SIZE];								// Ringpuffer zur Speicherung empfangener Daten  
volatile uint8_t usart_rx_head = 0;													// Index f�r den n�chsten Schreibzugriff im Puffer  
volatile uint8_t usart_rx_tail = 0;													// Index f�r den n�chsten Lesezugriff im Puffer  
char LCDstr[17];																	// Puffer f�r LCD-Ausgaben (16 Zeichen + Nullterminator)  
char text_buffer[50];
char *text_start = NULL;

const char* USART_GetText(void) {
	return text_start;
}

// USART initialisieren  
void USART_Init(uint16_t baud)														// Initialisiert die USART-Schnittstelle  
{  
	UBRR0L = 103;																	// Setzt den niedrigen Teil der Baudrateneinstellung (z.B. 103 f�r ca. 9600 Baud bei 16MHz)  
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);							// Aktiviert Empf�nger, Sender und den RX-Interrupt  
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00)    // 8-Bit Data
	| (1 << UPM01)                     // Even Parity enabled
	/* UPM00 = 0 */                     // (00 = No Parity, 10 = Even, 11 = Odd)
	/* USBS0 = 0 */;                    // 0 = 1 Stopbit, 1 = 2 Stopbits
	sei();																			// Aktiviert globale Interrupts  
}  

// Empf�ngt und verarbeitet UART-Befehle  
void USART_ProcessCommands(uint8_t* messung_aktiv, uint8_t* pos_aktiv)				// Verarbeitet empfangene Befehle; "messung_aktiv" zeigt an, ob Messung l�uft  
{  
	if (USART_DataAvailable()) {
		char command_buffer[30];												// Pr�ft, ob Daten im Empfangspuffer vorhanden sind  
		USART_ReadString(command_buffer, sizeof(command_buffer));					// Liest einen String aus dem Ringpuffer und speichert ihn in "buffer"   
		if (strcmp(command_buffer, "M") == 0) {										// Wenn der Befehl "M" empfangen wurde  
			*messung_aktiv = 1;														// Setze Messung aktiv  
			} else if (strcmp(command_buffer, "MN") == 0) {							// Wenn der Befehl "MN" empfangen wurde  
			*messung_aktiv = 0;														// Setze Messung inaktiv  
			} else if (strcmp(command_buffer, "R") == 0) {							// Falls Befehl "R" (Referenzfahrt) erkannt wurde  
			request_reference_start(1);												// F�hrt Referenzfahrt durch  
			USART_SendString("Referenzfahrt gestartet.\n");							// Sendet entsprechende Best�tigung �ber USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile (Adresse 0xC0)  
			lcd_text("Referenzfahrt!  ");											// Schreibt "Referenzfahrt!  " auf das LCD, mit zus�tzlichen Leerzeichen zum �berschreiben  
			} else if (strcmp(command_buffer, "STOP") == 0) {						// Falls Befehl "STOP" erkannt wurde  
			USART_SendString("Notstop.\n");											// Sendet "Notstop." �ber USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor wieder auf die zweite Zeile  
			lcd_text("Notstop!        ");											// Schreibt "Notstop!" mit Leerzeichen, um die gesamte Zeile zu �berschreiben  
			} else if (strcmp(command_buffer, "START") == 0) {						// Falls Befehl "START" erkannt wurde  
			request_Labeling_start(1);
			processTextState();
			USART_SendString("Wird gestartet.\n");									// Sendet die Nachricht "Wird gestartet." �ber USART  
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile  
			lcd_text("Wird gestartet! ");											// Schreibt "Wird gestartet!" auf das LCD, mit Leerzeichen zum Auff�llen  
			} else if (strcmp(command_buffer, "POS") == 0){							// Wenn der Befehl "POS" empfangen wurde
			*pos_aktiv = 1;															// Setze POS aktiv
			USART_SendString("POS\n");
			} else if (strcmp(command_buffer, "NPOS") == 0) {						// Wenn der Befehl "NPOS" empfangen wurde
			*pos_aktiv = 0;
			USART_SendString("NPOS\n");
			} else if (strstr(command_buffer, ":Text")!= 0) {						// Falls der Befehl "Text:" im empfangenen String enthalten ist  
			USART_SendString("Text wird geschrieben: ");
			command_buffer[strcspn(command_buffer, "\r\n")] = '\0';
			command_buffer[strcspn(command_buffer, ":")] = '\0';					// Entfernt alles ab ":"
			strcpy(text_buffer, command_buffer);								// Kopiert damit durch START nachfolgend nicht �berschieben wird
			text_start = text_buffer;												// Ermittelt die Position, wo der eigentliche Text beginnt
			USART_SendString(text_start);											// Sendet den ausgelesenen Textteil
			USART_SendString("\n");													// Sendet einen Zeilenumbruch
			lcd_cmd(0xC0);															// Setzt den LCD-Cursor auf die zweite Zeile
			char send_Text_buffer[17];												// Puffer f�r die formatierte Ausgabe auf dem LCD (16 Zeichen + Nullterminator)
			snprintf(send_Text_buffer, sizeof(send_Text_buffer), "%-16s",text_start);// Formatiert den Text linksb�ndig, f�llt ggf. mit Leerzeichen auf
			lcd_text(send_Text_buffer);												// Zeigt den formatierten Text auf dem LCD an
			}
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
		char send_buffer_pos[20];  // Puffer f�r die serielle Ausgabe
		snprintf(send_buffer_pos, sizeof(send_buffer_pos), "X:%u Y:%u Z:%u\n", act_Pos_x(), act_Pos_y(), act_Pos_z());
		USART_SendString(send_buffer_pos);           // Sende den formatierten String �ber die serielle Schnittstelle
	}
}

//void USART_Text(uint8_t text_aktiv){
	//if (text_aktiv){
		//char buffer[30];
		//USART_ReadLine(buffer, sizeof(buffer));
		//char* text_start = buffer + strlen("Text:");								// Ermittelt die Position, wo der eigentliche Text beginnt
		//USART_SendString(text_start);												// Sendet den ausgelesenen Textteil
		//USART_SendString("\n");														// Sendet einen Zeilenumbruch
		//lcd_cmd(0xC0);																// Setzt den LCD-Cursor auf die zweite Zeile
		//char send_Text_buffer[17];													// Puffer f�r die formatierte Ausgabe auf dem LCD (16 Zeichen + Nullterminator)
		//snprintf(send_Text_buffer, sizeof(send_Text_buffer), "%-16s",text_start);	// Formatiert den Text linksb�ndig, f�llt ggf. mit Leerzeichen auf
		//lcd_text(send_Text_buffer);													// Zeigt den formatierten Text auf dem LCD an
		//*text_aktiv = 0;															// Stellt Text aktiv wieder auf 0
	//}
//}


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
ISR(USART0_RX_vect) {
	uint8_t status = UCSR0A;
	uint8_t data   = UDR0;           // Muss auf UDR0 zugegriffen werden, um das Flag zur�ckzusetzen

	if (status & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))) {
		// Frame Error FE0, Overrun Error DOR0 oder Parity Error UPE0 aufgetreten
		// hier z.B. verwerfen oder zur�ckmelden:
		USART_SendString("UART Error: ");
		if (status & (1 << UPE0)) USART_SendString("Parity ");
		if (status & (1 << FE0))  USART_SendString("Frame ");
		if (status & (1 << DOR0)) USART_SendString("Overrun ");
		USART_SendString("error\n");
		return;  // kein weiteres Puffer?Speichern
	}

	// sonst ins Ringbuffer:
	uint8_t next = (usart_rx_head + 1) % USART_BUFFER_SIZE;
	if (next != usart_rx_tail) {
		usart_rx_buffer[usart_rx_head] = data;
		usart_rx_head = next;
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
//void USART_ReadString(char *buffer, uint8_t max_length) {							// Liest einen String aus dem Empfangspuffer bis zum Zeilenumbruch oder zur maximalen L�nge  
	//uint8_t i = 0;																	// Initialisiert den Index f�r den Zielpuffer  
																					//// Daten aus dem Ringbuffer holen, bis \n gefunden wird oder max. L�nge erreicht ist  
	//while (USART_DataAvailable() && i < max_length - 1) {							// Liest weiter, solange Daten vorhanden sind und Platz im Puffer ist  
		//buffer[i] = USART_ReadData();												// Liest ein Zeichen und speichert es im Puffer  
		//if (buffer[i] == '\n') {													// Falls das Ende des Befehls (Zeilenumbruch) erreicht wird  
			//break;																	// Beendet das Einlesen  
		//}  
		//i++;																		// Erh�ht den Index f�r das n�chste Zeichen  
	//}  
	//buffer[i] = '\0';																// F�gt den Nullterminator an, um den String abzuschlie�en  
//}

void USART_ReadString(char *buffer, uint8_t max_length) {
	uint8_t i = 0;
	char c;
	do {
		// warte aufs n�chste Byte
		while (!USART_DataAvailable()) {}
		c = USART_ReadData();
		if (c == '\r') continue;              // CR �berspringen
		if (i < max_length-1) buffer[i++] = c;
	} while (c != '\n');
	// das '\n' sitzt jetzt in buffer[i-1]
	buffer[i-1] = '\0';                      // ersetze '\n' durch '\0'
}

//void USART_ReadLine(char *buffer, uint8_t max_length) {							// Liest einen String aus dem Empfangspuffer bis zum Zeilenumbruch oder zur maximalen L�nge
	//uint8_t i = 0;																	// Initialisiert den Index f�r den Zielpuffer
	//char c;
	//// Daten aus dem Ringbuffer holen, bis \n gefunden wird oder max. L�nge erreicht ist
	//do {																			// 1) Warte blockierend aufs n�chste Byte
		//while (!USART_DataAvailable()) {;}											// busy?wait, bis ein Zeichen da ist
		//c = USART_ReadData();														// 2) Hol das Byte aus dem Puffer
		//if (c == '\r') {continue;}													// 3) Optional: CR ignorieren
		//if (i < max_length - 1) {buffer[i++] = c;}									// 4) Abspeichern, wenn noch Platz ist
	//} while (c != '\n');															// 5) Wenn '\n' gelesen, Abbruch (Zeilenende)
	//buffer[i] = '\0';																// 6) Null-Terminator setzen
//}