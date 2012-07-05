#include "ledcube.h"

/**
 * TLC
 */

#define NUM_ROWS 16
#define NUM_TLC_CHANNELS (NUM_TLC_CHIPS * 16)
#define TLC_DC_BYTES (NUM_TLC_CHANNELS*6/8)
#define TLC_GS_ROW_BYTES (NUM_TLC_CHANNELS*12/8)
#define TLC_GS_BYTES (TLC_GS_ROW_BYTES*NUM_ROWS)

#define TLC_PWM_PERIOD (4096 * 1)

#define TLC_CYCLE_COUNTS_PER_MULTIPLEX 2

#define PWM_MAX_BLUE  400
#define PWM_MAX_GREEN 400
#define PWM_MAX_RED   4095

struct hsb {
	float h;
	float s;
	float b;
};
struct rgb {
	float r;
	float g;
	float b;
};


volatile uint16_t tlc_cycle_counts = TLC_CYCLE_COUNTS_PER_MULTIPLEX;
volatile uint16_t current_row = 0;
uint8_t tlc_dot_correction_data[TLC_DC_BYTES];

uint8_t tlc_gs_live_data[TLC_GS_BYTES];
uint8_t tlc_gs_data[TLC_GS_BYTES];

void tlc_blank(void)
{
	TLC_BLANK_PORT |= _BV(TLC_BLANK);
}
void tlc_unblank(void)
{
	// Reset TLC timing
	//tlc_blank();
	TCNT1 = 0;
	TIFR1 |= _BV(TOV1);
	TLC_BLANK_PORT &= ~_BV(TLC_BLANK);
}
void tlc_gs_input_mode(void)
{
	TLC_VPRG_PORT &= ~_BV(TLC_VPRG);
}
void tlc_dc_input_mode(void)
{
	TLC_VPRG_PORT |= _BV(TLC_VPRG);
}
void tlc_latch(void)
{
	TLC_XLAT_PORT |= _BV(TLC_XLAT); // 10 ns min
	TLC_XLAT_PORT &= ~_BV(TLC_XLAT);
}

void enable_xlat(void)
{
	TCCR1A |= _BV(COM1A1);
}
void disable_xlat(void)
{
	TCCR1A &= ~_BV(COM1A1);
}
void tlc_timer_init(void)
{

	// GSCLK timer -- high frequency
	TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
	OCR2B = 0;
	OCR2A = 3; // don't touch this
	TCCR2B = _BV(WGM22) | TIMER2_PS_BITS; // no prescale, start

	// Timer 1 - BLANK / XLAT 
	TCCR1A = _BV(COM1B1);  // non inverting, output on OC1B, BLANK
	OCR1A = 1;             // duty factor on OC1A, XLAT is inside BLANK
	OCR1B = 2;             // duty factor on BLANK (larger than OCR1A (XLAT))
	ICR1 = TLC_PWM_PERIOD; // don't touch
	TCCR1B = _BV(WGM13) | TIMER1_PS_BITS;   // Phase/freq correct PWM, ICR1 top, no prescale, start
	TIMSK1 = _BV(TOIE1);
	sei();
	return;
}
void tlc_spi_init(void)
{
	// DDRs
	TLC_BLANK_DDR |= _BV(TLC_BLANK); // SS
	TLC_BLANK_PORT |= _BV(TLC_BLANK);

	TLC_SIN_DDR |= _BV(TLC_SIN); // MOSI
	
	TLC_SCLK_DDR |= _BV(TLC_SCLK); // SCK
	TLC_SCLK_PORT &= ~_BV(TLC_SCLK);

	SPCR = _BV(SPE) | _BV(MSTR);
	SPSR |= _BV(SPI2X); // F_CPU/2
	return;
}
void tlc_sclk_strobe(void)
{
	uint8_t spcr = SPCR;
	TLC_SCLK_DDR |= _BV(TLC_SCLK); // SCK

	SPCR &= ~_BV(SPE); // Disable SPI

	TLC_SCLK_PORT |= _BV(TLC_SCLK); // SCLK high
	TLC_SCLK_PORT &= ~_BV(TLC_SCLK); // SCLK low

	SPCR = spcr; // restore SPI state
	return;
}
void tlc_set_dc( uint8_t channel, uint8_t dc )
{
	uint8_t triplet_idx = channel/4;
	channel -= ( 4 * triplet_idx );
	triplet_idx *= 3;
	
	uint8_t *byte0 = tlc_dot_correction_data + triplet_idx;
	uint8_t *byte1 = byte0+1;
	uint8_t *byte2 = byte1+1;

	switch(channel)
	{
		case 0:
			*byte0 = (*byte0 & 0xc0) | ( dc & 0x3f );
			break;
		case 1:
			*byte0 = (*byte0 & 0x3f) | (( dc & 0x03 ) << 6 );
			*byte1 = (*byte1 & 0xf0) | (dc >> 2);
			break;
		case 2:
			*byte1 = (*byte1 & 0x0f) | ((dc & 0x0f) << 4 );
			*byte2 = (*byte2 & 0xfc) | (dc >> 4);
			break;
		case 3:
			*byte2 = (*byte2 & 0x03) | (dc << 2);
			break;
	}

	return;
}
void tlc_set_all_dc( uint8_t dc )
{
	for( uint16_t i = 0; i < NUM_TLC_CHANNELS; i++ )
	{
		tlc_set_dc( i, dc );
	}
	return;
}
void tlc_shift8( uint8_t byte )
{
	SPDR = byte;
	while( !(SPSR & _BV(SPIF)) );
	return;
}
void tlc_update_dc(void)
{
	tlc_dc_input_mode();

	uint16_t bytes = TLC_DC_BYTES;
	uint8_t *dcd = tlc_dot_correction_data + TLC_DC_BYTES - 1;
	while(bytes--)
	{
		tlc_shift8(*dcd--);
	}
	tlc_latch(); // latch data

	// Send GS data, then strobe SCK
	tlc_update_gs();
	tlc_sclk_strobe();

	return;
}

