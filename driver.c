#ifndef _DRIVER_SOURCE_
#define _DRIVER_SOURCE_

#include "ports.h"
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "types.h"
#include "config.h"
#include "driver.h"
#include "asmdriver.h"

#define TLC_DC_INPUT_MODE() TLC_VPRG_PORT |= _BV(TLC_VPRG)
#define TLC_LATCH() TLC_XLAT_PORT |= _BV(TLC_XLAT); TLC_XLAT_PORT &= ~_BV(TLC_XLAT)

volatile uint8_t auto_gs_enabled = 0;
volatile uint8_t tlc_cycle_counts = TLC_CYCLE_COUNTS_PER_MULTIPLEX;
volatile uint16_t current_row = 0;
static uint8_t tlc_dot_correction_data[TLC_DC_BYTES];

uint8_t tlc_gs_live_data[TLC_GS_BYTES];
static uint8_t tlc_gs_data[TLC_GS_BYTES];

volatile uint8_t spi_bytes_remaining = TLC_GS_ROW_BYTES;
volatile uint8_t *spi_gsd = tlc_gs_live_data + sizeof(tlc_gs_live_data) - TLC_GS_ROW_BYTES;

void tlc_timer_init(void)
{
	// GSCLK timer -- high frequency
	TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
	OCR2B = TIMER2_GS_TRIGGER;
	OCR2A = TIMER2_TOPVAL; // TOP value - min value is 3
	TCCR2B = _BV(WGM22) | TIMER2_PS_BITS; // no prescale, start

	// Timer 1 - BLANK / XLAT 
	TCCR1A = _BV(COM1B1);  // non inverting, output on OC1B, BLANK
	OCR1A = 1;             // duty factor on OC1A, XLAT is inside BLANK
	OCR1B = 2;             // duty factor on BLANK (larger than OCR1A (XLAT))
	ICR1 = TLC_PWM_PERIOD + 76 / 2;  // 76 is the minimum number of clock cycles the ISR takes to complete
	TCCR1B = _BV(WGM13) | TIMER1_PS_BITS;   // Phase/freq correct PWM, ICR1 top, no prescale, start
	TIMSK1 = _BV(TOIE1);
	sei();
	return;
}

/**
 * SPI
 */
void tlc_spi_init(void)
{
	// DDRs
	TLC_BLANK_DDR |= _BV(TLC_BLANK); // SS
	TLC_BLANK_PORT |= _BV(TLC_BLANK);

	TLC_SIN_DDR |= _BV(TLC_SIN); // MOSI
	
	TLC_SCLK_DDR |= _BV(TLC_SCLK); // SCK
	TLC_SCLK_PORT &= ~_BV(TLC_SCLK);

	SPCR = _BV(SPE) | _BV(MSTR);// | SPI_PS_BITS;
	SPSR |= _BV(SPI2X);
	return;
}

void tlc_sclk_strobe(void) // special case
{
	uint8_t spcr = SPCR;
	TLC_SCLK_DDR |= _BV(TLC_SCLK); // SCK

	SPCR &= ~_BV(SPE); // Disable SPI

	TLC_SCLK_PORT |= _BV(TLC_SCLK); // SCLK high
	TLC_SCLK_PORT &= ~_BV(TLC_SCLK); // SCLK low

	SPCR = spcr; // restore SPI state
	return;
}

// TLC data setting methods
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
	TLC_DC_INPUT_MODE();

	uint16_t bytes = TLC_DC_BYTES;
	uint8_t *dcd = tlc_dot_correction_data + TLC_DC_BYTES - 1;
	while(bytes--)
	{
		tlc_shift8(*dcd--);
	}
	TLC_LATCH(); // latch data

	// Send GS data, then strobe SCK
	uint8_t c = TLC_GS_ROW_BYTES;
	while(c--)
	{
		SPDR = 0;
		while( !(SPSR & _BV(SPIF)) );
	}
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
void tlc_get_gs( uint8_t channel, uint8_t row, uint16_t *gs )
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
			//*byte0 = gs & 0xff;
			*gs = *byte0;
			//*byte1 = (*byte1 & 0xf0) | (gs >> 8);
			*gs |= (*byte1 & 0x0f) << 8;
			break;
		case 1:
			*gs = *byte2 << 4;
			*gs |= (*byte1 & 0xf0) >> 4;

			//*byte1 = (*byte1 & 0x0f) | ((gs & 0x0f) << 4);
			//*byte2 = gs >> 4;
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
	//cli();
	memcpy( tlc_gs_live_data, tlc_gs_data, sizeof(tlc_gs_data) );
	//sei();
	return;
}
void tlc_set_all_dc_rgb( rgb_t *dc )
{
	for( int i = 0; i < 4; i++ )
	{
		tlc_set_dc( i * 3 + 0, dc->r );
		tlc_set_dc( i * 3 + 1, dc->g );
		tlc_set_dc( i * 3 + 2, dc->b );
	}
}

