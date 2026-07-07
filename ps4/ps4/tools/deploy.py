#!/usr/bin/env python3
"""
PS4 Brew — FTP Deployer (PKG'siz yöntem)
GoldHEN aktifken PKG kurmadan uygulamayı doğrudan yükler.

Kullanım:
    python3 tools/deploy.py 192.168.1.XXX

Yaptığı:
    1. param.sfo oluşturur
    2. icon0.png oluşturur
    3. FTP ile /data/homebrew/ps4brew/ altına kopyalar
"""

import sys, struct, ftplib, io
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

ROOT  = Path(__file__).resolve().parent.parent
EBOOT_SRC = ROOT / "eboot.bin"

TITLE_ID   = "BREW00001"
CONTENT_ID = "UP0000-BREW00001_00-PS4BREW000000001"
TITLE      = "PS4 Brew"
APP_VER    = "01.00"
VERSION    = "01.00"

# ── SFO ──────────────────────────────────────────────────────────────────────
FMT_UTF8_SPECIAL = 0x0004
FMT_UTF8         = 0x0204
FMT_UINT32       = 0x0404

_SFO_MAX = {
    "APP_VER": 8, "ATTRIBUTE": 4, "CATEGORY": 4,
    "CONTENT_ID": 48, "SYSTEM_VER": 4,
    "TITLE": 128, "TITLE_ID": 12, "VERSION": 8,
}

def build_sfo(entries):
    entries = sorted(entries, key=lambda e: e[0])
    key_table, key_offsets = b"", []
    for key, _, _ in entries:
        key_offsets.append(len(key_table))
        key_table += key.encode() + b"\x00"
    while len(key_table) % 4: key_table += b"\x00"

    data_table, data_meta = b"", []
    for key, fmt, val in entries:
        off = len(data_table)
        if fmt == FMT_UINT32:
            data_table += struct.pack("<I", val)
            data_meta.append((4, 4, off))
        else:
            raw   = val.encode("utf-8") + b"\x00"
            maxsz = _SFO_MAX.get(key, max(len(raw), 4))
            data_table += raw.ljust(maxsz, b"\x00")
            data_meta.append((len(raw), maxsz, off))
            while len(data_table) % 4: data_table += b"\x00"

    n = len(entries)
    kt_off = 0x14 + n * 0x10
    dt_off = kt_off + len(key_table)
    hdr = struct.pack("<IIIII", 0x46535000, 0x00000101, kt_off, dt_off, n)
    idx = b""
    for i, (key, fmt, _) in enumerate(entries):
        pl, pm, doff = data_meta[i]
        idx += struct.pack("<HHIII", key_offsets[i], fmt, pl, pm, doff)
    return hdr + idx + key_table + data_table

