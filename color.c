#ifndef _COLOR_SOURCE_
#define _COLOR_SOURCE_
#include <avr/io.h>
#include "color.h"

float process( float input, float temp1, float temp2 ) {
	float output;
	if( input < 0 )
		input += 1.0;
	if( input > 1)
		input -= 1.0;

	if( (6.0 * input) < 1 )
		output = (temp1 + (temp2 - temp1) * 6.0 * input);
	else if( (2.0 * input) < 1 )
		output = temp2;
	else if( (3.0 * input) < 2 )
		output = (temp1 + (temp2 - temp1) * ( 2.0 / 3.0 - input ) * 6.0 );
	else
		output = temp1;
	return( output );
}

void hsb_to_rgb( hsb_t *hsbvals, rgb_t *output ) 
{
	float temp1;
	float temp2;
	float hue;
	float sat;
	float bright;
	float red;
	float green;
	float blue;

	hue = hsbvals->h;
	sat = hsbvals->s;
	bright = hsbvals->b;

	if( sat == 0 ) {
		output->r = (float)bright; 
		output->g = (float)bright; 
		output->b = (float)bright;
		return;
	}
	else if( bright < 0.5 )
		temp2 = bright * ( 1.0 + sat );
	else if( bright >= 0.5 )
		temp2 = bright + sat - bright * sat;

	temp1 = 2.0 * bright - temp2;

	red   = hue + 1.0 / 3.0;
	green = hue;
	blue  = hue - 1.0 / 3.0;

	output->r = process( red, temp1, temp2 );
	output->g = process( green, temp1, temp2 );
	output->b = process( blue, temp1, temp2 );

	return;
}
void alpha_composite( rgb_t *current_rgb, rgb_t *top, double alpha ) 
{

	double original_alpha = 1.0;
	double out_alpha = alpha + original_alpha * (1.0-alpha);

	current_rgb->r = (top->r * alpha + current_rgb->r * original_alpha * (1.0 - alpha)) / out_alpha;
	current_rgb->g = (top->g * alpha + current_rgb->g * original_alpha * (1.0 - alpha)) / out_alpha;
	current_rgb->b = (top->b * alpha + current_rgb->b * original_alpha * (1.0 - alpha)) / out_alpha;

	return;
}

#endif
