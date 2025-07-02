/*
 * buttons.h
 *
 * Created: 23.06.2025 10:45:48
 *  Author: lukasstrittmatter
 */ 


#ifndef BUTTONS_CONTROL_H_
#define BUTTONS_CONTROL_H_

#include "config.h" // need to be first

//Functions from button_control.c
void button_init(void);
void buttons(void);


// --- Confirmation Button on PG1 ---
#define CONFIRM_BUTTON_PORT     PORTG     // Port Register for Confirmation
#define CONFIRM_BUTTON_DDR      DDRG      // Data Direction Register for Confirmation
#define CONFIRM_BUTTON_PINREG   PING      // Input Register for Confirmation
#define CONFIRM_BUTTON_PIN      PG1       // Pin Number for Confirmation

// --- Abort Button on PG0 ---
#define ABORT_BUTTON_PORT     PORTG     // Port Register for Abort
#define ABORT_BUTTON_DDR      DDRG      // Data Direction Register for Abort
#define ABORT_BUTTON_PINREG   PING      // Input Register for Abort
#define ABORT_BUTTON_PIN      PG0       // Pin Number for Abort

// --- Second_Button Button on PG1 ---
#define SECOND_BUTTON_PORT     PORTL     // Port Register for Second_Button
#define SECOND_BUTTON_DDR      DDRL      // Data Direction Register for Second_Button
#define SECOND_BUTTON_PINREG   PINL      // Input Register for Second_Button
#define SECOND_BUTTON_PIN      PL7       // Pin Number for Second_Button

// --- Start Button on PG1 ---
#define START_BUTTON_PORT     PORTL     // Port Register for Second_Button
#define START_BUTTON_DDR      DDRL      // Data Direction Register for Second_Button
#define START_BUTTON_PINREG   PINL      // Input Register for Second_Button
#define START_BUTTON_PIN      PL6       // Pin Number for Second_Button


#endif /* BUTTONS_CONTROL_H_ */