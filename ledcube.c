#ifndef _LEDCUBE_SOURCE_
#define _LEDCUBE_SOURCE_
#include "ledcube.h"
#include <avr/eeprom.h>
#include <util/delay.h>

void fader_task2(void)
{
	static hsb_t color = { .h = 0.0 , .s = 1, .b = 0.5 };
	rgb_t rgb;

	long delay = 10;

	hsb_to_rgb( &color, &rgb );
	for( uint8_t x = 0; x < LED_WIDTH; x++ )
	{
		for( uint8_t y = 0; y < LED_HEIGHT; y++ )
		{
			for( uint8_t z = 0; z < LED_DEPTH; z++ )
			{
				set_led( x, y, z, &rgb );
			}
		}
	}
	tlc_gs_data_latch();

	_delay_ms(delay);

	color.h += 0.0001;
	if( color.h >= 1 )
		color.h -= 1;
		

	return;
}

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

	//cubes_init();
	snake_init();
	//fader_init();

	uint16_t g = 0;
	char inc = 1;
	while(1) 
	{
		//snake_task();
		//continue;

		tlc_set_all_gs(g);
		tlc_gs_data_latch();
		g++;
		_delay_us(1000);
		if( g > PWM_MAX_VAL ) 
			g = 0;
		continue;

		//fader_task2();
		//fader_task();
		//cubes_task();
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
