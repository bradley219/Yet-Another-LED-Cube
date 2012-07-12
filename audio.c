#ifndef _AUDIO_SOURCE_
#define _AUDIO_SOURCE_

#include "ports.h"
#include <string.h>
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

/* FFT buffer */
static complex_t butterfly[FFT_N];

/* Spectrum output buffer */
uint8_t spectrum[FFT_N/2];

void audio_init(void)
{
	return;
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
void audio_task(void)
{
	return;

//	fft_input( audio_buffer, butterfly );
//	fft_execute( butterfly );
//	fft_output( butterfly, spectrum );

	uint8_t *spec = spectrum;

	hsb_t color = { .s = 1.0, .h = 0.3 };
	rgb_t rgb;
	for( uint8_t x = 0; x < LED_WIDTH; x++ )
	{
		for( uint8_t y = 0; y < LED_HEIGHT; y++ )
		{
			for( uint8_t z = 0; z < LED_DEPTH; z++ )
			{
				color.b = (double)*spec / (double)0xff;
				color.b = 0.5;

				hsb_to_rgb( &color, &rgb );
				set_led( x, y, z, &rgb );

				tlc_gs_data_latch();
				_delay_ms(100);

				spec++;
			}
		}
	}
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
