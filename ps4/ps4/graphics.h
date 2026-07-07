#pragma once
#include <stdint.h>
#include <orbis/libkernel.h>
#include <orbis/VideoOut.h>

// ── Color ────────────────────────────────────────────────────────────────────
typedef struct {
    uint8_t r, g, b;
} Color;

// ── Globals ──────────────────────────────────────────────────────────────────
extern void        *VideoMem;
extern void       **FrameBuffers;
extern uintptr_t    VideoMemSP;
extern OrbisKernelEqueue            FlipQueue;
extern OrbisVideoOutBufferAttribute Attr;
extern off_t   DirectMemOff;
extern size_t  DirectMemAllocationSize;
extern int Width, Height, Depth;
extern int FrameBufferSize, FrameBufferCount;
extern int ActiveFrameBufferIdx;

// ── API ───────────────────────────────────────────────────────────────────────
void setDimensions(int width, int height, int depth);
void setActiveFrameBuffer(int idx);
int  initializeFlipQueue(int video);
int  allocateVideoMem(size_t size, int alignment);
void deallocateVideoMem();
int  allocateFrameBuffers(int video, int num);
void *allocDisplayMem(size_t sz);

void frameWait(int video, int frameID);
void frameBufferSwap();
void frameBufferClear();
void frameBufferFill(Color color);

void drawPixel(int x, int y, Color color);
void drawRectangle(int x, int y, int width, int height, Color color);
void drawLine(int x0, int y0, int x1, int y1, Color color);
void drawText(int x, int y, const char *str, Color fg, int scale);

// ── Runtime image loading ─────────────────────────────────────────────────────
// Simple image container for runtime-loaded BGRA data.
struct RawImage {
    uint32_t *pixels;  // heap-allocated BGRA pixel array
    int       w, h;
};

// Loads a .raw file (format: uint32 W, uint32 H, W*H uint32 BGRA pixels).
// Returns true on success. path = absolute path like "/app0/images/before.raw"
bool loadRawImage(const char *path, RawImage &out);

// ── Image blit ───────────────────────────────────────────────────────────────
// Draws a pre-decoded BGRA image onto the framebuffer, scaled to fill the screen.
void drawImageFullscreen(const uint32_t *pixels, int imgW, int imgH);
