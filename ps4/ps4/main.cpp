#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdarg.h>

#include <orbis/UserService.h>
#include <orbis/Pad.h>
#include <orbis/libkernel.h>

#include "graphics.h"
#include "audio.h"

// ── Static overrides for ALL unresolved libc symbols ─────────────────────────
// Our libc.prx stub (required by the loader) is empty — GoldHEN does NOT patch
// it on FW 9.60. Override every symbol that the linker would otherwise pull from
// libc.prx at runtime, so no dynamic resolution is needed at all.
extern "C" {
    // ── crt1.o startup calls ──────────────────────────────────────────────────
    void _init_env() {}
    int  atexit(void (*)(void)) { return 0; }
    void exit(int) { while(1) {} }

    // ── File I/O ──────────────────────────────────────────────────────────────
    FILE *fopen(const char *, const char *) { return nullptr; }
    int   fclose(FILE *)                    { return 0; }
    int   vfprintf(FILE *, const char *, va_list) { return 0; }
    int   fflush(FILE *)                    { return 0; }

    // ── Heap (calloc for FrameBuffers pointer array and image RAM textures) ──
    static uint8_t s_heap[16 * 1024 * 1024];
    static size_t  s_heap_used = 0;

    void *calloc(size_t n, size_t sz) {
        size_t total = n * sz;
        if (s_heap_used + total > sizeof(s_heap)) return nullptr;
        void *p = s_heap + s_heap_used;
        s_heap_used += total;
        return p;
    }
    void free(void *) {}

    // ── String / memory builtins ──────────────────────────────────────────────
    void *memcpy(void *dst, const void *src, size_t n) {
        uint8_t *d = (uint8_t *)dst;
        const uint8_t *s = (const uint8_t *)src;
        while (n--) *d++ = *s++;
        return dst;
    }

    size_t strlen(const char *s) {
        size_t n = 0;
        while (*s++) n++;
        return n;
    }
}

// ────────────────────────────────────────────────────────────────────────────
// Screen layout constants  (1920×1080)
// ────────────────────────────────────────────────────────────────────────────
#define SCR_W   1920
#define SCR_H   1080

// 60 fps × 9 saniye = 540 frame
#define SURE_FRAMES   540

// ── Image paths (PS4 sandbox — /app0/ is the installed PKG root) ──────────────
#define IMG_BEFORE  "/app0/images/before.raw"
#define IMG_AFTER   "/app0/images/after.raw"
#define IMG_POLAT   "/app0/images/polat.raw"
#define IMG_MUSIC   "/app0/images/music.raw"

// ── Runtime image slots ──────────────────────────────────────────────────────
static RawImage g_before;
static RawImage g_after;
static RawImage g_polat;
static bool     g_imagesLoaded = false;

// ────────────────────────────────────────────────────────────────────────────
// State machine  (Wii versiyonundaki birebir aynı mantık)
//   0 → before.raw göster  (X'e bas)
//   1 → after.raw'a geçiş  (tek frame, timer başlat)
//   2 → after.raw göster   (timer bekle)
//   3 → polat.raw göster   (X ile sıfırla)
// ────────────────────────────────────────────────────────────────────────────
static int      g_state       = 0;
static uint64_t g_startTimeUs = 0;
static uint64_t g_frameCount  = 0;

// ── Renk sabitleri ───────────────────────────────────────────────────────────
static const Color C_WHITE    = { 255, 255, 255 };
static const Color C_YELLOW   = { 255, 220,  40 };
static const Color C_RED      = { 255,  60,  60 };

