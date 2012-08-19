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

void triangle( uint8_t low, uint8_t high, rgb_t *rgb )
{
    line_t line;

    coord_t start = { .x = 0, .y = low, .z = 0 };
    coord_t end = start;

    tlc_set_all_gs(0);
   
    end.x = 3;
    end.y = high;
    make_line( &start, &end, &line );
    render_line( &line, rgb );
   
    start = end;
    end.x = 3;
    end.y = low;
    end.z = 3;
    make_line( &start, &end, &line );
    render_line( &line, rgb );
    
    start = end;
    end.z = 3;
    end.y = high;
    end.x = 0;
    make_line( &start, &end, &line );
    render_line( &line, rgb );
    
    start = end;
    end.z = 0;
    end.y = low;
    end.x = 0;
    make_line( &start, &end, &line );
    render_line( &line, rgb );

    return;
}

void panels_task(void)
{
    rgb_t rgb;
    hsb_to_rgb( &color, &rgb );
    color.h += 0.0001;
    if( color.h >= 1 )
        color.h -= 1.0;

 
    uint16_t delay = 100;
    for( uint8_t i = 0; i < 4; i++ )
    {
        triangle( i, 3-i, &rgb );
        tlc_gs_data_latch();
        _delay_ms(delay);
    }
    for( uint8_t i = 2; i >= 1; i-- )
    {
        triangle( i, 3-i, &rgb );
        tlc_gs_data_latch();
        _delay_ms(delay);
    }
    
	
    return;
}


#endif
