#ifndef _LEDCUBE_SOURCE_
#define _LEDCUBE_SOURCE_
#include "ledcube.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

hsb_t main_color = { .h = 0, .s = 1, .b = 0.5 };
uint8_t is_beat = 0;


int main(void)
{
	audio_init();

	eeprom_srand();

	tlc_set_all_gs(0);
	tlc_gs_data_latch();
	
	led_driver_init();

	//cubes_init();
	//fader_init();
	
	uint16_t g = 0;//PWM_MAX_VAL * 7 / 8;
	char inc = 1;
	snake_init();
	while(1) 
	{
		//rando();
		//solid_fader_task();
		//fader_task();
		//led_test5();
		//cubes_task();
		//audio_task();
		snake_task();
	}
	panels_init();
	while(1) 
	{
		panels_task();
	}
	return 0;
}

static unsigned long EEMEM analog_seed = 0;
void eeprom_srand(void)
{
	volatile unsigned long seed;
	eeprom_read_block( &seed, &analog_seed, sizeof(analog_seed) );
	srand(seed);
	seed++;
	eeprom_write_block( &seed, &analog_seed, sizeof(analog_seed) );
	return;
}
#endif
