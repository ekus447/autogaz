# settings
XBE_TITLE = autogaz
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.cpp \
	   $(CURDIR)/utils/input.cpp \
	   $(CURDIR)/utils/audio.c \
	   $(CURDIR)/utils/draw.c
NXDK_DIR ?= $(CURDIR)/../nxdk
NXDK_SDL = y

# make all
all: agm_wav.h makeAssetsDir

# compile_main
include $(NXDK_DIR)/Makefile

# music_agm
agm_wav.h: data/agm.wav
	ffmpeg -i $< -f s16le -acodec pcm_s16le $@.bin
	xxd --include $@.bin $@

# make bin/assets
makeAssetsDir:
	$(shell mkdir -p $(CURDIR)/bin/assets)

# asset_before
TARGET += $(OUTPUT_DIR)/assets/before.jpg
$(GEN_XISO): $(OUTPUT_DIR)/assets/before.jpg
$(OUTPUT_DIR)/assets/before.jpg: $(CURDIR)/data/before.jpg $(OUTPUT_DIR)/assets
	$(VE)cp '$<' '$@'

# asset_after
TARGET += $(OUTPUT_DIR)/assets/after.jpg
$(GEN_XISO): $(OUTPUT_DIR)/assets/after.jpg
$(OUTPUT_DIR)/assets/after.jpg: $(CURDIR)/data/after.jpg $(OUTPUT_DIR)/assets
	$(VE)cp '$<' '$@'

# asset_polat
TARGET += $(OUTPUT_DIR)/assets/polat.jpg
$(GEN_XISO): $(OUTPUT_DIR)/assets/polat.jpg
$(OUTPUT_DIR)/assets/polat.jpg: $(CURDIR)/data/polat.jpg $(OUTPUT_DIR)/assets
	$(VE)cp '$<' '$@'