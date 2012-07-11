#ifndef _CUBES_SOURCE_
#define _CUBES_SOURCE_
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "driver.h"
#include "cubes.h"

#define CW (LED_WIDTH/CUBE_WIDTH)
#define CH (LED_HEIGHT/CUBE_HEIGHT)
#define CD (LED_DEPTH/CUBE_DEPTH)

typedef enum {
	MOVE_FROM_ABOVE = 1,
	MOVE_FROM_BELOW = 2,
	MOVE_FROM_LEFT = 3,
	MOVE_FROM_RIGHT = 4,
	MOVE_FROM_BACK = 5,
	MOVE_FROM_FRONT = 6,
} move_t;

typedef struct {
	rgb_t color;
	coord_t position;
} cube_t;
static cube_t cubespace[CW][CH][CD];
static coord_t blank_cube = { .x = 0, .y = 0, .z = 0 };

void cubes_init(void)
{
	hsb_t hsb = { .h = 0, .s = 1, .b = 0.5 };
	for( uint8_t x = 0; x < CW; x++ )
	{
		for( uint8_t y = 0; y < CH; y++ )
		{
			for( uint8_t z = 0; z < CD; z++ )
			{
				if( blank_cube.x == x && blank_cube.y == y && blank_cube.z == z )
				{
					cubespace[x][y][z].color.r = 0;
					cubespace[x][y][z].color.g = 0;
					cubespace[x][y][z].color.b = 0;
				}
				else 
				{
					hsb_to_rgb( &hsb, &cubespace[x][y][z].color );
					//hsb.h = (double)rand() / (double)RAND_MAX;
					hsb.h += (double)sizeof(cube_t)/(double)sizeof(cubespace);
				}
			}
		}
	}
	return;
}
void render_cube_starting_at_vertex( coord_t *coord, rgb_t *color )
{
	for( uint8_t x = coord->x; x < (coord->x+CUBE_WIDTH) && x < LED_WIDTH; x++ )
	{
		for( uint8_t y = coord->y; y < (coord->y+CUBE_HEIGHT) && y < LED_HEIGHT; y++ )
		{
			for( uint8_t z = coord->z; z < (coord->z+CUBE_DEPTH) && z < LED_DEPTH; z++ )
			{
				set_led( x, y, z, color );
			}
		}
	}
	return;
}
void render_cubes(void)
{
	for( uint8_t cx = 0; cx < CW; cx++ )
	{
		for( uint8_t cy = 0; cy < CH; cy++ )
		{
			for( uint8_t cz = 0; cz < CD; cz++ )
			{
				coord_t vertex;
				vertex.x = cx * CUBE_WIDTH;
				vertex.y = cy * CUBE_HEIGHT;
				vertex.z = cz * CUBE_DEPTH;
				render_cube_starting_at_vertex( &vertex, &cubespace[cx][cy][cz].color );
			}
		}
	}
	return;
}

move_t pick_random_move( move_t *possible, uint8_t count )
{
	uint8_t index = rand() * (long)count / RAND_MAX;
	move_t theMove = possible[index];
	return theMove;
}

