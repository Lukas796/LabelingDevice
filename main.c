/*
 * LabelingDevice.c
 *
 * Created: 15.05.2025 12:49:39
 * Authors : lukasstrittmatter, robinschad
 */ 

#include <avr/io.h>
#include "Laser.h"
#include "config.h"


int main(void)
{
	DDRK &= ~((1 << OP1) | (1 << OP2)); // Eingänge setzen
	laser_init();  // Sensor initialisieren

	while (1)
	{
		laser_read();  // Abstandsmessung
		_delay_ms(500);   // Alle 500ms aktualisieren
	}
}