ISR( SPI_STC_vect, ISR_BLOCK )
{
	asm volatile( "wdr\n\t" );
	if( spi_bytes_remaining-- )
	{
		SPDR = *--spi_gsd;
	}
	else 
	{
		// enable timer1 interrupt
		//TIMSK1 |= _BV(TOIE1);
	}

	return;
}

void tlc_init(void)
{
	// Set in/out data-directions
	TLC_VPRG_PORT &= ~_BV(TLC_VPRG);
	TLC_VPRG_DDR |= _BV(TLC_VPRG); 

	TLC_BLANK_PORT |= _BV(TLC_BLANK);
	
	TLC_BLANK_DDR |= _BV(TLC_BLANK);
	TLC_GSCLK_DDR |= _BV(TLC_GSCLK);
	TLC_XLAT_DDR |= _BV(TLC_XLAT);

	rgb_t dc_rgb = { 
		.r = DOT_CORRECTION_RED, 
		.g = DOT_CORRECTION_GREEN, 
		.b = DOT_CORRECTION_BLUE
	};
	tlc_set_all_dc_rgb( &dc_rgb );
	
	tlc_spi_init();
	tlc_update_dc();

	auto_gs_enabled = 1;
	//tlc_gs_start_spi_chain();
	
	TLC_VPRG_PORT &= ~_BV(TLC_VPRG);
	SPSR |= _BV(SPIF);
	SPCR |= _BV(SPIE);

	tlc_timer_init();
	
	tlc_set_all_gs(0);
	tlc_gs_data_latch();
	
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
void shift_register_init(void) 
{
	SHIFT_REG_CP_DDR  |= _BV(SHIFT_REG_CP);
	SHIFT_REG_MR_DDR  |= _BV(SHIFT_REG_MR);
	SHIFT_REG_SIN_DDR |= _BV(SHIFT_REG_SIN);

	SHIFT_REG_CP_PORT &= ~_BV(SHIFT_REG_CP);
	SHIFT_REG_SIN_PORT &= ~_BV(SHIFT_REG_SIN);

void tlc_shift8( uint8_t byte )
{
	SPDR = byte;
	while( !(SPSR & _BV(SPIF)) );
	return;
}
	shift_register_blank();
	shift_register_unblank();
	return;
}

void set_led_raw( uint8_t x, uint8_t y, uint8_t z, uint16_t red, uint16_t green, uint16_t blue )
{
	uint8_t row = y + z * 4; // FIXME: don't hardcode 

	uint8_t chanstart = x * 3;
	tlc_set_gs( chanstart,   row, red );
	tlc_set_gs( chanstart+1, row, green );
	tlc_set_gs( chanstart+2, row, blue );

	return;
}
void set_led( uint8_t x, uint8_t y, uint8_t z, rgb_t *color )
{
	uint8_t row = y + z * 4; // FIXME: don't hardcode 

	uint8_t chanstart = x * 3;

	uint16_t red = color->r * (double)PWM_MAX_RED;
	uint16_t green = color->g * (double)PWM_MAX_GREEN;
	uint16_t blue = color->b * (double)PWM_MAX_BLUE;

	tlc_set_gs( chanstart,   row, red );
	tlc_set_gs( chanstart+1, row, green );
	tlc_set_gs( chanstart+2, row, blue );

	return;
}
void get_led( uint8_t x, uint8_t y, uint8_t z, rgb_t *color )
{
	uint8_t row = y + z * 4; // FIXME: don't hardcode 

	uint8_t chanstart = x * 3;

	uint16_t red,green,blue;
	tlc_get_gs( chanstart, row, &red );
	tlc_get_gs( chanstart+1, row, &green );
	tlc_get_gs( chanstart+2, row, &blue );

	color->r = (double)red / (double)PWM_MAX_RED;
	color->g = (double)green / (double)PWM_MAX_GREEN;
	color->b = (double)blue / (double)PWM_MAX_BLUE;

	return;
}
void set_led_coord( coord_t *coord, rgb_t *color )
{
	set_led( coord->x, coord->y, coord->z, color );
}
void get_led_coord( coord_t *coord, rgb_t *color )
{
	get_led( coord->x, coord->y, coord->z, color );
}
void wdt_init(void)
{
	WDTCSR |= _BV(WDCE);
	WDTCSR = 0;//_BV(WDE);// | _BV(WDP1) | _BV(WDP2) | _BV(WDP0);// | _BV(WDP0);
	return;
}
void led_driver_init(void)
{
	shift_register_init();
	tlc_init();
	wdt_init();
	return;
}

#endif
