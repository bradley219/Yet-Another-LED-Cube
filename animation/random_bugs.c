#ifndef _RANDOM_BUGS_SOURCE_
#define _RANDOM_BUGS_SOURCE_
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "color.h"
#include "random_bugs.h"
#include "driver.h"

typedef struct {
	rgb_t color;
	coord_t pos;
} bug_t;
enum move {
	MOVE_UP = 0,
	MOVE_DOWN = 1,
	MOVE_LEFT = 2,
	MOVE_RIGHT = 3,
	MOVE_BACK = 4,
	MOVE_FORWARD = 5
};
static bug_t bugs[NUM_BUGS];
void random_move( coord_t *coord )
{
	uint8_t moved = 0;
	enum move move = -1;

	while( !moved )
	{
		//move = (double)rand() / (double)RAND_MAX * 6.0;
		move = rand() * 6L / RAND_MAX;

		switch(move)
		{
			case MOVE_UP:
				if( coord->y < (LED_HEIGHT-1) ) {
					moved = 1;
					(coord->y)++;
				}
				break;
			case MOVE_DOWN:
				if( coord->y > 0 ) {
					moved = 1;
					(coord->y)--;
				}
				break;
			case MOVE_RIGHT:
				if( coord->x < (LED_WIDTH-1) ) {
					moved = 1;
					(coord->x)++;
				}
				break;
			case MOVE_LEFT:
				if( coord->x > 0 ) {
					moved = 1;
					(coord->x)--;
				}
				break;
			case MOVE_FORWARD:
				if( coord->z < (LED_DEPTH-1) ) {
					moved = 1;
					(coord->z)++;
				}
				break;
			case MOVE_BACK:
				if( coord->z > 0 ) {
					moved = 1;
					(coord->z)--;
				}
				break;
			default:
				moved = 0;
				break;
		}
	}
	return;
}
void bugs_init(void)
{
	hsb_t color = { .h = 0.32, .s = 1, .b = 0.5 };
	for( int i = 0; i < NUM_BUGS; i++ )
	{
		hsb_to_rgb( &color, &bugs[i].color );
		bugs[i].pos.x = rand() * (long)LED_WIDTH / RAND_MAX;
		bugs[i].pos.y = rand() * (long)LED_HEIGHT / RAND_MAX;
		bugs[i].pos.z = rand() * (long)LED_DEPTH / RAND_MAX;
		color.h = (double)rand() / (double)RAND_MAX;
	}
	return;
}
void bugs_task(void)
{
	tlc_set_all_gs(0);
	
	for( int i = 0; i < NUM_BUGS; i++ )
	{
		random_move( &bugs[i].pos );
		set_led_coord( &bugs[i].pos, &bugs[i].color );
	}
	tlc_gs_data_latch();
	_delay_ms(100);
	return;
}
#endif
