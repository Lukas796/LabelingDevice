/*
 * buttons.h
 *
 * Created: 23.06.2025 10:45:48
 *  Author: lukasstrittmatter
 */ 


#ifndef BUTTONS_CONTROL_H_
#define BUTTONS_CONTROL_H_

#include "config.h" // need to be first

//Functions from .c
void button_init(void);
void buttons(void);


// --- Confirmation Button on PG1 ---
#define CONFIRM_BUTTON_PORT     PORTG     // Port Register
#define CONFIRM_BUTTON_DDR      DDRG      // Data Direction Register
#define CONFIRM_BUTTON_PINREG   PING      // Input Register
#define CONFIRM_BUTTON_PIN      PG1       // Pin Number

// --- Confirmation Button on PG0 ---
#define ABORT_BUTTON_PORT     PORTG     // Port Register
#define ABORT_BUTTON_DDR      DDRG      // Data Direction Register
#define ABORT_BUTTON_PINREG   PING      // Input Register
#define ABORT_BUTTON_PIN      PG0       // Pin Number

// --- Confirmation Button on PG1 ---
#define SECOND_BUTTON_PORT     PORTL     // Port Register
#define SECOND_BUTTON_DDR      DDRL      // Data Direction Register
#define SECOND_BUTTON_PINREG   PINL      // Input Register
#define SECOND_BUTTON_PIN      PL7       // Pin Number

// --- Confirmation Button on PG1 ---
#define START_BUTTON_PORT     PORTL     // Port Register
#define START_BUTTON_DDR      DDRL      // Data Direction Register
#define START_BUTTON_PINREG   PINL      // Input Register
#define START_BUTTON_PIN      PL6       // Pin Number


#endif /* BUTTONS_CONTROL_H_ */