// ── Yarı-saydam koyu kutu çizer (%75 koyuluk / opacity) ──────────────────────
static void drawDarkBox(int x, int y, int w, int h)
{
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > SCR_W) w = SCR_W - x;
    if (y + h > SCR_H) h = SCR_H - y;
    for (int yp = y; yp < y + h; yp++) {
        uint32_t *row = &((uint32_t *)FrameBuffers[ActiveFrameBufferIdx])[yp * Width];
        for (int xp = x; xp < x + w; xp++) {
            uint32_t orig = row[xp];
            // %25 parlaklık (%75 koyu black box)
            uint8_t r = (uint8_t)(((orig >> 16) & 0xFF) * 25 / 100);
            uint8_t g = (uint8_t)(((orig >>  8) & 0xFF) * 25 / 100);
            uint8_t b = (uint8_t)(((orig >>  0) & 0xFF) * 25 / 100);
            row[xp] = 0x80000000u | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
        }
    }
}

// ── Sol alta büyük (scale=5) sade ipucu metni ve arkasına black box çizer ──
static void drawPromptLeft(const char *text)
{
    int len = 0; for (const char *p = text; *p; p++) len++;
    int padX = 16, padY = 10;
    int tx = 40, ty = SCR_H - 75;
    drawDarkBox(tx - padX, ty - padY, len * 30 + padX * 2, 35 + padY * 2);
    drawText(tx, ty, text, C_YELLOW, 5);
}

// ── Sol üste büyük (scale=5) sade ipucu metni ve arkasına black box çizer ──
static void drawPromptTopLeft(const char *text)
{
    int len = 0; for (const char *p = text; *p; p++) len++;
    int padX = 16, padY = 10;
    int tx = 40, ty = 30;
    drawDarkBox(tx - padX, ty - padY, len * 30 + padX * 2, 35 + padY * 2);
    drawText(tx, ty, text, C_YELLOW, 5);
}

// ── Hata ekranı — görsel yüklenemezse göster ─────────────────────────────────
static void drawError(const char *msg)
{
    Color bg = { 20, 0, 0 };
    frameBufferFill(bg);
    drawText(60, SCR_H / 2 - 30, "IMAGE LOAD ERROR:", C_RED, 4);
    drawText(60, SCR_H / 2 + 20, msg,                  C_WHITE, 3);
}

// ── drawScene — her frame çağrılır ───────────────────────────────────────────
static void drawScene()
{
    if (!g_imagesLoaded) {
        drawError("Gorseller yuklenemedi - /app0/images/*.raw");
        return;
    }

    uint64_t nowUs = sceKernelGetProcessTime();
    uint64_t elapsedUs = (nowUs > g_startTimeUs) ? (nowUs - g_startTimeUs) : 0;
    // Müziğin tam süresi + 0.3 saniye (kullanıcının belirttiği 1s uzatma ile beraber)
    const uint64_t TARGET_US = 8134666ULL;

    if (g_state == 2 && elapsedUs >= TARGET_US) {
        g_state = 3;
    }

    if (g_state == 0) {
        drawImageFullscreen(g_before.pixels, g_before.w, g_before.h);
        drawPromptLeft("[X] auto gazlan!");

    } else if (g_state == 2) {
        drawImageFullscreen(g_after.pixels, g_after.w, g_after.h);
        // Sayma ve alt bar kaldırıldı

    } else if (g_state == 3) {
        drawImageFullscreen(g_polat.pixels, g_polat.w, g_polat.h);
        drawPromptTopLeft("[0] devam...");
    }

    // Sağ üste @egemenkus0 ekle (beyaz, orta boy scale=3)
    // 11 karakter * 6 * 3 = 198px -> X = 1920 - 198 - 30 = 1692
    drawText(1692, 30, "@egemenkus0", C_WHITE, 3);
}

// Ayrı butonlar: state 0→2 = X, state 3→0 = O
#define BTN_START   PAD_BUTTON_X       // 0x4000  — ilk ekrandan başlatma
#define BTN_RESET   PAD_BUTTON_CIRCLE  // 0x2000  — polat ekranından geri dönüş

