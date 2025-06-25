/*
 * USART.h
 *
 * Created: 28.05.2025 10:30:28
 *  Author: robin
 */ 


#ifndef USART_H_
#define USART_H_

void USART_Init(uint16_t baud);
void USART_SendData(uint8_t data);
void USART_SendString(const char *str);
uint8_t USART_ReadData(void);
uint8_t USART_DataAvailable();
ISR(USART0_RX_vect);
void USART_MESSUNG(uint8_t messung_aktiv);
void USART_POSITIONIERUNG(uint8_t pos_aktiv);

#endif /* USART_H_ */