#ifndef _AUDIO_SOURCE_
#define _AUDIO_SOURCE_

#include "ports.h"
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "types.h"
#include "config.h"
#include "color.h"
#include "driver.h"
#include "audio.h"
#include "asmdriver.h"
#include "ffft.h"

volatile int16_t audio_buffer[AUDIO_BUFFER_SIZE];
volatile int16_t *abp = audio_buffer;
volatile uint8_t abc = AUDIO_BUFFER_SIZE;

// 4099390717
void audio_init(void)
{
	DDRD |= _BV(PORTD5);
	OCR0A = 20;
	OCR0B = OCR0A / 2;
	TCCR0A = _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(WGM02) | TIMER0_PS_BITS;

	// digital input disable
	//DIDR0 = _BV(ADC5D) | _BV(ADC4D) | _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D);

	//DDRC &= ~_BV(PORTC0);

	ADMUX = _BV(REFS0);
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADATE);
	ADCSRB = 0;

	sei();
	ADCSRA |= _BV(ADSC);

	return;
}
//void audio_task2(void)
//{
//	static double last_sum = 0;
//
//	/* FFT buffer */
//	static complex_t butterfly[FFT_N];
//
//	/* Spectrum output buffer */
//	uint8_t spectrum[FFT_N/2];
//
//	fft_input( audio_buffer, butterfly );
//	fft_execute( butterfly );
//	fft_output( butterfly, spectrum );
//
//	uint8_t *spec = spectrum;
//
//	static uint16_t last_bars[16];
//	
//	hsb_t color = { .s = 1.0, .h = 0.3 };
//	rgb_t rgb;
//	for( uint8_t x = 0; x < LED_WIDTH; x++ )
//	{
//		for( uint8_t z = 0; z < LED_DEPTH; z++ )
//		{
//			unsigned long bar = *spec++;
//			bar += *spec++;
//
//#define BAR_EXP 7
//			bar = bar * 1.0/(double)BAR_EXP + last_bars[z*x] * (double)(BAR_EXP-1)/(double)BAR_EXP;
//			last_bars[z*x] = bar;
//
//#define BAR_FACTOR 35
//			for( uint8_t y = 0; y < LED_HEIGHT; y++ )
//			{
//				if( bar > BAR_FACTOR )
//				{
//					color.b = 0.5;
//					bar -= BAR_FACTOR;
//				}
//				else
//				{
//					color.b = ((double)bar / (double)BAR_FACTOR) * 0.5;
//					bar = 0;
//				}
//				//color.b = 0.5;
//				hsb_to_rgb( &color, &rgb );
//				if( z % 2 )
//					set_led( x, (3-y), z, &rgb );
//				else
//					set_led( (3-x), (3-y), z, &rgb );
//			}
//		}
//	}
//	tlc_gs_data_latch();
//	return;
//}
void audio_task(void)
{
	static double last_sum = 0;

	/* FFT buffer */
	static complex_t butterfly[FFT_N];

	/* Spectrum output buffer */
	uint8_t spectrum[FFT_N/2];

	fft_input( audio_buffer, butterfly );
	fft_execute( butterfly );
	fft_output( butterfly, spectrum );

#define ENVELOPE 4
	uint8_t *spec = spectrum + 1;

	int c = ENVELOPE;
	double sum = 0;
	while( c-- )
	{
		sum += *spec++;
	}

	static hsb_t color = { .s = 1.0, .h = 0.3 };
	rgb_t rgb;

#define ENVELOPE_MAX ((double)(ENVELOPE * 128))
#define ENVELOPE_THRESHOLD (ENVELOPE_MAX/10.0)
#define SUM_EXPONENT 6
	sum = sum * (1.0/(double)SUM_EXPONENT) + last_sum * (double)(SUM_EXPONENT-1)/(double)SUM_EXPONENT;

	static uint8_t last_tshld = 0;
	uint8_t tshld = 0;

	if( sum > ENVELOPE_THRESHOLD ) {
		tshld = 1;
	}
	else {
		tshld = 0;
	}

	if( tshld && !last_tshld )
	{
		color.h += 0.3;
		color.h += (double)rand() / (double)RAND_MAX * 0.5;
		if( color.h >= 1 )
			color.h -= 1;
	}

	last_tshld = tshld;

	color.b = ((double)sum / ENVELOPE_MAX) * 0.5;
	if( color.b > 0.5 )
		color.b = 0.5;

	last_sum = sum;

//	if( color.b < 0.01 )
//		color.b = 0;
	
	hsb_to_rgb( &color, &rgb );

	for( uint8_t x = 0; x < LED_WIDTH; x++ )
	{
		for( uint8_t y = 0; y < LED_HEIGHT; y++ )
		{
			for( uint8_t z = 0; z < LED_DEPTH; z++ )
			{
				set_led( x, y, z, &rgb );
			}
		}
	}
	tlc_gs_data_latch();
	return;
}

/*
double rms_audio_level(void)
{
	int16_t *abp = audio_buffer;
	uint8_t c = AUDIO_BUFFER_SIZE;
	double sum = 0;

	while( c-- ) 
	{
		sum += *abp * *abp;
		abp++;
	}
	sum /= (double)AUDIO_BUFFER_SIZE;

	double rms = sqrt( sum );

	return rms;
}
*/

#endif
