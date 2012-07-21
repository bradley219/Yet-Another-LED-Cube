#ifndef _LEDCUBE_SOURCE_
#define _LEDCUBE_SOURCE_
#include "ledcube.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

hsb_t main_color = { .h = 0, .s = 1, .b = 0.5 };

//void fader_task2(void)
//{
//	static hsb_t color = { .h = 0.0 , .s = 1, .b = 0.5 };
//	rgb_t rgb;
//
//	long delay = 10;
//
//	hsb_to_rgb( &color, &rgb );
//	for( uint8_t x = 0; x < LED_WIDTH; x++ )
//	{
//		for( uint8_t y = 0; y < LED_HEIGHT; y++ )
//		{
//			for( uint8_t z = 0; z < LED_DEPTH; z++ )
//			{
//				set_led( x, y, z, &rgb );
//			}
//		}
//	}
//	tlc_gs_data_latch();
//
//	_delay_ms(delay);
//
//	color.h += 0.0002;
//	if( color.h >= 1 )
//		color.h -= 1;
//
//	return;
//}

volatile unsigned long loopcount = 0;
int main(void)
{
	audio_init();

	eeprom_srand();

	tlc_set_all_gs(0);
	tlc_gs_data_latch();
	
	led_driver_init();

	//cubes_init();
	snake_init();
	//fader_init();
	
	uint16_t g = 0;//PWM_MAX_VAL * 7 / 8;
	char inc = 1;
	while(1) 
	{
		loopcount++;

		audio_task();
		snake_task();
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
#endif
