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
#include "ledcube.h"

volatile int16_t audio_buffer[AUDIO_BUFFER_SIZE];
volatile int16_t *abp = audio_buffer;
volatile uint8_t abc = AUDIO_BUFFER_SIZE;

// 4099390717
void audio_init(void)
{
	// Setup low-pass filter
	DDRD |= _BV(PORTD5);
	OCR0A = 20;
	OCR0B = OCR0A / 2;
	TCCR0A = _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(WGM02) | TIMER0_PS_BITS;

	// Setup ADC capture
	ADMUX = _BV(REFS0);
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADATE);
	ADCSRB = 0;

	// Start ADC capture
	sei();
	ADCSRA |= _BV(ADSC);
	return;
}
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

	if( sum < 50 )
		sum = 0;

	rgb_t rgb;

#define ENVELOPE_MAX ((double)(ENVELOPE * 128))
#define ENVELOPE_THRESHOLD 0.10
#define SUM_EXPONENT 6
	sum = sum * (1.0/(double)SUM_EXPONENT) + last_sum * (double)(SUM_EXPONENT-1)/(double)SUM_EXPONENT;

	static long high_count = 0;
	static long low_count = 0;
	long last_low_count = -1,last_high_count = -1;

	uint8_t beat = 0;

	double relative_level = sum / ENVELOPE_MAX;
	if( relative_level > ENVELOPE_THRESHOLD )
	{
		if( high_count == 0 )
		{
			last_low_count = low_count;
			low_count = 0;
		}
		high_count++;
	}
	else
	{
		if( low_count == 0 )
		{
			last_high_count = high_count;

			if( last_high_count > (0.5 * (double)last_low_count) )
			{
				beat = 1;
			}

			high_count = 0;
		}
		low_count++;
	}

	if( beat )
	{
		is_beat = 1;
		main_color.h += 0.3;
		main_color.h += (double)rand() / (double)RAND_MAX * 0.5;
		if( main_color.h >= 1 )
			main_color.h -= 1;
	}

//	main_color.b = ((double)sum / ENVELOPE_MAX) * 0.5;
//	if( main_color.b > 0.5 )
//		main_color.b = 0.5;

	last_sum = sum;

//	hsb_to_rgb( &color, &rgb );
//
//	for( uint8_t x = 0; x < LED_WIDTH; x++ )
//	{
//		for( uint8_t y = 0; y < LED_HEIGHT; y++ )
//		{
//			for( uint8_t z = 0; z < LED_DEPTH; z++ )
//			{
//				set_led( x, y, z, &rgb );
//			}
//		}
//	}
//	tlc_gs_data_latch();
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
