/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 */ 

#include "config.h"
#include "motor_control.h"
#include "lcd_control.h"
#include "USART.h"


int main(void) 
{
    USART_Init(BAUDRATE);
    init_ADC();
    lcd_init();

    uint8_t messung_aktiv = 0;

    while (1) {
        USART_ProcessCommands(&messung_aktiv);  // Verarbeitet eingehende Steuerbefehle
        USART_MESSUNG(messung_aktiv);           // Führt Messungen durch, falls aktiv
        _delay_ms(2000);
    }

    return 0;
}

