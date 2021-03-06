#ifndef _TESTS_SOURCE_
#define _TESTS_SOURCE_

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "types.h"
#include "config.h"
#include "tests.h"
#include "driver.h"

/*
void ledtest1(void)
{
	uint16_t d = 100;
	rgb_t rgb = { .r = 0, .g = 0, .b = 0 };
	for( uint8_t c = 0; c < 3; c++ )
	{
		for( uint8_t y = 0; y < LED_HEIGHT; y++ )
		{
			for( uint8_t z = 0; z < LED_DEPTH; z++ )
			{
				for( uint8_t x = 0; x < LED_WIDTH; x++ )
				{
					rgb.r = 1;
					set_led( x, y, z, &rgb );
					tlc_gs_data_latch();
					_delay_ms(d);
					
					rgb.r = 0;
					rgb.g = 1;
					set_led( x, y, z, &rgb );
					tlc_gs_data_latch();
					_delay_ms(d);

					rgb.g = 0;
					rgb.b = 1;
					set_led( x, y, z, &rgb );
					tlc_gs_data_latch();
					_delay_ms(d);
					
					rgb.b = 0;
					set_led( x, y, z, &rgb );
				}
			}
		}
	}
}

void purple_test(void)
{
	for( uint8_t c = 0; c < 3; c++ )
	{
		for( uint8_t y = 0; y < LED_HEIGHT; y++ )
		{
			for( uint8_t z = 0; z < LED_DEPTH; z++ )
			{
				rgb_t rgb = { .r = 1, .g = 1, .b = 1 };
				for( uint8_t x = 0; x < LED_WIDTH; x++ )
				{
					set_led( x, y, z, &rgb );
				}
			}
		}
	}
	tlc_gs_data_latch();
}

void led_test(void)
{
	for( uint8_t c = 0; c < 3; c++ )
	{
		for( uint8_t y = 0; y < LED_HEIGHT; y++ )
		{
			for( uint8_t z = 0; z < LED_DEPTH; z++ )
			{
				rgb_t rgb = { .r = 0, .g = 0, .b = 0 };
				switch(c)
				{
					case 0:
						rgb.r = 1;
						break;
					case 1:
						rgb.g = 1;
						break;
					case 2:
						rgb.b = 1;
						break;
				}
				tlc_set_all_gs(50);
				for( uint8_t x = 0; x < LED_WIDTH; x++ )
				{
					set_led( x, y, z, &rgb );
				tlc_gs_data_latch();
				_delay_ms(300);
				}
			}
		}
	}
}
void led_test4(void)
{
	for( uint8_t c = 0; c < 3; c++ )
	{
		for( uint8_t x = 0; x < LED_WIDTH; x++ )
		{
			for( uint8_t y = 0; y < LED_HEIGHT; y++ )
			{
				for( uint8_t z = 0; z < LED_DEPTH; z++ )
				{
					rgb_t rgb = { .r = 0, .g = 0, .b = 0 };
					switch(c)
					{
						case 0:
							rgb.r = 1;
							break;
						case 1:
							rgb.g = 1;
							break;
						case 2:
							rgb.b = 1;
							break;
					}
					set_led( x, y, z, &rgb );
					tlc_gs_data_latch();
					_delay_ms(200);
					rgb.r = rgb.g = rgb.b = 0;
					set_led( x, y, z, &rgb );
				}
			}
		}
	}
	return;
}
void led_test5(void)
{
	for( uint8_t c = 0; c < 3; c++ )
	{
		rgb_t rgb = { .r = 0, .g = 0, .b = 0 };
		for( uint8_t x = 0; x < LED_WIDTH; x++ )
		{
			for( uint8_t y = 0; y < LED_HEIGHT; y++ )
			{
				for( uint8_t z = 0; z < LED_DEPTH; z++ )
				{
					switch(c)
					{
						case 0:
							rgb.r = 1;
							break;
						case 1:
							rgb.g = 1;
							break;
						case 2:
							rgb.b = 1;
							break;
					}
					set_led( x, y, z, &rgb );
				}
			}
		}
		tlc_gs_data_latch();
		_delay_ms(1500);
		rgb.r = rgb.g = rgb.b = 0;
	}
	return;
}
*/
#endif