void tlc_set_gs( uint8_t channel, uint8_t row, uint16_t gs )
{
	uint8_t triplet_idx = channel/2;
	channel -= ( 2 * triplet_idx );
	triplet_idx *= 3;
	
	uint8_t *byte0 = tlc_gs_data + (row*TLC_GS_ROW_BYTES) + triplet_idx;
	uint8_t *byte1 = byte0+1;
	uint8_t *byte2 = byte1+1;

	switch(channel)
	{
		case 0:
			*byte0 = gs & 0xff;
			*byte1 = (*byte1 & 0xf0) | (gs >> 8);
			break;
		case 1:
			*byte1 = (*byte1 & 0x0f) | ((gs & 0x0f) << 4);
			*byte2 = gs >> 4;
			break;
	}

	return;
}
void tlc_set_all_gs( uint16_t gs )
{
	for( uint8_t row = 0; row < NUM_ROWS; row++ )
	{
		for( uint16_t i = 0; i < NUM_TLC_CHANNELS; i++ )
		{
			tlc_set_gs( i, row, gs );
		}
	}
	return;
}
void tlc_gs_data_latch(void)
{
	memcpy( tlc_gs_live_data, tlc_gs_data, sizeof(tlc_gs_data) );
	return;
}
void tlc_update_gs(void)
{
	tlc_gs_input_mode();

	uint16_t bytes = TLC_GS_ROW_BYTES;
	uint8_t *gsd = tlc_gs_live_data + (TLC_GS_ROW_BYTES * current_row) + TLC_GS_ROW_BYTES - 1;
	while(bytes--)
		tlc_shift8(*gsd--);

	enable_xlat(); // latch data when ready
	return;
}
void tlc_set_all_dc_rgb( struct rgb *dc )
{
	for( int i = 0; i < 4; i++ )
	{
		tlc_set_dc( i * 3 + 0, dc->r );
		tlc_set_dc( i * 3 + 1, dc->g );
		tlc_set_dc( i * 3 + 2, dc->b );
	}
}

