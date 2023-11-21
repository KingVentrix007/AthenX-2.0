#include "sb16.h"
#include "printf.h"
#include "kheap.h"
#include "sound.h"
#include "fat_filelib.h"
#include "string.h"
#include "wav.h"
bool sound_init = false;
const char* check_file_sound_type(const char* filename);
int play_beep(int duration)
{
    if(sound_init != true)
    {
        init_audio();
    }
    uint16_t sampleRate = 44100;
    uint8_t *beepBuffer = (uint8_t *)kmalloc(duration * sizeof(uint8_t));
    generateBeep(beepBuffer, duration);
    if(sb16_present() == true)
    {
        SB16_Write(0xD1);
        SB16_Play(beepBuffer, duration, 11025);
        kfree(beepBuffer);
        // printf("played sound\n");
        return 0;
    }
    else
    {
        printf("NO sound card present\n");
        return -1;
    }
}
void init_audio()
{
    SB16_Init();
    sound_init = true;
    
}
int play_sound_file(const char *path)
{
    if(strcmp(check_file_sound_type(path),"RIFF (WAV)") == 0)
    {
         FILE* file = fopen(path, "rb");
        uint8_t buffer = kmalloc(get_file_size(file)*sizeof(uint8_t));
        size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
        WAVFile wavFile = WAV_ParseFileData(buffer);
        SB16_Play(wavFile.data, wavFile.data_length, wavFile.header.sample_rate);
        kfree(buffer);
        fclose(file);

    }
    else
    {
        printf("File type %s not supported\n",check_file_sound_type(path));
    }
}

void generateBeep(uint8_t *buffer, uint32_t sampleSize) {
    double pi = 3.14159265358979323846;

    for (uint32_t i = 0; i < sampleSize; ++i) {
        // Generate a square wave with fade-in and fade-out
        double fade = sin((i / (double)sampleSize) * pi);
        buffer[i] = (uint8_t)(fade * (i < sampleSize / 2 ? 0xFF : 0x00));
    }
}

const char* check_file_sound_type(const char* filename) {
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        return "File not found";
    }

    // Read the first few bytes to identify the file type
    uint8_t buffer[4];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);

    fclose(file);

    if (bytesRead < sizeof(buffer)) {
        return "File too small";
    }

    // Check the file signature to determine the file type
    if (buffer[0] == 0x52 && buffer[1] == 0x49 && buffer[2] == 0x46 && buffer[3] == 0x46) {
        return "RIFF (WAV)";
    } else if (buffer[0] == 0xFF && buffer[1] == 0xFB) {
        return "MPEG-1 Layer 3 (MP3)";
    } else {
        return "Unknown";
    }
}