#ifndef _DRIVER_H_
#define _DRIVER_H_
#include "config.h"
#include "ports.h"

void led_driver_init(void);
void tlc_set_all_dc( uint8_t dc );
void tlc_gs_data_latch(void);
void set_led_raw( uint8_t x, uint8_t y, uint8_t z, uint16_t red, uint16_t green, uint16_t blue );
void set_led( uint8_t x, uint8_t y, uint8_t z, rgb_t *color );

void get_led( uint8_t x, uint8_t y, uint8_t z, rgb_t *color );
void set_led_coord( coord_t *coord, rgb_t *color );
void get_led_coord( coord_t *coord, rgb_t *color );
void tlc_set_all_gs( uint16_t gs );

#endif