// ────────────────────────────────────────────────────────────────────────────
// Entry point
// ────────────────────────────────────────────────────────────────────────────
int main()
{
    int rc;
    int video;
    int curFrame = 0;

    // ── Video init ────────────────────────────────────────────────────────────
    video = sceVideoOutOpen(ORBIS_VIDEO_USER_MAIN, ORBIS_VIDEO_OUT_BUS_MAIN, 0, 0);
    if (video < 0) return video;

    rc = initializeFlipQueue(video);
    if (rc < 0) { sceVideoOutClose(video); return rc; }

    setDimensions(SCR_W, SCR_H, 4);

    // Framebuffer (~16MB) + 3 görsel (~3.6MB) + ses (~1.5MB) + marj
    // Toplam: 0x1E00000 = 30MB
    rc = allocateVideoMem(0x1E00000, 0x200000);
    if (rc < 0) { sceVideoOutClose(video); return rc; }

    rc = allocateFrameBuffers(video, 2);
    if (rc < 0) { deallocateVideoMem(); sceVideoOutClose(video); return rc; }

    setActiveFrameBuffer(0);
    sceVideoOutSetFlipRate(video, 0);

    // ── Runtime image loading ─────────────────────────────────────────────────
    // Görüntüler PKG içinde /app0/images/*.raw olarak kurulur.
    g_imagesLoaded = loadRawImage(IMG_BEFORE, g_before) &&
                     loadRawImage(IMG_AFTER,  g_after)  &&
                     loadRawImage(IMG_POLAT,  g_polat);

    // ── Pad init ─────────────────────────────────────────────────────────────
    OrbisUserServiceInitializeParams usp;
    usp.priority = ORBIS_KERNEL_PRIO_FIFO_LOWEST;
    sceUserServiceInitialize(&usp);

    OrbisUserServiceUserId userId = 1;
    sceUserServiceGetInitialUser(&userId);

    scePadInit();
    int padHandle = scePadOpen(userId, 0, 0, nullptr);
    if (padHandle < 0) {
        padHandle = scePadOpen(0, 0, 0, nullptr);
        if (padHandle < 0) padHandle = scePadOpen(1, 0, 0, nullptr);
    }

    // ── Audio init (Arka plan müziği) ────────────────────────────────────────
    initAudio(IMG_MUSIC, userId);

    // Debounce: son aksiyondan bu yana 300ms geçmeden yeni basışı yoksay.
    // Edge-detection (down = cur & ~prevButtons) güvenilmez çünkü scePadReadState
    // bazı frame'lerde stale/geçersiz veri döndürebiliyor.
    OrbisPadData padData;
    padData.buttons = 0;
    uint64_t g_lastActionUs = 0;
    const uint64_t DEBOUNCE_US = 300000; // 300ms

    // ── Draw loop ─────────────────────────────────────────────────────────────
    for (;;) {
        int frameID = curFrame ^ 1;
        g_frameCount++;

        if (padHandle >= 0) {
            // 1) Birikmiş eventlerden kısa tıklamaları yakala
            OrbisPadData evts[64];
            int n = scePadRead(padHandle, evts, 64);
            uint32_t anyBtn = 0;
            for (int i = 0; i < n; i++)
                anyBtn |= evts[i].buttons;

            // 2) Anlık durumu da ekle
            scePadReadState(padHandle, &padData);
            anyBtn |= padData.buttons;

            uint64_t nowUs = sceKernelGetProcessTime();
            bool cooldownOk = (nowUs - g_lastActionUs) > DEBOUNCE_US;

            if ((anyBtn & BTN_START) && g_state == 0 && cooldownOk) {
                g_startTimeUs  = nowUs;
                g_state        = 2;
                g_lastActionUs = nowUs;
                playAudio();
            }
            if ((anyBtn & BTN_RESET) && g_state == 3 && cooldownOk) {
                g_state        = 0;
                g_lastActionUs = nowUs;
                pauseAudio();
            }
        }

        drawScene();

        sceVideoOutSubmitFlip(video, ActiveFrameBufferIdx,
                              ORBIS_VIDEO_OUT_FLIP_VSYNC, curFrame);
        frameWait(video, curFrame);
        frameBufferSwap();
        curFrame = frameID;
    }

    return 0;
}
