#!/usr/bin/env python3
"""
tools/ogg2raw.py — Converts OGG audio files to raw 16-bit signed stereo PCM (48000 Hz).
These files are included in the PKG as /app0/images/music.raw and streamed from memory at runtime.

Usage:
    python3 tools/ogg2raw.py <input.ogg> <output.raw>
"""

import sys
import shutil
import subprocess
from pathlib import Path


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input.ogg> <output.raw>")
        sys.exit(1)

    src = Path(sys.argv[1])
    dst = Path(sys.argv[2])

    if not src.exists():
        print(f"[ERROR] Source audio not found: {src}")
        sys.exit(1)

    dst.parent.mkdir(parents=True, exist_ok=True)

    # Find ffmpeg
    ffmpeg = shutil.which("ffmpeg") or "/opt/homebrew/bin/ffmpeg"
    if not Path(ffmpeg).exists():
        print("[ERROR] ffmpeg not found! Please install ffmpeg.")
        sys.exit(1)

    # Convert to 16-bit signed little-endian PCM, 48000 Hz, stereo
    cmd = [
        ffmpeg, "-y",
        "-i", str(src),
        "-f", "s16le",
        "-ar", "48000",
        "-ac", "2",
        str(dst)
    ]

    res = subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if res.returncode != 0:
        print(f"[ERROR] ffmpeg conversion failed for {src.name}")
        sys.exit(1)

    size_kb = dst.stat().st_size / 1024
    print(f"  ✓ {src.name}  →  {dst.name}  (48000Hz stereo PCM, {size_kb:.0f} KB)")


if __name__ == "__main__":
    main()
