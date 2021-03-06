#ifndef _COLOR_H_
#define _COLOR_H_
#include "types.h"
#include "config.h"
#include "ports.h"

float process( float input, float temp1, float temp2 );
void hsb_to_rgb( hsb_t *hsbvals, rgb_t *output );
void alpha_composite( rgb_t *current_rgb, rgb_t *top, double alpha );

#endif
