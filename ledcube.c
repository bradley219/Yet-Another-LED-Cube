#ifndef _LEDCUBE_SOURCE_
#define _LEDCUBE_SOURCE_
#include "ledcube.h"
#include "avr/eeprom.h"


int main(void)
{
	eeprom_srand();

	led_driver_init();
	tlc_set_all_gs(0);

	cubes_init();

	volatile unsigned long counter = 0;
	while(1) 
	{
		//purple_test();

		//ledtest1();
		cubes_task();
		counter++;
	}
	return 0;
}

static unsigned long EEMEM analog_seed = 0;
void eeprom_srand(void)
{
	volatile unsigned long seed;
	eeprom_read_block( &seed, &analog_seed, sizeof(analog_seed) );
	srand(seed);
	seed++;
	eeprom_write_block( &seed, &analog_seed, sizeof(analog_seed) );
	return;
}
void analog_init(void)
{
	// digital input disable
	DIDR0 = _BV(ADC5D) | _BV(ADC4D) | _BV(ADC3D) | _BV(ADC2D) | _BV(ADC0D);
	return;
}
#endif
