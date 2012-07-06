#include "ledcube.h"

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

#define CUBE_HEIGHT 2
#define CUBE_WIDTH 2
#define CUBE_DEPTH 2

void set_cube_color( cube_t *cube, hsb_t *color )
{
	hsb_to_rgb( color, &cube->color );
	return;
}
void set_cube_position( cube_t *cube, coord_t *coord )
{
	memcpy( &cube->position, coord, sizeof(coord_t) );
	return;
}
void render_cube( cube_t *cube )
{
	for( uint8_t x = cube->position.x; x < (cube->position.x+CUBE_WIDTH) && x < LED_WIDTH; x++ )
	{
		for( uint8_t y = cube->position.y; y < (cube->position.y+CUBE_HEIGHT) && y < LED_HEIGHT; y++ )
		{
			for( uint8_t z = cube->position.z; z < (cube->position.z+CUBE_DEPTH) && z < LED_DEPTH; z++ )
			{
				set_led( x, y, z, &cube->color );
			}
		}
	}
	return;
}

enum move {
	MOVE_UP = 0,
	MOVE_DOWN = 1,
	MOVE_LEFT = 2,
	MOVE_RIGHT = 3,
	MOVE_BACK = 4,
	MOVE_FORWARD = 5
};
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

typedef struct {
	rgb_t color;
	coord_t pos;
} bug_t;
#define NUM_BUGS 10

void analog_srand(void)
{
	uint8_t admux = ADMUX;
	uint8_t adcsra = ADCSRA;
	uint16_t data = 1;
	ADMUX = _BV(REFS0) | _BV(MUX0); // AVcc reference, ADC1 input
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // prescale 128

	long seed = 1;
	for( int i = 0; i < 100; i++ ) {
		
		_delay_us(data);
		
		ADCSRA |= _BV(ADSC);
		while( !(ADCSRA & _BV(ADIF)) );
		data = ADCL;
		data |= ADCH << 8;

		seed += data * data;
	}
	srand(seed);

	ADCSRA = adcsra;
	ADMUX = admux;
	return;
}
void analog_init(void)
{
	// digital input disable
	DIDR0 = _BV(ADC5D) | _BV(ADC4D) | _BV(ADC3D) | _BV(ADC2D) | _BV(ADC0D);
	return;
}

int main(void)
{
	analog_init();
	analog_srand();

	led_driver_init();

	tlc_set_all_gs(0);

	bug_t bugs[NUM_BUGS];

	hsb_t color = { .h = 0.32, .s = 1, .b = 0.5 };
	for( int i = 0; i < NUM_BUGS; i++ )
	{
		hsb_to_rgb( &color, &bugs[i].color );
		bugs[i].pos.x = rand() * (long)LED_WIDTH / RAND_MAX;
		bugs[i].pos.y = rand() * (long)LED_HEIGHT / RAND_MAX;
		bugs[i].pos.z = rand() * (long)LED_DEPTH / RAND_MAX;
		color.h = (double)rand() / (double)RAND_MAX;
	}

	while(1) 
	{
		tlc_set_all_gs(0);
		
		for( int i = 0; i < NUM_BUGS; i++ )
		{
			random_move( &bugs[i].pos );
			set_led_coord( &bugs[i].pos, &bugs[i].color );
		}
		tlc_gs_data_latch();
		//_delay_ms(1000);
	}

	return 0;
}

