
#ifndef IMAGES_H
#define IMAGES_H

#include <stdint.h>

extern const uint8_t error_skull[];
extern const uint8_t athenx_logo[];
extern const uint8_t background[];
extern const uint8_t test[];
struct Font {
    char letter;
    char code[7][5];
};

extern struct Font font[];
extern const unsigned char fontdata[];

#endif // GRAPHICS_H