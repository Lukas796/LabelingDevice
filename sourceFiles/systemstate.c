/*
 * buttons.c
 *
 * Created: 12.06.2025 09:20:17
 *  Author: lukasstrittmatter
 */ 
#include "config.h" // need to be first
#include "systemstate.h"
#include "motor_control.h"
#include "letter_control.h"

static uint8_t referenced = 0;	//static state variable if Axis are referenced	0 = not referenced , 1 = referenced
static uint8_t start_reference = 0;	//static state variable if Axis are referenced	0 = dont start reference , 1 = start reference
static uint8_t inStartPos = 0;	//static state variable if Axis is in startPos
static uint8_t interrupts_initialized = 0;
static uint8_t laser_pos_reached = 0;
static uint8_t start_Labeling = 0;
static uint8_t Labeling_finfished = 0;


uint8_t is_referenced(void) {
	return referenced;
}

uint8_t is_inStartPos(void) {
	return inStartPos;
}

uint8_t reference_start_requested(void) {
	return start_reference;
}

uint8_t labeling_start_requested(void) {
	return start_Labeling;
}

void set_referenced(uint8_t state) {
	referenced = state;
}

void set_inStartPos(uint8_t state) {
	inStartPos = state;
}

void request_reference_start(uint8_t state) {
	start_reference = state;
}

void request_Labeling_start(uint8_t state) {
	start_Labeling = state;
}

void reference_StartPos_control(void){
	
	// Wenn Refernce Start Requested UND noch nicht referenziert wurde
	if (!is_referenced())
	{
		if (reference_start_requested())
		{
			
			disable_limit_switch_interrupts();
			interrupts_initialized = 0;
			start_XY_reference();			 // Referenzfahrt starten
		}
	}
	
	if ((is_referenced()) && (!is_inStartPos()))
	{
		move_to_position_steps_xy(2600,5000,1000);	// move to startposition 
		move_to_position_steps_z(1000,200);	// move to startposition 
		set_inStartPos(1);
	}
	
	if ((is_referenced()) && (is_inStartPos()) && !interrupts_initialized)
	{
		//limit_switch_interrupt_init(); //init interrupts for limit switches
		interrupts_initialized = 1;
	}

} 

void start_Laser_Positioning (void) {
	if ((labeling_start_requested()) && (is_referenced()) && (is_inStartPos()) && interrupts_initialized)
	{
		if (!laser_pos_reached)
		{
			
			move_Y_left_until_laser(20,200);

			laser_pos_reached = 1;
		}
		
		if ((laser_pos_reached) && (!Labeling_finfished))
		{
			//draw_A();
			draw_B();
			//draw_C();
			//draw_D();
			//draw_E();
			//draw_F();
			//draw_G();
			//draw_H();
			//draw_I();
			//draw_J();
			draw_K();
			//draw_L();
			//draw_M();
			draw_N();
			//draw_O();
			//draw_P();
			draw_Q();
			draw_R();
			draw_S();
			draw_T();
			//draw_U();
			//draw_V();
			//draw_W();
			draw_X();
			draw_Y();
			draw_Z();
			move_X_relative(700,500); //Zweite Zeile
			move_to_position_steps_z(1000,500);	// move to startposition for second line
			draw_A();
			
			//draw_U();
			//draw_TEST();
			//processTextState(); // check String
			//
			//move_X_relative(400,500); //Zweite Zeile
			//move_to_position_steps_z(200,500);	// move to startposition for second line	
			//processTextStateLine2(); // check String
			Labeling_finfished = 1;
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
	// Ziffern 0–9
	//handlerTable['0'] = draw_0;
	//handlerTable['1'] = draw_1;
	//handlerTable['2'] = draw_2;
	//handlerTable['3'] = draw_3;
	//handlerTable['4'] = draw_4;
	//handlerTable['5'] = draw_5;
	//handlerTable['6'] = draw_6;
	//handlerTable['7'] = draw_7;
	//handlerTable['8'] = draw_8;
	//handlerTable['9'] = draw_9;

	// Buchstaben A–Z
	handlerTable['A'] = draw_A;
	//handlerTable['B'] = draw_B;
	//handlerTable['C'] = draw_C;
	//handlerTable['D'] = draw_D;
	//handlerTable['E'] = draw_E;
	//handlerTable['F'] = draw_F;
	//handlerTable['G'] = draw_G;
	//handlerTable['H'] = draw_H;
	handlerTable['I'] = draw_I;
	//handlerTable['J'] = draw_J;
	//handlerTable['K'] = draw_K;
	//handlerTable['L'] = draw_L;
	//handlerTable['M'] = draw_M;
	//handlerTable['N'] = draw_N;
	//handlerTable['O'] = draw_O;
	//handlerTable['P'] = draw_P;
	//handlerTable['Q'] = draw_Q;
	//handlerTable['R'] = draw_R;
	//handlerTable['S'] = draw_S;
	//handlerTable['T'] = draw_T;
	handlerTable['U'] = draw_U;
	//handlerTable['V'] = draw_V;
	//handlerTable['W'] = draw_W;
	//handlerTable['X'] = draw_X;
	//handlerTable['Y'] = draw_Y;
	//handlerTable['Z'] = draw_Z;
	//handlerTable[' '] = draw_space;
	
}

//-----------------------------------------------------------------------------
// processTextState(): Zerlegt den String und ruft für jedes Zeichen seinen
//                   Handler mit der Position als Parameter auf
//-----------------------------------------------------------------------------
void processTextState(void) {
	const char *txt = USART_GetText();
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
		// Debug-Ausgabe
		char dbg[16];
		int n = snprintf(dbg, sizeof(dbg), "%02u:%c\r\n", pos, c);
		if (n > 0) {
			USART_SendString(dbg);
		}
	}
}