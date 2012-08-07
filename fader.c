#ifndef _CUBES_SOURCE_
#define _CUBES_SOURCE_
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "driver.h"
#include "fader.h"



void fader_init(void)
{
	tlc_set_all_gs(0);
	tlc_gs_data_latch();
	return;
}

void rando(void)
{
	static hsb_t color = { .h = 0.0 , .s = 1, .b = 0.5 };
    rgb_t rgb;

    int delay = 100;
    uint8_t x = 0;
    for( x = 0; x < LED_HEIGHT; x++ )
    {
        for( uint8_t y = 0; y < LED_WIDTH; y++ )
        {
            for( uint8_t z = 0; z < LED_DEPTH; z++ )
            {
                color.h = (double)rand() / (double)RAND_MAX;
                color.b = ((double)rand() / (double)RAND_MAX) * 0.5;
                color.s = ((double)rand() / (double)RAND_MAX) * 0.25 + 1.0 - 0.25;
                hsb_to_rgb( &color, &rgb );
                set_led( x, y, z, &rgb );
            }
        }
    }
    tlc_gs_data_latch();
    _delay_ms(delay);
		
    return;
}

void solid_fader_task(void)
{
	static hsb_t color = { .h = 0.0 , .s = 1, .b = 0.5 };
    rgb_t rgb;

    color.h += 1.0/500.0;
    if( color.h >=1 )
        color.h -= 1.0;
	hsb_to_rgb( &color, &rgb );

    int delay = 50;
    uint8_t x = 0;
    for( x = 0; x < LED_HEIGHT; x++ )
    {
        for( uint8_t y = 0; y < LED_WIDTH; y++ )
        {
            for( uint8_t z = 0; z < LED_DEPTH; z++ )
            {
                set_led( x, y, z, &rgb );
            }
        }
        tlc_gs_data_latch();
	    tlc_set_all_gs(0);
        _delay_ms(delay);
    }
    x--;
    while( x-- - 1 )
    {
        for( uint8_t y = 0; y < LED_WIDTH; y++ )
        {
            for( uint8_t z = 0; z < LED_DEPTH; z++ )
            {
                set_led( x, y, z, &rgb );
            }
        }
        tlc_gs_data_latch();
	    tlc_set_all_gs(0);
        _delay_ms(delay);
    }
		
    return;
}

void fader_task(void)
{
	static hsb_t color = { .h = 0.0 , .s = 1, .b = 0.5 };
	rgb_t rgb;

	long delay = rand() * 25L / RAND_MAX + 20;
	delay = 1500;

	float max_bright = 0.5;
	float min_bright = 0;

	uint8_t num_steps_requested = 5;

	float bright_step = (max_bright-min_bright) / (float)num_steps_requested;
	uint8_t num_steps = num_steps_requested * 2 - 1;

	color.h += 1.0 / 50.0;
//	color.h += 1.0 / 3.0;
	//color.h += (double)rand() / (double)RAND_MAX * (1.0/3.0);
	if( color.h >= 1 )
		color.h -= 1;

	//color.s = (double)rand() / (double)RAND_MAX * 0.25 + 0.75;

//	for( float hue = 0; hue < 1; hue += 0.08 )
//	{
		//color.h = hue;

		/**
		 * Fade in
		 */
		float bright = min_bright - (bright_step * (float)(LED_MAX_DIMENSION-1));
		for( uint8_t step = 0; step < num_steps; step++ )
		{
			float b = bright;// + bright_step;

			for( uint8_t N = LED_WIDTH; N > 0; N-- )
			{
				color.b = b;
				if( color.b > max_bright )
					color.b = max_bright;
				else if( color.b < min_bright )
					color.b = min_bright;

				b += bright_step;
			
				hsb_to_rgb( &color, &rgb );

				// Y-Z plane
				for( uint8_t y = 0; y < N; y++ )
				{
					for( uint8_t z = 0; z < N; z++ )
					{
						set_led( N - 1, y, z, &rgb );

					}
				}
				// X-Y plane
				for( uint8_t y = 0; y < N; y++ )
				{
					for( uint8_t x = 0; x < N; x++ )
					{
						set_led( x, y, N - 1, &rgb );
					}
				}
				// X-Z plane
				for( uint8_t z = 0; z < N; z++ )
				{
					for( uint8_t x = 0; x < N; x++ )
					{
						set_led( x, N - 1, z, &rgb );
					}
				}
			tlc_gs_data_latch();
			}
			tlc_gs_data_latch();
			_delay_ms(delay);
			bright += bright_step;
		}
		
		/**
		 * Fade out
		 */
		//bright = max_bright + (bright_step * (float)(LED_MAX_DIMENSION-1));
		bright -= bright_step;
		for( uint8_t step = 0; step < num_steps; step++ )
		{
			float b = bright;

			for( uint8_t N = LED_WIDTH; N >= 1; N-- )
			{
				color.b = b;
				if( color.b > max_bright )
					color.b = max_bright;
				else if( color.b < min_bright )
					color.b = min_bright;

				b -= bright_step;
			
				hsb_to_rgb( &color, &rgb );

				// Y-Z plane
				for( uint8_t y = N-1; y < LED_WIDTH; y++ )
				{
					for( uint8_t z = N-1; z < LED_WIDTH; z++ )
					{
						set_led( N - 1, y, z, &rgb );
					}
				}
				// X-Y plane
				for( uint8_t y = N-1; y < LED_WIDTH; y++ )
				{
					for( uint8_t x = N-1; x < LED_WIDTH; x++ )
					{
						set_led( x, y, N - 1, &rgb );
					}
				}
				// X-Z plane
				for( uint8_t z = N-1; z < LED_WIDTH; z++ )
				{
					for( uint8_t x = N-1; x < LED_WIDTH; x++ )
					{
						set_led( x, N - 1, z, &rgb );
					}
				}
			tlc_gs_data_latch();
			}
			tlc_gs_data_latch();
			_delay_ms(delay);
			bright -= bright_step;
		}
	//}

	return;
}

#endif
