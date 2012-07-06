#ifndef _LEDCUBE_SOURCE_
#define _LEDCUBE_SOURCE_
#include "ledcube.h"

int main(void)
{
	analog_init();
	analog_srand();

	led_driver_init();
	tlc_set_all_gs(0);

	cubes_init();

	volatile unsigned long counter = 0;
	while(1) 
	{
		cubes_task();
		counter++;
	}

	return 0;
}

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
#endif
