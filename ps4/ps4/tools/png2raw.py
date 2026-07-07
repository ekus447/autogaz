#!/usr/bin/env python3
"""
tools/png2raw.py  —  Converts PNG files to raw BGRA binary files.
These files are included in the PKG as /app0/images/*.raw and loaded at runtime.
This keeps the ELF binary small (~70KB) so create-eboot can produce a valid SELF.

Usage:
    python3 tools/png2raw.py <input.png> <output.raw> <target_w> <target_h>
"""

import sys
import struct
from pathlib import Path
from PIL import Image


def main():
    if len(sys.argv) != 5:
        print(f"Usage: {sys.argv[0]} <input.png> <output.raw> <target_w> <target_h>")
        sys.exit(1)

    src    = Path(sys.argv[1])
    dst    = Path(sys.argv[2])
    tw     = int(sys.argv[3])
    th     = int(sys.argv[4])

    img = Image.open(src).convert("RGBA").resize((tw, th), Image.LANCZOS)
    W, H = img.size

    dst.parent.mkdir(parents=True, exist_ok=True)

    # File format: [uint32 W][uint32 H][W*H * uint32 BGRA pixels]
    # BGRA: B=byte0, G=byte1, R=byte2, A=byte3
    with open(dst, "wb") as f:
        f.write(struct.pack("<II", W, H))
        for r, g, b, a in img.getdata():
            # BGRA little-endian uint32
            val = (a << 24) | (r << 16) | (g << 8) | b
            f.write(struct.pack("<I", val))

    size_kb = dst.stat().st_size / 1024
    print(f"  ✓ {src.name}  →  {dst.name}  ({W}x{H}, {size_kb:.0f} KB)")


if __name__ == "__main__":
    main()
