MEDIA = /media/PANDORA/psp/game/CAT_Homebrews\ perso/
TARGET = autogaz

OBJS = main.o ./gLib2D/glib2d.o mp3player.o
LIBS = -ljpeg -lpng -lz -lpspgum -lpspgu -lpsprtc -lm -lpspvram -lmad -lpspaudiolib -lpspaudio -lpsppower

CFLAGS = -O2 -G0 -Wall -g
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = AutoGaz - PSP

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 
install:
	cp ./EBOOT.PBP $(MEDIA)$(TARGET)
dir:
	mkdir $(MEDIA)$(TARGET)
lclean:
	rm *.o
pack:
	mkdir RELEASE
	cp ./EBOOT.PBP RELEASE
	cp -r resources RELEASE
cleanpack:
	rm -r RELEASE