#include "io_ports.h"
#include "timer.h"
#include "speaker.h"

void play_sound(uint32_t nFrequency)
{
    // This function plays a sound at the specified frequency.
    // It uses the PC speaker to generate the sound.
    // The nFrequency parameter represents the frequency of the sound to be played.
    // It calculates the appropriate divisor value (Div) based on the frequency.
    // Then, it writes the appropriate values to the timer registers (0x43 and 0x42)
    // to generate the desired sound.
    // Finally, it enables the speaker output to produce the sound.
    uint32_t Div;
    uint8_t tmp;
    Div = 1193180 / nFrequency;
    out_bytes(0x43, 0xb6);  // Set the command byte for the timer (channel 2)
    out_bytes(0x42, (uint8_t)(Div));  // Set the low byte of the divisor
    out_bytes(0x42, (uint8_t)(Div >> 8));  // Set the high byte of the divisor

    tmp = input_bytes(0x61);
    if (tmp != (tmp | 3))
    {
        out_bytes(0x61, tmp | 3);  // Enable the speaker output (bits 0 and 1 set to 1)
    }
}

void nosound()
{
    // This function turns off the PC speaker, effectively stopping any sound that was being played.
    // It clears the bits 0 and 1 of the control register at port 0x61, which disables the speaker output.
    uint8_t tmp = input_bytes(0x61) & 0xFC;
    out_bytes(0x61, tmp);
}

void beep()
{
    // This function plays a short beep sound at a frequency of 1000 Hz for 0.2 seconds.
    // It does so by calling the `play_sound()` function with the specified frequency,
    // then pausing for 0.2 seconds using the `sleep()` function, and finally turning off the speaker using `nosound()`.
    play_sound(1000);
    sleep(1);
    nosound();
}

void controlled_beep(uint32_t freq, double delay)
{
    // This function plays a controlled beep sound at the specified frequency and duration.
    // It takes two parameters: `freq` (the frequency of the sound to be played) and `delay` (the duration in seconds).
    // It plays the sound using the `play_sound()` function, then pauses for the specified `delay` using `sleep()`,
    // and finally stops the sound by calling `nosound()`.
    play_sound(freq);
    sleep(delay);
    nosound();
}

void crude_song()
{
    // This function plays a crude melody by calling `controlled_beep()` for each note in the song.
    // The song is defined as an array of integers representing the frequencies of the notes to be played.
    // It uses the `controlled_beep()` function to play each note for a fixed duration of 0.5 seconds.
    int song[18] = {100, 200, 100, 200, 100, 200, 100, 200, 500, 1000, 900, 1000, 900, 950, 800, 120, 50, 1000};
    for (uint16 i = 0; i < 18; i++)
    {
        controlled_beep(song[i], 0.5);
    }
}
