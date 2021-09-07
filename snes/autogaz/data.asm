.include "hdr.asm"

.section ".rodata1" superfree

patterns:
.incbin "autogaz_map.pic"
patterns_end:

.ends

.section ".rodata2" superfree
map:
.incbin "autogaz_map.map"
map_end:

palette:
.incbin "autogaz_map.pal"

.ends

.section ".rodata3" superfree

soundbrr:
.incbin "tada.brr"
soundbrrend:

snesfont:
.incbin "pvsneslibfont.pic"

.ends
