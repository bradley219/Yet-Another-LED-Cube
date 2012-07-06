#ifndef _TESTS_SOURCE_
#define _TESTS_SOURCE_

#include "config.h"
#include "tests.h"
#include "driver.h"

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
				}
				tlc_gs_data_latch();
				_delay_ms(300);
			}
		}
	}
}
#endif
