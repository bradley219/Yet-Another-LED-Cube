#ifndef _AUDIO_H_
#define _AUDIO_H_
#include "config.h"
#include "ports.h"

extern volatile int16_t audio_buffer[AUDIO_BUFFER_SIZE];
void analog_init(void);
double rms_audio_level(void);

#endif
