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
#include "USART.h"								// extern char *text_start
#include <stdint.h>
#include <stdio.h>

#define MAX_CHAR 128 

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
		move_to_position_steps_xy(2300,4300,1000);	// move to startposition 
		
		set_inStartPos(1);
	}
	
	if ((is_referenced()) && (is_inStartPos()) && !interrupts_initialized)
	{
		limit_switch_interrupt_init(); //init interrupts for limit switches
		interrupts_initialized = 1;
	}

} 

void start_Laser_Positioning (void) {
	if ((labeling_start_requested()) && (is_referenced()) && (is_inStartPos()) && interrupts_initialized)
	{
		if (!laser_pos_reached)
		{
			
			//move_Y_left_until_laser(20,500);
			move_to_position_steps_xy(2300,5500,400);
			laser_pos_reached = 1;
		}
		
		if ((laser_pos_reached) && (!Labeling_finfished))
		{
			
			//draw_A();
			Labeling_finfished = 1;
		}
	}
		
}



    // wir nutzen nur ’0’–’9’ und ’A’–’Z’, Rest bleibt NULL

//-----------------------------------------------------------------------------
// Signatur für alle Handlers: bekommen die 0-basierte Position mit
//-----------------------------------------------------------------------------
typedef void (*charHandler_t)(uint8_t pos);

//-----------------------------------------------------------------------------
// Forward?Deklarationen Deiner Draw?Routinen (jeweils in einem anderen Modul)
//-----------------------------------------------------------------------------
//void draw_0(uint8_t pos);
//void draw_1(uint8_t pos);
//void draw_2(uint8_t pos);
//void draw_3(uint8_t pos);
//void draw_4(uint8_t pos);
//void draw_5(uint8_t pos);
//void draw_6(uint8_t pos);
//void draw_7(uint8_t pos);
//void draw_8(uint8_t pos);
//void draw_9(uint8_t pos);

void draw_A(void);
//void draw_B(uint8_t pos);
//void draw_C(uint8_t pos);
//void draw_D(uint8_t pos);
//void draw_E(uint8_t pos);
//void draw_F(uint8_t pos);
//void draw_G(uint8_t pos);
//void draw_H(uint8_t pos);
//void draw_I(uint8_t pos);
//void draw_J(uint8_t pos);
//void draw_K(uint8_t pos);
//void draw_L(uint8_t pos);
//void draw_M(uint8_t pos);
//void draw_N(uint8_t pos);
//void draw_O(uint8_t pos);
//void draw_P(uint8_t pos);
//void draw_Q(uint8_t pos);
//void draw_R(uint8_t pos);
//void draw_S(uint8_t pos);
//void draw_T(uint8_t pos);
//void draw_U(uint8_t pos);
//void draw_V(uint8_t pos);
//void draw_W(uint8_t pos);
//void draw_X(uint8_t pos);
//void draw_Y(uint8_t pos);
//void draw_Z(uint8_t pos);
//void draw_space(uint8_t pos);


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
	//handlerTable['I'] = draw_I;
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
	//handlerTable['U'] = draw_U;
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
