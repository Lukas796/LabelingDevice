
/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 * Last Change 12-06-2025
 */ 

#include "config.h" // need to be first

#include "motor_control.h"
#include "lcd_control.h"
#include "laser.h"
#include "systemstate.h"
#include "USART.h"
#include "buttons_control.h"

int main(void)
{
	sei();  // activate global interrupts
	motor_init();
	limit_switch_init();
	motor_init_timer();
	button_init();  
	lcd_init();
	init_ADC();  
	//USART_Init(BAUDRATE);
	
	uint8_t messung_aktiv = 0;
	uint8_t pos_aktiv = 0;
	
	lcd_text("Labeling Device");
	
	//start_XY_reference();			 // Referenzfahrt starten
	//move_to_position_steps_xy(2300,4300,1000);	// move to startposition 
	//move_to_position_steps_xz(2500 , 50, 500);
	//limit_switch_interrupt_init(); //init interrupts for limit switches
	//move_Y_left_until_laser(20,500);
	//_delay_ms(20);
	//move_to_position_steps_xy(100,100,400);	// move to startposition
	//move_to_position_steps_z(100,400);
	//move_to_position_steps_xy(2300,4300,1000);	// move to startposition 
	//move_to_position_steps_xz(2500 , 150, 500);
	//move_Y_left_until_laser(20,500);
	//_delay_ms(20);
	//move_to_position_steps_xy(2300,4300,1000);	// move to startposition
	//move_to_position_steps_xz(2100 , 100, 500);
	//move_to_position_steps_z(200,400);
	//move_Y_left_until_laser(20,500);
	//_delay_ms(20);
	//move_to_position_steps_xy(100,100,500);	// move to startposition
	//move_to_position_steps_z(300,500);
	//_delay_ms(20);
	//move_to_position_steps_xy(2300,4300,1000);	// move to startposition
	//move_to_position_steps_xz(2100 , 350, 500);
	//move_to_position_steps_z(100,500);
	//move_Y_left_until_laser(20,500);
	//_delay_ms(20);
	//move_to_position_steps_xy(200,200,300);	// move to startposition
	//move_to_position_steps_z(600,500);
	//_delay_ms(20);
	//move_to_position_steps_xy(2300,4300,400);	// move to startposition
	//move_Y_left_until_laser(20,500);
	//_delay_ms(20);
	//move_to_position_steps_xy(100,100,1000);	// move to startposition
	//_delay_ms(20);
	//move_to_position_steps_xy(2300,4300,1000);	// move to startposition
	//move_Y_left_until_laser(20,500);
	//_delay_ms(20);
	//move_to_position_steps_xy(100,100,1000);	// move to startposition
	//
	
	 //uint16_t speed_main = 600;
	 //int32_t diag_steps_x_main = 600;
	 //int32_t diag_steps_z_main = 150;
	 //int32_t crossbar_z_steps_main = 150;
	 //int32_t height_x_steps_main = 600;
//
	//move_XZ_diagonal_relative(-diag_steps_x_main, diag_steps_z_main, speed_main);
	//_delay_ms(20);
	//move_XZ_diagonal_relative(diag_steps_x_main,diag_steps_z_main,speed_main);
	//_delay_ms(20);
	//move_pen_backward();
	//_delay_ms(20);
	//move_XZ_diagonal_relative(-diag_steps_x_main/2, -diag_steps_z_main/2, speed_main);
	//_delay_ms(20);
	//move_pen_forward();
	//_delay_ms(20);
	//move_Z_relative(-crossbar_z_steps_main,speed_main);
	//_delay_ms(20);
	//move_pen_backward();
	//_delay_ms(20);
	//move_XZ_diagonal_relative(diag_steps_x_main/2, diag_steps_z_main/2, speed_main);
	//_delay_ms(20);
	//move_Z_relative(crossbar_z_steps_main+20,speed_main);
	//_delay_ms(20);
	//move_pen_backward();
	//_delay_ms(20);
	//move_pen_forward();
	//_delay_ms(20);
	//move_XZ_diagonal_relative(-diag_steps_x_main, diag_steps_z_main, speed_main);
	//_delay_ms(20);
	//move_XZ_diagonal_relative(diag_steps_x_main,diag_steps_z_main,speed_main);
	//_delay_ms(20);
	//move_pen_backward();
	//_delay_ms(20);
	//move_XZ_diagonal_relative(-diag_steps_x_main/2, -diag_steps_z_main/2, speed_main);
	//_delay_ms(20);
	//move_pen_forward();
	//_delay_ms(20);
	//move_Z_relative(-crossbar_z_steps_main,speed_main);
	//_delay_ms(20);
	//move_pen_backward();
	//_delay_ms(20);
	//move_XZ_diagonal_relative(diag_steps_x_main/2, diag_steps_z_main/2, speed_main);
	//_delay_ms(20);
	//move_Z_relative(crossbar_z_steps_main+20,speed_main);
	//_delay_ms(20);
	//move_pen_backward();
	//
	while (1)
	{
		buttons();
		reference_StartPos_control();
		start_Laser_Positioning();
		
		  //USART_ProcessCommands(&messung_aktiv, &pos_aktiv);  // Verarbeitet eingehende Steuerbefehle
		  //USART_MESSUNG(messung_aktiv);           // Führt Messungen durch, falls aktiv
		  //USART_POSITIONIERUNG(pos_aktiv);
		  _delay_ms(10);
	}
}


	
