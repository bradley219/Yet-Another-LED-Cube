#ifndef _LEDCUBE_SOURCE_
#define _LEDCUBE_SOURCE_
#include "ledcube.h"
#include <avr/eeprom.h>
#include <util/delay.h>

int main(void)
{

	DDRD |= _BV(PORTD5);
	OCR0A = 10;
	OCR0B = OCR0A / 2;
	TCCR0A = _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(WGM02) | TIMER0_PS_BITS;

	eeprom_srand();

	led_driver_init();
	tlc_set_all_gs(4095/2);
	tlc_gs_data_latch();

	snake_init();

	uint16_t g = 0;
	while(1) 
	{
		snake_task();
		continue;
		tlc_set_all_gs(g);
		tlc_gs_data_latch();
		_delay_us(500);
		g++;
		if( g > 4095 )
			g = 0;
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
