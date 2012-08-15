#ifndef _CUBES_SOURCE_
#define _CUBES_SOURCE_
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "driver.h"
#include "snake.h"
#include "audio.h"
#include "ledcube.h"

#define NUM_SNAKES 1
#define SNAKE_LENGTH 28
#define SNAKE_COLOR_STEP 0.001

#define SNAKE_MIN_DELAY 1
#define SNAKE_MAX_DELAY 15

#define RAINBOW_SNAKE

//#define MORE_STRAIGHT_PROBABILITY

typedef enum {
	MOVE_UP = 1,
	MOVE_DOWN = 2,
	MOVE_LEFT = 3,
	MOVE_RIGHT = 4,
	MOVE_FORWARD = 5,
	MOVE_BACKWARD = 6,
} move_t;

typedef struct {
	uint8_t length;
	move_t last_move;
	coord_t coords[SNAKE_LENGTH];
	hsb_t color;
} snake_t;

static snake_t snakes[NUM_SNAKES];

void snake_init(void)
{
	snake_t *sp = snakes;
	float hue = (double)rand() / (double)RAND_MAX;
	float hue_step = 1.0 / (double)NUM_SNAKES;
	for( int i = 0; i < NUM_SNAKES; i++ )
	{
		sp->length = 0;
		sp->color.h = hue;
		sp->color.s = 1;
		sp->last_move = 0;
			
		hue += hue_step;

		if( hue >= 1 )
			hue = 0;

		sp++;
	}

	tlc_set_all_gs(0);
	tlc_gs_data_latch();
	return;
}
void move_snake_buffer_down( snake_t *snake )
{
	for( uint8_t i = SNAKE_LENGTH-1; i >= 1; i-- )
	{
		memcpy( &(snake->coords[i]), &(snake->coords[i-1]), sizeof(coord_t) );
	}
	return;
}

move_t pick_random_snake_move( move_t *possible, uint8_t count )
{
	uint8_t index = rand() * (long)count / RAND_MAX;
	move_t theMove = possible[index];
	return theMove;
}
void snake_move( snake_t *snake )
{
	if( snake->length < SNAKE_LENGTH )
		(snake->length)++;

	move_snake_buffer_down(snake);

	coord_t *head = &(snake->coords[0]);

	if( snake->length == 1 ) // first move
	{
		head->x = rand() * (long)LED_WIDTH / RAND_MAX;
		head->y = rand() * (long)LED_HEIGHT / RAND_MAX;
		head->z = rand() * (long)LED_DEPTH / RAND_MAX;
	}
	else 
	{
		uint8_t pmc = 0;
		move_t possible_moves[12];
		move_t *pmp = possible_moves;

		if( head->x > 0 ) {
			if( snake->last_move != MOVE_LEFT ) {
				pmc++;
				*pmp++ = MOVE_RIGHT;
#ifdef MORE_STRAIGHT_PROBABILITY
				if( snake->last_move == MOVE_RIGHT ) {
					pmc++;
					*pmp++ = MOVE_RIGHT;
					pmc++;
					*pmp++ = MOVE_RIGHT;
				}
#endif
			}
		}
		if( head->x < (LED_WIDTH-1) ) {
			if( snake->last_move != MOVE_RIGHT ) {
				pmc++;
				*pmp++ = MOVE_LEFT;
#ifdef MORE_STRAIGHT_PROBABILITY
				if( snake->last_move == MOVE_LEFT ) {
					pmc++;
					*pmp++ = MOVE_LEFT;
					pmc++;
					*pmp++ = MOVE_LEFT;
				}
#endif
			}
		}
		if( head->y > 0 ) {
			if( snake->last_move != MOVE_UP ) {
				pmc++;
				*pmp++ = MOVE_DOWN;
#ifdef MORE_STRAIGHT_PROBABILITY
				if( snake->last_move == MOVE_DOWN ) {
					pmc++;
					*pmp++ = MOVE_DOWN;
					pmc++;
					*pmp++ = MOVE_DOWN;
				}
#endif
			}
		}
		if( head->y < (LED_HEIGHT-1) ) {
			if( snake->last_move != MOVE_DOWN ) {
				pmc++;
				*pmp++ = MOVE_UP;
#ifdef MORE_STRAIGHT_PROBABILITY
				if( snake->last_move == MOVE_UP ) {
					pmc++;
					*pmp++ = MOVE_UP;
					pmc++;
					*pmp++ = MOVE_UP;
				}
#endif
			}
		}
		if( head->z > 0 ) {
			if( snake->last_move != MOVE_BACKWARD ) {
				pmc++;
				*pmp++ = MOVE_FORWARD;
#ifdef MORE_STRAIGHT_PROBABILITY
				if( snake->last_move == MOVE_FORWARD ) {
					pmc++;
					*pmp++ = MOVE_FORWARD;
					pmc++;
					*pmp++ = MOVE_FORWARD;
				}
#endif
			}
		}
		if( head->z < (LED_DEPTH-1) ) {
			if( snake->last_move != MOVE_FORWARD ) {
				pmc++;
				*pmp++ = MOVE_BACKWARD;
#ifdef MORE_STRAIGHT_PROBABILITY
				if( snake->last_move == MOVE_BACKWARD ) {
					pmc++;
					*pmp++ = MOVE_BACKWARD;
					pmc++;
					*pmp++ = MOVE_BACKWARD;
				}
#endif
			}
		}

		move_t mymove = pick_random_snake_move( possible_moves, pmc );
		snake->last_move = mymove;
		switch(mymove)
		{
			case MOVE_RIGHT:
				(head->x)--;
				break;
			case MOVE_LEFT:
				(head->x)++;
				break;
			case MOVE_DOWN:
				(head->y)--;
				break;
			case MOVE_UP:
				(head->y)++;
				break;
			case MOVE_FORWARD:
				(head->z)--;
				break;
			case MOVE_BACKWARD:
				(head->z)++;
				break;
		}
	}
}

