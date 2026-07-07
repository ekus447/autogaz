TOOLCHAIN    := $(OO_PS4_TOOLCHAIN)
PROJDIR      := ps4autogaz
INTDIR       := $(PROJDIR)/x64/Debug

# ── Linked libraries ──────────────────────────────────────────────────────────
LIBS := -lc -lkernel -lSceVideoOut -lSceUserService -lScePad -lSceAudioOut

# ── Detect macOS / Linux ──────────────────────────────────────────────────────
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CC   := clang++
    LD   := ld.lld
    CDIR := linux
endif
ifeq ($(UNAME_S),Darwin)
    ifneq ($(wildcard /opt/homebrew/opt/llvm/bin/clang++),)
        CC   := /opt/homebrew/opt/llvm/bin/clang++
    else
        CC   := /usr/local/opt/llvm/bin/clang++
    endif
    ifneq ($(wildcard /opt/homebrew/opt/llvm/bin/ld.lld),)
        LD   := /opt/homebrew/opt/llvm/bin/ld.lld
    else
        LD   := /usr/local/opt/llvm/bin/ld.lld
    endif
    CDIR := macos
endif

# ── Directories ───────────────────────────────────────────────────────────────
ODIR  := $(INTDIR)
SDIR  := .
IDIRS := -I$(TOOLCHAIN)/include
LDIRS := -L$(TOOLCHAIN)/lib

# ── Flags ─────────────────────────────────────────────────────────────────────
CFLAGS  := -cc1 -triple x86_64-scei-ps4-elf $(IDIRS) -emit-obj -O2
LFLAGS  := -m elf_x86_64 -pie --script $(TOOLCHAIN)/link.x --eh-frame-hdr \
            $(LDIRS) $(LIBS) $(TOOLCHAIN)/lib/crt1.o

# ── Sources ───────────────────────────────────────────────────────────────────
CPPFILES := $(wildcard $(SDIR)/*.cpp)
OBJS     := $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(CPPFILES))

_unused := $(shell mkdir -p $(INTDIR))

# ── Hedefler ─────────────────────────────────────────────────────────────────
.PHONY: all pkg deploy send clean images

all: images $(PROJDIR)/x64/Debug/$(PROJDIR).elf

# ── PNG → RAW binary dönüşümü ────────────────────────────────────────────────
# Görüntüler ELF'e gömülmez; PKG içinde /app0/images/*.raw olarak gider.
images:
	@echo "  [PNG2RAW/OGG2RAW] Converting images and audio to raw binary..."
	@mkdir -p images
	@python3 tools/png2raw.py ../wii/data/before.png images/before.raw 640 480
	@python3 tools/png2raw.py ../wii/data/after.png  images/after.raw  640 480
	@python3 tools/png2raw.py ../wii/data/polat.png  images/polat.raw  640 480
	@python3 tools/ogg2raw.py ../wii/data/music.ogg  images/music.raw

$(PROJDIR)/x64/Debug/$(PROJDIR).elf: $(OBJS)
	$(LD) $(ODIR)/*.o -o $(ODIR)/$(PROJDIR).elf $(LFLAGS)
	$(TOOLCHAIN)/bin/$(CDIR)/create-eboot \
	    -in=$(ODIR)/$(PROJDIR).elf \
	    -out=$(ODIR)/$(PROJDIR).oelf \
	    --paid 0x3800000000000011
	@echo ""
	@echo "✅  Build tamam!"
	@echo "    ELF  : $(ODIR)/$(PROJDIR).elf"
	@echo "    OELF : $(ODIR)/$(PROJDIR).oelf"
	@echo "    EBOOT: eboot.bin"
	@echo ""

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

# ── PKG üret ─────────────────────────────────────────────────────────────────
pkg: all
	python3 tools/make_pkg.py

# ── FTP deploy ────────────────────────────────────────────────────────────────
deploy: all
	@if [ -z "$(PS4_IP)" ]; then echo "Kullanim: make deploy PS4_IP=192.168.1.XXX"; exit 1; fi
	python3 tools/deploy.py $(PS4_IP)

# ── ELF gönder (GoldHEN port 9090) ───────────────────────────────────────────
send: all
	@if [ -z "$(PS4_IP)" ]; then echo "Kullanim: make send PS4_IP=192.168.1.XXX"; exit 1; fi
	python3 tools/send_elf.py $(PS4_IP)

# GoldHEN klog loglarını dinle
log:
	@if [ -z "$(PS4_IP)" ]; then echo "Kullanim: make log PS4_IP=192.168.1.XXX"; exit 1; fi
	python3 tools/capture_logs.py $(PS4_IP)

# ── Temizle ──────────────────────────────────────────────────────────────────
clean:
	rm -rf $(INTDIR) *.pkg pkg_project/eboot.bin pkg_project/ps4autogaz.gp4 pkg_project/sce_sys/icon0.png pkg_project/sce_sys/param.sfo eboot.bin images/ pkg_project/images/
