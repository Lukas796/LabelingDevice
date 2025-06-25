/*
 * USART.h
 *
 * Created: 28.05.2025 10:30:28
 *  Author: robin
 */ 


#ifndef USART_H_
#define USART_H_

#include "config.h" // need to be first

void USART_Init(uint16_t baud);
void USART_ProcessCommands(uint8_t* messung_aktiv);
void USART_MESSUNG(uint8_t messung_aktiv);
void USART_SendData(uint8_t data);
void USART_SendString(const char *str);
uint8_t USART_ReadData(void);
uint8_t USART_DataAvailable(void);
ISR(USART0_RX_vect);




#endif /* USART_H_ */