/*
 * buttons.c
 *
 * Created: 12.06.2025 09:20:17
 *  Author: lukasstrittmatter
 */ 

#include "config.h"			// include config.h - need to be first
#include "systemstate.h"	// include own header File
#include "motor_control.h"	// include motor_control Functions
#include "letter_control.h"	// include letter_control Functions

static uint8_t referenced = 0;		//static state variable for the refernce status of the axis. 0 = not referenced , 1 = referenced
static uint8_t start_reference = 0;	//static state variable to start the reference process	0 = no reference start is requested , 1 = reference start is requested
static uint8_t inStartPos = 0;		//static state variable if Axis is in startPos, 0 = no, 1 = yes
static uint8_t interrupts_initialized = 0;	//static state variabale if the interrupts for the limitswitches are initialized and active, 0 = no, 1 = yes
static uint8_t laser_pos_reached = 0;	//static state variable if the positioning with the laser has reached its target pos. 0 = no, 1 = yes
static uint8_t start_Labeling = 0;	//static state variable to start the Labeling, 0 = no start reqeusted, 1 = start requested
static uint8_t Labeling_finfished = 0;	// static state variable for the Finish of the labeling. 0 = Labeling hasnt finished, 1 = Labeling has finished

// is_refernced(): Funktion mit der der Referenzierungsstaus abgefragt werden kann. Ausgabe is der refernced state. 0 = nein, 1 = ja
uint8_t is_referenced(void) {
	return referenced;		// return static state variable for refernce
}

// is_inStartPos(): Funktion mit der das Erreichen der Startposition zum Beschriften abgefragt werden kann. Ausgabe ist 0 = nein, 1 = ja
uint8_t is_inStartPos(void) {
	return inStartPos;		// return static state variable for inStartPos
}

// reference_start_requested(): Funktion mit der die Anfrage ob eine refernzfahrt ausgeführt werden soll abgefragt werden kann. Ausgabe ist 0 = keine Anfrage, 1 = ja Refernzfahrt soll ausgeführt werden
uint8_t reference_start_requested(void) {
	return start_reference;	// return static state variable for istart_refernce
}

// labeling_start_requested(): Funktion mit der der Start des labelingvorgangs ab´gefragt werden kann. 0 = kein Start, 1 = ja es soll gestartet werden 
uint8_t labeling_start_requested(void) {
	return start_Labeling;	// return static state variable for start_Labeling
}

// set_referenced(1): mit dieser Funktion kann die state Variable für refenced auf 1 oder 0 gesetzt werden. 1 = ist refernziert, 0 = ist nicht refernziert
void set_referenced(uint8_t state) {
	referenced = state;	// set static state variable for referenced
}

// set_inStartPos(1): mit dieser Funktion kann die State Variable für das Erreichen der StartPosition auf 1 oder 0 gesetzt werden.
void set_inStartPos(uint8_t state) {
	inStartPos = state;	// set static state variable for inStartPos
}

// request_reference_start(1): mit dieser Funktion kann die State Variable für das Starten der Referenzfahrt gesetzt werden.
void request_reference_start(uint8_t state) {
	start_reference = state;	// set static state variable for start_reference
}

// request_Labeling_start(1): mit dieser Funktion kann die State Variable für das Starten der Becshriftung gesetzt werden.
void request_Labeling_start(uint8_t state) {
	start_Labeling = state;	// set static state variable for start_Labeling
}

// reference_StartPos_control(): mit dieser Funktion wird der Status und die Steuerung der Refernzfahrt und der Startposition gemacht. --> call in MAIN
void reference_StartPos_control(void){
	
	// Starte Refernzfahrt wenn noch nicht refernziert wurde und Refernzfahrt angefragt wird
	if (!is_referenced())	//Abfrage referenced = 0
	{
		if (reference_start_requested())	//Abfarge start_reference = 1
		{
			
			disable_limit_switch_interrupts();	// deaktiviere zuerst die Interrupts der Endlagensensoren, da mit ihnen refernziert wird
			interrupts_initialized = 0;			// setzte State Variable dass die interrupts deaktiviert sind
			start_XY_reference();				// Referenzfahrt starten --> in motor_control.c
		}
	}
	
	// Fahre zu Start-Position wenn der refernziert wurde und die Startpsition noch nicht angefahren wurde 
	if ((is_referenced()) && (!is_inStartPos()))
	{
		move_to_position_steps_xy(1800,5000,1000);		// move to startposition --> first column 
		move_to_position_steps_z(1000,200);				// move to startposition in z 
		set_inStartPos(1);		// set state variable inStartPos = 1
	}
	
	// inizialisiere und aktiviere wieder die interrupts, wenn refernziert wurde, die Startposition erreicht wurde und die Interrupts noch deaktiviert sind.
	if ((is_referenced()) && (is_inStartPos()) && !interrupts_initialized)
	{
		limit_switch_interrupt_init(); //init interrupts for limit switches
		interrupts_initialized = 1;	// set state variable interrupts_initialized = 1
	}
} 

