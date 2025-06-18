/*
 * systemstate.h
 *
 * Created: 12.06.2025 09:19:45
 *  Author: lukasstrittmatter
 */ 


#ifndef SYSTEMSTATE_H_
#define SYSTEMSTATE_H_

#include <avr/io.h>
#include <stdint.h>

void reference_control(void);
void set_referenced(uint8_t state);
uint8_t is_referenced(void);

void set_inStartPos(uint8_t state);
uint8_t is_inStartPos(void);

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



#endif /* SYSTEMSTATE_H_ */