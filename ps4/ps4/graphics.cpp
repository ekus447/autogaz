#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "graphics.h"

// ── Globals ───────────────────────────────────────────────────────────────────
void        *VideoMem       = NULL;
void       **FrameBuffers   = NULL;
uintptr_t    VideoMemSP     = 0;
OrbisKernelEqueue             FlipQueue;
OrbisVideoOutBufferAttribute  Attr;
off_t   DirectMemOff              = 0;
size_t  DirectMemAllocationSize   = 0;

int Width  = 1920;
int Height = 1080;
int Depth  = 4;
int ActiveFrameBufferIdx = 0;
int FrameBufferSize      = 0;
int FrameBufferCount     = 0;

// ── Tiny 5×7 bitmap font (ASCII 32-126) ─────────────────────────────────────
// Each character is 5 bytes, each byte is one column of 7 bits (bit0=top).
static const uint8_t FONT5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // ' '
    {0x00,0x00,0x5F,0x00,0x00}, // '!'
    {0x00,0x07,0x00,0x07,0x00}, // '"'
    {0x14,0x7F,0x14,0x7F,0x14}, // '#'
    {0x24,0x2A,0x7F,0x2A,0x12}, // '$'
    {0x23,0x13,0x08,0x64,0x62}, // '%'
    {0x36,0x49,0x55,0x22,0x50}, // '&'
    {0x00,0x05,0x03,0x00,0x00}, // '''
    {0x00,0x1C,0x22,0x41,0x00}, // '('
    {0x00,0x41,0x22,0x1C,0x00}, // ')'
    {0x08,0x2A,0x1C,0x2A,0x08}, // '*'
    {0x08,0x08,0x3E,0x08,0x08}, // '+'
    {0x00,0x50,0x30,0x00,0x00}, // ','
    {0x08,0x08,0x08,0x08,0x08}, // '-'
    {0x00,0x60,0x60,0x00,0x00}, // '.'
    {0x20,0x10,0x08,0x04,0x02}, // '/'
    {0x3E,0x51,0x49,0x45,0x3E}, // '0'
    {0x00,0x42,0x7F,0x40,0x00}, // '1'
    {0x42,0x61,0x51,0x49,0x46}, // '2'
    {0x21,0x41,0x45,0x4B,0x31}, // '3'
    {0x18,0x14,0x12,0x7F,0x10}, // '4'
    {0x27,0x45,0x45,0x45,0x39}, // '5'
    {0x3C,0x4A,0x49,0x49,0x30}, // '6'
    {0x01,0x71,0x09,0x05,0x03}, // '7'
    {0x36,0x49,0x49,0x49,0x36}, // '8'
    {0x06,0x49,0x49,0x29,0x1E}, // '9'
    {0x00,0x36,0x36,0x00,0x00}, // ':'
    {0x00,0x56,0x36,0x00,0x00}, // ';'
    {0x00,0x08,0x14,0x22,0x41}, // '<'
    {0x14,0x14,0x14,0x14,0x14}, // '='
    {0x41,0x22,0x14,0x08,0x00}, // '>'
    {0x02,0x01,0x51,0x09,0x06}, // '?'
    {0x32,0x49,0x79,0x41,0x3E}, // '@'
    {0x7E,0x11,0x11,0x11,0x7E}, // 'A'
    {0x7F,0x49,0x49,0x49,0x36}, // 'B'
    {0x3E,0x41,0x41,0x41,0x22}, // 'C'
    {0x7F,0x41,0x41,0x22,0x1C}, // 'D'
    {0x7F,0x49,0x49,0x49,0x41}, // 'E'
    {0x7F,0x09,0x09,0x09,0x01}, // 'F'
    {0x3E,0x41,0x49,0x49,0x7A}, // 'G'
    {0x7F,0x08,0x08,0x08,0x7F}, // 'H'
    {0x00,0x41,0x7F,0x41,0x00}, // 'I'
    {0x20,0x40,0x41,0x3F,0x01}, // 'J'
    {0x7F,0x08,0x14,0x22,0x41}, // 'K'
    {0x7F,0x40,0x40,0x40,0x40}, // 'L'
    {0x7F,0x02,0x04,0x02,0x7F}, // 'M'
    {0x7F,0x04,0x08,0x10,0x7F}, // 'N'
    {0x3E,0x41,0x41,0x41,0x3E}, // 'O'
    {0x7F,0x09,0x09,0x09,0x06}, // 'P'
    {0x3E,0x41,0x51,0x21,0x5E}, // 'Q'
    {0x7F,0x09,0x19,0x29,0x46}, // 'R'
    {0x46,0x49,0x49,0x49,0x31}, // 'S'
    {0x01,0x01,0x7F,0x01,0x01}, // 'T'
    {0x3F,0x40,0x40,0x40,0x3F}, // 'U'
    {0x1F,0x20,0x40,0x20,0x1F}, // 'V'
    {0x3F,0x40,0x38,0x40,0x3F}, // 'W'
    {0x63,0x14,0x08,0x14,0x63}, // 'X'
    {0x03,0x04,0x78,0x04,0x03}, // 'Y'
    {0x61,0x51,0x49,0x45,0x43}, // 'Z'
    {0x00,0x00,0x7F,0x41,0x41}, // '['
    {0x02,0x04,0x08,0x10,0x20}, // '\'
    {0x41,0x41,0x7F,0x00,0x00}, // ']'
    {0x04,0x02,0x01,0x02,0x04}, // '^'
    {0x40,0x40,0x40,0x40,0x40}, // '_'
    {0x00,0x01,0x02,0x04,0x00}, // '`'
    {0x20,0x54,0x54,0x54,0x78}, // 'a'
    {0x7F,0x48,0x44,0x44,0x38}, // 'b'
    {0x38,0x44,0x44,0x44,0x20}, // 'c'
    {0x38,0x44,0x44,0x48,0x7F}, // 'd'
    {0x38,0x54,0x54,0x54,0x18}, // 'e'
    {0x08,0x7E,0x09,0x01,0x02}, // 'f'
    {0x08,0x14,0x54,0x54,0x3C}, // 'g'
    {0x7F,0x08,0x04,0x04,0x78}, // 'h'
    {0x00,0x44,0x7D,0x40,0x00}, // 'i'
    {0x20,0x40,0x44,0x3D,0x00}, // 'j'
    {0x00,0x7F,0x10,0x28,0x44}, // 'k'
    {0x00,0x41,0x7F,0x40,0x00}, // 'l'
    {0x7C,0x04,0x18,0x04,0x78}, // 'm'
    {0x7C,0x08,0x04,0x04,0x78}, // 'n'
    {0x38,0x44,0x44,0x44,0x38}, // 'o'
    {0x7C,0x14,0x14,0x14,0x08}, // 'p'
    {0x08,0x14,0x14,0x18,0x7C}, // 'q'
    {0x7C,0x08,0x04,0x04,0x08}, // 'r'
    {0x48,0x54,0x54,0x54,0x20}, // 's'
    {0x04,0x3F,0x44,0x40,0x20}, // 't'
    {0x3C,0x40,0x40,0x20,0x7C}, // 'u'
    {0x1C,0x20,0x40,0x20,0x1C}, // 'v'
    {0x3C,0x40,0x30,0x40,0x3C}, // 'w'
    {0x44,0x28,0x10,0x28,0x44}, // 'x'
    {0x0C,0x50,0x50,0x50,0x3C}, // 'y'
    {0x44,0x64,0x54,0x4C,0x44}, // 'z'
    {0x00,0x08,0x36,0x41,0x00}, // '{'
    {0x00,0x00,0x7F,0x00,0x00}, // '|'
    {0x00,0x41,0x36,0x08,0x00}, // '}'
    {0x08,0x08,0x2A,0x1C,0x08}, // '~'
};

