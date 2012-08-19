#ifndef _PANELS_SOURCE_
#define _PANELS_SOURCE_
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "driver.h"
#include "panels.h"
#include "audio.h"
#include "ledcube.h"
#include "line.h"


void panels_init(void)
{
	tlc_set_all_gs(0);
	tlc_gs_data_latch();
	return;
}

static hsb_t color = { .h = 0.0, .s = 1.0, .b = 0.5 };
static rgb_t rgb;
void panels_task2(void)
{
    uint16_t delayms = 50;
    hsb_to_rgb( &color, &rgb );

    int z = 0, x = 0, y = 0;
    for( x = 0; x < LED_WIDTH; x++ )
    {
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
	tlc_gs_data_latch();
    tlc_set_all_gs(0);
    _delay_ms(delayms);
  
    /////////// mid
    z = 0;
    for( x = 0; x < LED_WIDTH/2; x++ )
    {
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
    z = 1;
    for( x = LED_WIDTH/2; x < LED_WIDTH; x++ )
    {
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
	tlc_gs_data_latch();
    tlc_set_all_gs(0);
    _delay_ms(delayms);
    
    ////////// pre-diagonal
    for( z = 0; z < LED_WIDTH-1; z++ )
    {
        x = z;
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
    z = LED_WIDTH-2;
    x = LED_WIDTH-1;
    for( y = 0; y < LED_HEIGHT; y++ )
    {
        set_led( x, y, z, &rgb );
    }
	tlc_gs_data_latch();
    tlc_set_all_gs(0);
    _delay_ms(delayms);
   
    ////////// diagonal
    for( z = 0; z < LED_WIDTH; z++ )
    {
        x = z;
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
	tlc_gs_data_latch();
    tlc_set_all_gs(0);
    _delay_ms(delayms);

    ////////// post-diagonal
    for( z = 0; z < LED_WIDTH-1; z++ )
    {
        x = z;
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
    z = LED_WIDTH-1;
    x = LED_WIDTH-2;
    for( y = 0; y < LED_HEIGHT; y++ )
    {
        set_led( x, y, z, &rgb );
    }
	tlc_gs_data_latch();
    tlc_set_all_gs(0);
    _delay_ms(delayms);
   
    /////////// mid
    x = 0;
    for( z = 0; z < LED_WIDTH/2; z++ )
    {
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
    x = 1;
    for( z = LED_WIDTH/2; z < LED_WIDTH; z++ )
    {
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
	tlc_gs_data_latch();
    tlc_set_all_gs(0);
    _delay_ms(delayms);
  
    ///////////////
    x = 0;
    for( z = 0; z < LED_WIDTH; z++ )
    {
        for( y = 0; y < LED_HEIGHT; y++ )
        {
            set_led( x, y, z, &rgb );
        }
    }
	tlc_gs_data_latch();
    tlc_set_all_gs(0);
    _delay_ms(delayms);

	return;
}

void panels_task(void)
{
    rgb_t rgb;
    hsb_to_rgb( &color, &rgb );
    line_t line;

    coord_t start = { .x = 0, .y = 0, .z = 0 };
    coord_t end = start;

    tlc_set_all_gs(0);

   
    end.x = 3;
    end.y = 3;
    make_line( &start, &end, &line );
    render_line( &line, &rgb );
    tlc_gs_data_latch();
    _delay_ms(1000);
   
    start = end;
    end.z = 3;
    end.y = 0;
    make_line( &start, &end, &line );
    render_line( &line, &rgb );
    tlc_gs_data_latch();
    _delay_ms(1000);

    start = end;
    end.z = 3;
    end.y = 3;
    end.x = 0;
    make_line( &start, &end, &line );
    render_line( &line, &rgb );
    tlc_gs_data_latch();
    _delay_ms(1000);

    
    tlc_gs_data_latch();
    _delay_ms(200);
	
    return;
}


#endif