// start_Laser_Positioning(): Mt dieser Funktion wird der Stift zum Blatt gefahren und die gewünschte Buchstaben werden geschrieben 
void start_Laser_Positioning (void) {
	
	// Starte Laserpositionierung und Beschriftung wenn: Labeling Start angefragt wurde, Refernziert ist, Startposition erreicht wurde und interrupts initialisert sind
	if ((labeling_start_requested()) && (is_referenced()) && (is_inStartPos()) && interrupts_initialized)
	{
		// Abfrage ob der Stift schon mit dem Laser ans Blatt positioniert wurde
		if (!laser_pos_reached)	// Abfarge ob laser_pos_reached = 0
		{
			move_Y_left_until_laser(20,200);	//Fahre solange in Y-Richtung, bis der Laser nur noch 20mm Abstand misst. --> Funktion in motor_control.c
			laser_pos_reached = 1;				// setzte state_variable for laser_pos_reached = 1
		}
		
		// Abfarge ob der Stift mit der Laserpositionierung ans Blatt gefahren wurde und ob die Becshriftung noch nicht fertig ist
		if ((laser_pos_reached) && (!Labeling_finfished))
		{
			processTextoState();			 // check String für Obere Textzeile 
			move_X_relative(700,500); //Zweite Zeile
			move_to_position_steps_z(1000,500);	// move to startposition for second line
			processTextuState();			// check String für Untere Textzeile 
			Labeling_finfished = 1;	// setzte state Variable für Labeling finished auf 1
		}
	}
		
}

//-----------------------------------------------------------------------------
// Signatur für alle Handlers: bekommen die 0-basierte Position mit
//-----------------------------------------------------------------------------
typedef void (*charHandler_t)(uint8_t pos);
//-----------------------------------------------------------------------------
// Lookup?Tabelle für alle 36 Zeichen
//-----------------------------------------------------------------------------
static charHandler_t handlerTable[MAX_CHAR];

//-----------------------------------------------------------------------------
// initHandlers(): füllt die handlerTable einmalig zu Programmstart
//-----------------------------------------------------------------------------
void initHandlers(void) {

	// Buchstaben A–Z
	handlerTable['A'] = draw_A;
	handlerTable['B'] = draw_B;
	handlerTable['C'] = draw_C;
	handlerTable['D'] = draw_D;
	handlerTable['E'] = draw_E;
	handlerTable['F'] = draw_F;
	handlerTable['G'] = draw_G;
	handlerTable['H'] = draw_H;
	handlerTable['I'] = draw_I;
	handlerTable['J'] = draw_J;
	handlerTable['K'] = draw_K;
	handlerTable['L'] = draw_L;
	handlerTable['M'] = draw_M;
	handlerTable['N'] = draw_N;
	handlerTable['O'] = draw_O;
	handlerTable['P'] = draw_P;
	handlerTable['Q'] = draw_Q;
	handlerTable['R'] = draw_R;
	handlerTable['S'] = draw_S;
	handlerTable['T'] = draw_T;
	handlerTable['U'] = draw_U;
	handlerTable['V'] = draw_V;
	handlerTable['W'] = draw_W;
	handlerTable['X'] = draw_X;
	handlerTable['Y'] = draw_Y;
	handlerTable['Z'] = draw_Z;
	handlerTable[' '] = draw_space;
	
}

//-----------------------------------------------------------------------------
// processTextState(): Zerlegt den String und ruft für jedes Zeichen seinen
//                   Handler mit der Position als Parameter auf
//-----------------------------------------------------------------------------
void processTextoState(void) {								// Funktion zum Verarbeiten des Textzustands
	const char *txt = USART_GetTexto();						// Text von der USART-Schnittstelle abrufen
	
	if (!txt || !*txt) {									// abbrechen, falls kein Text oder leerer String
		USART_SendString("nichts zu tun\r\n");				// Meldung „nichts zu tun“ senden
		return;												// Funktion vorzeitig beenden
	}
	USART_SendString(txt);									 // empfangenen Text über USART ausgeben
	// 2) Nur starten, wenn Labeling angefordert
	for (uint8_t pos = 0; txt[pos] != '\0'; pos++) {					// über alle Zeichen iterieren bis Nullterminator
		char c = txt[pos];												// aktuelles Zeichen in Variable c speichern
		if ((uint8_t)c < MAX_CHAR && handlerTable[(uint8_t)c]) {		// prüfen, ob Zeichen im Bereich und ein Handler registriert ist
			handlerTable[(uint8_t)c](pos);								// passenden Handler mit Zeichenposition aufrufen
		}
	}
}

void processTextuState(void) {								//dasselbe für die untere Textzeile Textu
	const char *txt = USART_GetTextu();
	// 1) Wenn kein Text, Abbruch
	if (!txt || !*txt) {
		USART_SendString("nichts zu tun\r\n");
		return;
	}
	USART_SendString(txt);
	// 2) Nur starten, wenn Labeling angefordert
	for (uint8_t pos = 0; txt[pos] != '\0'; pos++) {
		char c = txt[pos];
		if ((uint8_t)c < MAX_CHAR && handlerTable[(uint8_t)c]) {
			handlerTable[(uint8_t)c](pos);
		}
		
	}
}