// ── Internal ─────────────────────────────────────────────────────────────────
void setDimensions(int width, int height, int depth)
{
    Width  = width;
    Height = height;
    Depth  = depth;
    FrameBufferSize = Width * Height * Depth;
}

void setActiveFrameBuffer(int idx) { ActiveFrameBufferIdx = idx; }

int initializeFlipQueue(int video)
{
    int rc = sceKernelCreateEqueue(&FlipQueue, "ps4autogaz flip queue");
    if (rc < 0) return rc;
    sceVideoOutAddFlipEvent(FlipQueue, video, 0);
    return rc;
}

int allocateVideoMem(size_t size, int alignment)
{
    int rc;
    DirectMemAllocationSize = (size + alignment - 1) / alignment * alignment;
    rc = sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(),
                                       DirectMemAllocationSize, alignment, 3,
                                       &DirectMemOff);
    if (rc < 0) { DirectMemAllocationSize = 0; return rc; }
    rc = sceKernelMapDirectMemory(&VideoMem, DirectMemAllocationSize,
                                  0x33, 0, DirectMemOff, alignment);
    if (rc < 0) {
        sceKernelReleaseDirectMemory(DirectMemOff, DirectMemAllocationSize);
        DirectMemOff = 0; DirectMemAllocationSize = 0;
        return rc;
    }
    VideoMemSP = (uintptr_t)VideoMem;
    return rc;
}

void deallocateVideoMem()
{
    sceKernelReleaseDirectMemory(DirectMemOff, DirectMemAllocationSize);
    VideoMem = NULL; VideoMemSP = 0;
    DirectMemOff = 0; DirectMemAllocationSize = 0;
    free(FrameBuffers); FrameBuffers = NULL;
}

