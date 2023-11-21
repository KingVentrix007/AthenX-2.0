#ifndef __SOUND_HAL__H
#define __SOUND_HAL__H
#include "stdint.h"
int play_beep(int duration);
int play_sound_file(const char *path);
void generateBeep(uint8_t *buffer, uint32_t sampleSize);
void init_audio();
#endif