# ── İkon ─────────────────────────────────────────────────────────────────────
def make_icon() -> bytes:
    W, H = 512, 512
    img  = Image.new("RGB", (W, H))
    draw = ImageDraw.Draw(img)
    for y in range(H):
        t = y / H
        draw.line([(0,y),(W,y)], fill=(int(12+20*t), int(12+30*t), int(22+60*t)))
    bars = [42,58,71,64,88,95,82,91,79,65,54,47]
    cw, ch = W-80, H-160
    bslot = cw // len(bars)
    for i, v in enumerate(bars):
        bh = int(ch * v / 100)
        bx = 40 + i*bslot + 2
        by = 80 + ch - bh
        for row in range(bh):
            t2 = row/bh if bh else 1
            draw.rectangle([bx, by+bh-1-row, bx+bslot-4, by+bh-row],
                fill=(int(30+70*t2), int(80+100*t2), int(220+35*t2)))
    draw.line([(40,80),(40,80+ch)],         fill=(200,210,240), width=2)
    draw.line([(40,80+ch),(40+cw,80+ch)],   fill=(200,210,240), width=2)
    try:
        fb = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 36)
        fs = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 18)
    except:
        fb = fs = ImageFont.load_default()
    draw.text((W//2,32), "PS4 Brew", font=fb, fill=(255,255,255), anchor="mm")
    draw.text((W//2,460), "Graph · FW9.60", font=fs, fill=(120,140,200), anchor="mm")
    draw.line([(40,56),(W-40,56)], fill=(255,180,40), width=3)
    buf = io.BytesIO()
    img.save(buf, "PNG")
    return buf.getvalue()

# ── FTP deploy ───────────────────────────────────────────────────────────────
def ftp_mkdirs(ftp, path):
    """Klasörü ve üst klasörlerini oluşturur (varsa geç)."""
    parts = [p for p in path.split("/") if p]
    cur = ""
    for p in parts:
        cur += "/" + p
        try:
            ftp.mkd(cur)
            print(f"  mkdir {cur}")
        except ftplib.error_perm:
            pass   # zaten var

def ftp_put(ftp, data: bytes, remote_path: str, label: str):
    ftp.storbinary(f"STOR {remote_path}", io.BytesIO(data))
    print(f"  ✓  {label} → {remote_path}  ({len(data):,} bytes)")

def deploy(host: str, port: int = 2121):
    if not EBOOT_SRC.exists():
        print(f"[HATA] eboot.bin bulunamadı: {EBOOT_SRC}")
        print("  Önce 'make' çalıştırın.")
        sys.exit(1)

    print("\n── Dosyalar oluşturuluyor ─────────────────────────────────")
    sfo_data = build_sfo([
        ("APP_VER",    FMT_UTF8_SPECIAL, APP_VER),
        ("ATTRIBUTE",  FMT_UINT32,       0),
        ("CATEGORY",   FMT_UTF8_SPECIAL, "gd"),
        ("CONTENT_ID", FMT_UTF8,         CONTENT_ID),
        ("SYSTEM_VER", FMT_UINT32,       0x09600000),
        ("TITLE",      FMT_UTF8,         TITLE),
        ("TITLE_ID",   FMT_UTF8_SPECIAL, TITLE_ID),
        ("VERSION",    FMT_UTF8,         VERSION),
    ])
    icon_data  = make_icon()
    eboot_data = EBOOT_SRC.read_bytes()
    print(f"  ✓  param.sfo  ({len(sfo_data)} bytes)")
    print(f"  ✓  icon0.png  ({len(icon_data):,} bytes)")
    print(f"  ✓  eboot.bin  ({len(eboot_data):,} bytes)")

    print(f"\n── FTP bağlantısı: {host}:{port} ─────────────────────────")
    try:
        ftp = ftplib.FTP()
        ftp.connect(host, port, timeout=10)
        ftp.login("anonymous", "anonymous")
        print(f"  ✓  Bağlandı!")
    except Exception as e:
        print(f"  [HATA] FTP bağlantısı başarısız: {e}")
        print("  → PS4'ün açık ve GoldHEN'in yüklü olduğundan emin ol.")
        sys.exit(1)

    BASE = "/data/homebrew/ps4brew"
    ftp_mkdirs(ftp, BASE + "/sce_sys")

    print("\n── Dosyalar kopyalanıyor ──────────────────────────────────")
    ftp_put(ftp, eboot_data, f"{BASE}/eboot.bin",          "eboot.bin")
    ftp_put(ftp, sfo_data,   f"{BASE}/sce_sys/param.sfo",  "param.sfo")
    ftp_put(ftp, icon_data,  f"{BASE}/sce_sys/icon0.png",  "icon0.png")

    ftp.quit()

    print(f"""
══════════════════════════════════════════════════════════════
  ✅  Deploy tamamlandı!

  PS4'te açmak için:
    GoldHEN → Homebrew Launcher → "PS4 Brew"
  veya
    Settings → Debug Settings → Game → Browse
    → /data/homebrew/ps4brew/eboot.bin
══════════════════════════════════════════════════════════════
""")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Kullanım: python3 tools/deploy.py <PS4_IP>")
        print("Örnek:    python3 tools/deploy.py 192.168.1.105")
        sys.exit(1)
    deploy(sys.argv[1])