int allocateFrameBuffers(int video, int num)
{
    FrameBuffers = (void **)calloc(num, sizeof(void *));
    for (int i = 0; i < num; i++)
        FrameBuffers[i] = allocDisplayMem(FrameBufferSize);
    sceVideoOutSetBufferAttribute(&Attr, 0x80000000, 1, 0, Width, Height, Width);
    FrameBufferCount = num;
    return sceVideoOutRegisterBuffers(video, 0, FrameBuffers, num, &Attr);
}

void *allocDisplayMem(size_t sz)
{
    void *p = (void *)VideoMemSP;
    VideoMemSP += sz;
    return p;
}

void frameBufferSwap()
{
    ActiveFrameBufferIdx = (ActiveFrameBufferIdx + 1) % FrameBufferCount;
}

void frameBufferClear()
{
    Color black = { 0, 0, 0 };
    frameBufferFill(black);
}

void frameBufferFill(Color color)
{
    drawRectangle(0, 0, Width, Height, color);
}

void frameWait(int video, int frameID)
{
    OrbisKernelEvent evt;
    int count;
    if (!video) return;
    for (;;) {
        OrbisVideoOutFlipStatus fs;
        sceVideoOutGetFlipStatus(video, &fs);
        if (fs.flipArg == frameID) break;
        if (sceKernelWaitEqueue(FlipQueue, &evt, 1, &count, 0) != 0) break;
    }
}

// ── Pixel / rectangle ────────────────────────────────────────────────────────
void drawPixel(int x, int y, Color color)
{
    if (x < 0 || x >= Width || y < 0 || y >= Height) return;
    int pixel = y * Width + x;
    uint32_t enc = 0x80000000u | ((uint32_t)color.r << 16) |
                                 ((uint32_t)color.g <<  8) |
                                  (uint32_t)color.b;
    ((uint32_t *)FrameBuffers[ActiveFrameBufferIdx])[pixel] = enc;
}

void drawRectangle(int x, int y, int width, int height, Color color)
{
    for (int yp = y; yp < y + height; yp++)
        for (int xp = x; xp < x + width; xp++)
            drawPixel(xp, yp, color);
}

// Bresenham line
void drawLine(int x0, int y0, int x1, int y1, Color color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// 5×7 bitmap text, with integer scale factor
void drawText(int x, int y, const char *str, Color fg, int scale)
{
    int cx = x;
    for (; *str; str++) {
        unsigned char ch = (unsigned char)*str;
        if (ch < 32 || ch > 126) { cx += 6 * scale; continue; }
        const uint8_t *col = FONT5x7[ch - 32];
        for (int c = 0; c < 5; c++) {
            uint8_t bits = col[c];
            for (int r = 0; r < 7; r++) {
                if (bits & (1 << r)) {
                    drawRectangle(cx + c * scale, y + r * scale, scale, scale, fg);
                }
            }
        }
        cx += 6 * scale;
    }
}

// ── Full-screen image blit (nearest-neighbor scale) ──────────────────────────
// pixels: BGRA uint32 array (little-endian: B=bits[7:0], G=bits[15:8], R=bits[23:16], A=bits[31:24])
void drawImageFullscreen(const uint32_t *pixels, int imgW, int imgH)
{
    uint32_t *fb = (uint32_t *)FrameBuffers[ActiveFrameBufferIdx];
    for (int sy = 0; sy < Height; sy++) {
        const uint32_t *srcRow = &pixels[(sy * imgH / Height) * imgW];
        for (int sx = 0; sx < Width; sx++) {
            uint32_t px = srcRow[sx * imgW / Width];
            *fb++ = (px & 0x00FFFFFFu) | 0x80000000u;
        }
    }
}

// ── Runtime image loading ─────────────────────────────────────────────────────
// File format written by tools/png2raw.py:
//   [4 bytes] uint32 little-endian W
//   [4 bytes] uint32 little-endian H
//   [W*H * 4 bytes] BGRA uint32 pixels, little-endian
//
// Returns true on success.
bool loadRawImage(const char *path, RawImage &out)
{
    int fd = sceKernelOpen(path, 0x0000 /* O_RDONLY */, 0);
    if (fd < 0) return false;

    uint32_t hdr[2];
    if (sceKernelRead(fd, hdr, 8) != 8) {
        sceKernelClose(fd);
        return false;
    }

    uint32_t W = hdr[0];
    uint32_t H = hdr[1];
    size_t   nbytes = (size_t)W * H * 4;

    void *buf = calloc(1, nbytes);
    if (!buf) {
        sceKernelClose(fd);
        return false;
    }

    size_t total = 0;
    uint8_t *p   = (uint8_t *)buf;
    while (total < nbytes) {
        size_t chunk = nbytes - total;
        if (chunk > 65536) chunk = 65536;
        int n = sceKernelRead(fd, p + total, chunk);
        if (n <= 0) break;
        total += (size_t)n;
    }

    sceKernelClose(fd);

    if (total != nbytes) return false;

    out.pixels = (uint32_t *)buf;
    out.w      = (int)W;
    out.h      = (int)H;
    return true;
}