void render_snake(snake_t *snake, uint8_t num )
{
	float bright_step = 0.50 / (double)snake->length;

//    snake->color.h = main_color.h;
//    main_color.h += SNAKE_COLOR_STEP;
//    if( main_color.h >= 1 )
//        main_color.h -= 1.0;
	snake->color.h += SNAKE_COLOR_STEP;// + (double)rand() / (double)RAND_MAX * ((double)SNAKE_COLOR_STEP/4.0);
	if( snake->color.h >= 1 )
		snake->color.h = 0;

	rgb_t newrgb;

	coord_t *pos = snake->coords + snake->length - 1;


	snake->color.b = 0.0;
	hsb_t color = snake->color;
#ifdef RAINBOW_SNAKE
	float hue_step = 0.20 / (double)snake->length;
#endif
	for( uint8_t i = 0; i < snake->length; i++ ) 
	{
		color.b += bright_step;
#ifdef RAINBOW_SNAKE
		color.h += hue_step;

		if( color.h >= 1.0 )
			color.h -= 1.0;
#endif

        if( color.b > 0.5 )
            color.b = 0.5;
        if( pos == snake->coords )
        {
                color.b = 0.5;
        }
//		if( color.b > 0.5 || pos == snake->coords )
//			color.b = 0.5;
		hsb_to_rgb( &color, &newrgb );

		rgb_t oldrgb;
		get_led_coord( pos, &oldrgb );

		if( oldrgb.r || oldrgb.g || oldrgb.b ) {
			alpha_composite( &oldrgb, &newrgb, 0.5 );
			set_led_coord( pos, &oldrgb );
		}
		else {
			set_led_coord( pos, &newrgb );
		}

		pos--;
	}
}

static float snake_delay = (SNAKE_MIN_DELAY + SNAKE_MAX_DELAY) / 2;
static const float snake_delay_exp = 7;

void snake_task(void)
{
    static unsigned long timer = 0;
    
    float inc = ((double)rand() / (double)RAND_MAX - 0.5) * 5;
    snake_delay = snake_delay * (snake_delay_exp-1.0) / snake_delay_exp + (inc+snake_delay) / snake_delay_exp;

    if( snake_delay < SNAKE_MIN_DELAY )
        snake_delay = SNAKE_MIN_DELAY;
    else if( snake_delay > SNAKE_MAX_DELAY )
        snake_delay = SNAKE_MAX_DELAY;
   
    if( timer++ < snake_delay )
    {
        return;
    }


    timer = 0;
	tlc_set_all_gs(0);


	snake_t *sp = snakes;
	for( int i = 0; i < NUM_SNAKES; i++ )
	{
		snake_move(sp);
		render_snake(sp, i);
		sp++;
	}
	tlc_gs_data_latch();
	return;
}


#endif
