#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#include <string.h>
#include <hal/audio.h>
#include <xboxkrnl/xboxkrnl.h>
#include <assert.h>

#define MIN(x,y) ((x)<(y)?(x):(y))

#include "../agm_wav.h"

volatile unsigned int callback_count;

unsigned char *voice_data;
unsigned int voice_len;
unsigned int voice_pos;

#define NUM_BUFFERS 27
const unsigned short buffer_size = 48*1024;
unsigned char *buffers[NUM_BUFFERS];
unsigned int current_buf;

#define MAXRAM 0x03FFAFFF

static void provide_samples_callback(void *pac97Device, void *data)
{
    int is_final = (voice_pos+buffer_size) >= voice_len;
    int chunk_size = MIN(voice_len-voice_pos, buffer_size);

    memcpy(buffers[current_buf], voice_data+voice_pos, chunk_size);
    XAudioProvideSamples(buffers[current_buf], chunk_size, is_final);

    if (is_final) {
        voice_pos = 0;
    } else {
        voice_pos = voice_pos+chunk_size;
    }

    current_buf = (current_buf+1) % NUM_BUFFERS;
    callback_count++;
}

int initAGM(void)
{

    for (int i = 0; i < NUM_BUFFERS; i++) {
        buffers[i] = MmAllocateContiguousMemoryEx(buffer_size, 0, MAXRAM, 0,
            (PAGE_READWRITE | PAGE_WRITECOMBINE));
        assert(buffers[i] != NULL);
    }

    voice_data = agm_wav_h_bin;
    voice_len = agm_wav_h_bin_len;
    voice_pos = 0;
    current_buf = 0;
    callback_count = 0;

    XAudioInit(16, 2, &provide_samples_callback, NULL);

    for (int i = 0; i < NUM_BUFFERS; i++) {
        provide_samples_callback(NULL, NULL);
    }

    return 0;
}

int playAGM() {
    XAudioPlay();
    return 0;
}
