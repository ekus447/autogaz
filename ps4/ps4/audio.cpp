#include "audio.h"
#include "graphics.h"
#include <orbis/AudioOut.h>
#include <orbis/libkernel.h>

struct RawAudio {
    uint8_t *data;
    size_t   size;
};

static RawAudio g_audio = { nullptr, 0 };
static int32_t  g_audioHandle = -1;
static OrbisPthread g_audioThread;
static volatile bool g_audioRunning = false;
static volatile bool g_audioPlaying = false;
static size_t g_audioOffset = 0;

static bool loadRawAudio(const char *path, RawAudio &out)
{
    int fd = sceKernelOpen(path, 0x0000 /* O_RDONLY */, 0);
    if (fd < 0) return false;

    // Allocate 2 MB from direct display memory pool for our ~1.5 MB PCM file
    size_t max_size = 2 * 1024 * 1024;
    void *buf = allocDisplayMem(max_size);
    if (!buf) {
        sceKernelClose(fd);
        return false;
    }

    size_t total = 0;
    uint8_t *p = (uint8_t *)buf;
    while (total < max_size) {
        size_t chunk = max_size - total;
        if (chunk > 65536) chunk = 65536;
        int n = sceKernelRead(fd, p + total, chunk);
        if (n <= 0) break;
        total += (size_t)n;
    }

    sceKernelClose(fd);
    if (total == 0) return false;

    out.data = (uint8_t *)buf;
    out.size = total;
    return true;
}

static void *audioThreadFunc(void *arg)
{
    // 1024 samples * stereo (2 channels) * 16-bit (2 bytes) = 4096 bytes
    const size_t chunk_size = 1024 * 4;

    while (g_audioRunning) {
        if (g_audioPlaying && g_audio.data && g_audio.size >= chunk_size && g_audioHandle >= 0) {
            if (g_audioOffset + chunk_size > g_audio.size) {
                g_audioPlaying = false; // Wii'deki OGG_ONE_TIME gibi tek sefer çal ve dur
                g_audioOffset = 0;
                continue;
            }
            sceAudioOutOutput(g_audioHandle, g_audio.data + g_audioOffset);
            g_audioOffset += chunk_size;
        } else {
            sceKernelUsleep(16000);
        }
    }
    return nullptr;
}

bool initAudio(const char *rawPath, int32_t userId)
{
    if (!loadRawAudio(rawPath, g_audio)) {
        return false;
    }

    // Initialize audio system
    sceAudioOutInit();

    // Open main audio port: 1024 samples per channel, 48000 Hz, S16 Stereo
    // Pass 0xFF (ORBIS_USER_SERVICE_USER_ID_SYSTEM) to open system audio output (TV/HDMI)
    // without requiring a DualShock 4 headset connection (which causes error 0x20000007).
    int32_t targetId = 0xFF;
    g_audioHandle = sceAudioOutOpen(targetId, ORBIS_AUDIO_OUT_PORT_TYPE_MAIN, 0, 1024, 48000, ORBIS_AUDIO_OUT_PARAM_FORMAT_S16_STEREO);
    if (g_audioHandle < 0) {
        g_audioHandle = sceAudioOutOpen(userId, ORBIS_AUDIO_OUT_PORT_TYPE_MAIN, 0, 1024, 48000, ORBIS_AUDIO_OUT_PARAM_FORMAT_S16_STEREO);
        if (g_audioHandle < 0) {
            return false;
        }
    }

    g_audioPlaying = false;
    g_audioOffset  = 0;
    g_audioRunning = true;
    int rc = scePthreadCreate(&g_audioThread, nullptr, (void *)audioThreadFunc, nullptr, "audio_thread");
    if (rc < 0) {
        g_audioRunning = false;
        sceAudioOutClose(g_audioHandle);
        g_audioHandle = -1;
        return false;
    }

    return true;
}

void stopAudio()
{
    if (g_audioRunning) {
        g_audioRunning = false;
        sceKernelUsleep(50000); // Wait for thread to finish current chunk
        if (g_audioHandle >= 0) {
            sceAudioOutClose(g_audioHandle);
            g_audioHandle = -1;
        }
    }
}

void playAudio()
{
    g_audioOffset  = 0;
    g_audioPlaying = true;
}

void pauseAudio()
{
    g_audioPlaying = false;
}
