#include "io_ports.h"
#include "timer.h"
#include "speaker.h"

void play_sound(uint32_t nFrequency)
{
    uint32_t Div;
    uint8_t tmp;
    Div = 1193180 / nFrequency;
    out_bytes(0x43,0xb6);
    out_bytes(0x42,(uint8_t)(Div));
    out_bytes(0x42,(uint8_t)(Div >> 8));

    tmp = input_bytes(0x61);
    if(tmp != (tmp | 3))
    {
        out_bytes(0x61,tmp | 3);
    }

}

void nosound()
{
    uint8_t tmp = input_bytes(0x61) & 0xFC;
    out_bytes(0x61, tmp);
}

void beep()
{
    play_sound(1000);
    sleep(0.2);
    nosound();
}
void controlled_beep(uint32_t freq,double delay)
{
    play_sound(freq);
    sleep(delay);
    nosound();
}
void crude_song()
{
    int *song[18] = {100,200,100,200,100,200,100,200,500,1000,900,1000,900,950,800,120,50,1000};
    int song2[5] = {As5,C2,Cs2,Bb2,Fs3};
    for (uint16 i = 0; i < 18; i++)
    {
        controlled_beep(song[i],0.5);
    }
    
}