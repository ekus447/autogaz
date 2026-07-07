#pragma once
#include <stdint.h>

// Initializes background audio thread and loads PCM data from rawPath
// Returns true if audio started successfully.
bool initAudio(const char *rawPath, int32_t userId);

// Stops audio thread and closes audio handle.
void stopAudio();

// Start and pause background audio playback
void playAudio();
void pauseAudio();
