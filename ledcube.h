#ifndef _LEDCUBE_H_
#define _LEDCUBE_H_

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h> 
#include "types.h"
#include "config.h"
#include "ports.h"
#include "driver.h"
#include "tests.h"
#include "color.h"
#include "random_bugs.h"
#include "cubes.h"
#include "panels.h"
#include "fader.h"
#include "snake.h"
//#include "audio.h"

extern uint8_t is_beat;
extern hsb_t main_color;
void eeprom_srand(void);

#endif // ifndef _SHARK_H_