void move_into_blank(void)
{
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
	static move_t last_move = 0;
#endif
	uint8_t pmc = 0; // possible moves count
	move_t possible_moves[6];
	move_t *pmp = possible_moves;

	if( blank_cube.x > 0 ) {
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		if( last_move != MOVE_FROM_RIGHT ) {
#endif
			pmc++;
			*pmp++ = MOVE_FROM_LEFT;
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		}
#endif
	}
	if( blank_cube.x < (CW-1) ) {
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		if( last_move != MOVE_FROM_LEFT ) {
#endif
			pmc++;
			*pmp++ = MOVE_FROM_RIGHT;
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		}
#endif
	} 
	if( blank_cube.y > 0 ) {
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		if( last_move != MOVE_FROM_ABOVE ) {
#endif
			pmc++;
			*pmp++ = MOVE_FROM_BELOW;
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		}
#endif
	}
	if( blank_cube.y < (CH-1) ) {
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		if( last_move != MOVE_FROM_BELOW ) {
#endif
			pmc++;
			*pmp++ = MOVE_FROM_ABOVE;
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		}
#endif
	}
	if( blank_cube.z > 0 ) {
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		if( last_move != MOVE_FROM_BACK ) {
#endif
			pmc++;
			*pmp++ = MOVE_FROM_FRONT;
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		}
#endif
	}
	if( blank_cube.z < (CD-1) ) {
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		if( last_move != MOVE_FROM_FRONT ) {
#endif
			pmc++;
			*pmp++ = MOVE_FROM_BACK;
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
		}
#endif
	}

	move_t mymove = pick_random_move( possible_moves, pmc );
#ifdef DISALLOW_UNDO_OF_PREVIOUS_SLIDE
	last_move = mymove;
#endif
	
	coord_t target_cube; // cube we will be moving
	memcpy( &target_cube, &blank_cube, sizeof(coord_t) ); // copy the blank cube's location

	// Manipulate the blank cube's position to locate the target cube
	switch(mymove)
	{
		case MOVE_FROM_ABOVE:
			(target_cube.y)++;
			break;
		case MOVE_FROM_BELOW:
			(target_cube.y)--;
			break;
		case MOVE_FROM_LEFT:
			(target_cube.x)--;
			break;
		case MOVE_FROM_RIGHT:
			(target_cube.x)++;
			break;
		case MOVE_FROM_BACK:
			(target_cube.z)++;
			break;
		case MOVE_FROM_FRONT:
			(target_cube.z)--;
			break;
	}

	// Color of cube we're moving
	rgb_t *color = &cubespace[target_cube.x][target_cube.y][target_cube.z].color;
	rgb_t black = { .r = 0, .g = 0, .b = 0 };

	// Cube vertex start and end points
	coord_t start_vertex,end_vertex;
	start_vertex.x = target_cube.x * CUBE_WIDTH;
	start_vertex.y = target_cube.y * CUBE_HEIGHT;
	start_vertex.z = target_cube.z * CUBE_DEPTH;
	
	end_vertex.x = blank_cube.x * CUBE_WIDTH;
	end_vertex.y = blank_cube.y * CUBE_HEIGHT;
	end_vertex.z = blank_cube.z * CUBE_DEPTH;

	int xinc = 0, yinc = 0, zinc = 0;
	if( start_vertex.x < end_vertex.x )
		xinc = 1;
	else if( start_vertex.x > end_vertex.x )
		xinc = -1;
	if( start_vertex.y < end_vertex.y )
		yinc = 1;
	else if( start_vertex.y > end_vertex.y )
		yinc = -1;
	if( start_vertex.z < end_vertex.z )
		zinc = 1;
	else if( start_vertex.z > end_vertex.z )
		zinc = -1;

	// Do stepwise animation

	do {
		// Blank the previous frame
		render_cube_starting_at_vertex( &start_vertex, &black ); 

		// Increment all values
		(start_vertex.x) += xinc;
		(start_vertex.y) += yinc;
		(start_vertex.z) += zinc;

		// Render the cube at the new vertex with color
		render_cube_starting_at_vertex( &start_vertex, color );

		// Latch data
		tlc_gs_data_latch(); 

		// Animation step delay
		_delay_ms(CUBE_STEP_ANIMATION_DELAY_MS);

	} while( memcmp( &start_vertex, &end_vertex, sizeof(coord_t) ) ); // When the start and end points match, break out

	// Swap colors in cubespace
	memcpy( &cubespace[blank_cube.x][blank_cube.y][blank_cube.z].color,
			&cubespace[target_cube.x][target_cube.y][target_cube.z].color, sizeof(rgb_t) );

	// Blank the color of blank_cube in cubespace
	memcpy( &cubespace[target_cube.x][target_cube.y][target_cube.z].color, &black, sizeof(rgb_t) );

	// Save the new location of the blank_cube
	memcpy( &blank_cube, &target_cube, sizeof(coord_t) );

}

void cubes_task(void)
{
	render_cubes();
	tlc_gs_data_latch();
	//_delay_ms(CUBE_SWAP_DELAY_MS);
	_delay_ms( rand() * 300L / RAND_MAX + 5L );
	move_into_blank();
}

#endif