void tlc_init(void)
{
	// Set in/out data-directions
	TLC_VPRG_PORT &= ~_BV(TLC_VPRG);
	TLC_VPRG_DDR |= _BV(TLC_VPRG); 

	tlc_blank(); // start with blank on
	
	TLC_BLANK_DDR |= _BV(TLC_BLANK);
	TLC_GSCLK_DDR |= _BV(TLC_GSCLK);
	TLC_XLAT_DDR |= _BV(TLC_XLAT);

	tlc_timer_init();
	tlc_spi_init();

	tlc_set_all_gs(0);
	tlc_gs_data_latch();


	struct rgb dc_rgb = { 
		.r = 43, 
		.g = 63, 
		.b = 63
	};
	tlc_set_all_dc_rgb( &dc_rgb );
	tlc_update_dc();
	
	return;
}

/**
 * Shift register 
 */
void shift_register_blank(void)
{
	SHIFT_REG_MR_PORT &= ~_BV(SHIFT_REG_MR);
}
void shift_register_unblank(void)
{
	SHIFT_REG_MR_PORT |= _BV(SHIFT_REG_MR);
}
void shift_register_shift(void)
{
	SHIFT_REG_CP_PORT |=  _BV(SHIFT_REG_CP);
	asm volatile( "nop\n\tnop\n\t" );
	SHIFT_REG_CP_PORT &= ~_BV(SHIFT_REG_CP);
	asm volatile( "nop\n\tnop\n\t" );
}
void shift_register_init(void) 
{
	SHIFT_REG_CP_DDR  |= _BV(SHIFT_REG_CP);
	SHIFT_REG_MR_DDR  |= _BV(SHIFT_REG_MR);
	SHIFT_REG_SIN_DDR |= _BV(SHIFT_REG_SIN);

	SHIFT_REG_CP_PORT &= ~_BV(SHIFT_REG_CP);
	SHIFT_REG_SIN_PORT &= ~_BV(SHIFT_REG_SIN);

	shift_register_blank();
	shift_register_unblank();
	return;
}

volatile uint8_t step_next = 0;
//ISR( TIMER1_OVF_vect, ISR_BLOCK ) // fires when TLC cycle is done
ISR( TIMER1_OVF_vect, ISR_BLOCK ) // fires when TLC cycle is done
{
	disable_xlat(); // disable latches until next data write
	if( step_next )
	{
		if( current_row == 0 )
		{
			SHIFT_REG_SIN_PORT |= _BV(SHIFT_REG_SIN);
			shift_register_shift();
			SHIFT_REG_SIN_PORT &= ~_BV(SHIFT_REG_SIN);
		}
		else
		{
			shift_register_shift();
		}
		step_next = 0;
	}
	if( --tlc_cycle_counts == 0 )
	{
		current_row++;

		if( current_row >= NUM_ROWS ) {
			current_row = 0;
		}

		tlc_update_gs();
		step_next = 1;
		tlc_cycle_counts = TLC_CYCLE_COUNTS_PER_MULTIPLEX;
	}
	return;
}

void set_led( uint8_t x, uint8_t y, uint8_t z, struct rgb *color )
{
	uint8_t row = y + z * 4; // FIXME: don't hardcode 

	uint8_t chanstart = x * 3;
	tlc_set_gs( chanstart,   row, color->r * (double)PWM_MAX_RED );
	tlc_set_gs( chanstart+1, row, color->g * (double)PWM_MAX_GREEN );
	tlc_set_gs( chanstart+2, row, color->b * (double)PWM_MAX_BLUE );

	return;
}
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
void hsb_to_rgb( struct hsb *hsbvals, struct rgb *output ) 
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
	
	return( output );
}

int main(void)
{

	shift_register_init();
	tlc_init();

	tlc_set_all_gs(0);

	struct rgb rgb;
	struct hsb hsb;
	hsb.h = 0;
	hsb.s = 1;
	hsb.b = 0.5;
	
	while(1)
	{
		for( uint8_t x = 0; x < 4; x++ ) {
			for( uint8_t y = 0; y < 4; y++ ) {
				for( uint8_t z = 0; z < 4; z++ ) {

//					hsb.h += 0.00015;

//		hsb.h += 0.4;
		hsb.h += ((double)rand() / (double)RAND_MAX) * 0.20;
		if( hsb.h >= 1 )
			hsb.h -= 1;
		
					
					hsb_to_rgb( &hsb, &rgb );
					
					set_led( x, y, z, &rgb );

				}
			}
		}
	
		tlc_gs_data_latch();

		_delay_ms(100);
	}
	return 0;
}

