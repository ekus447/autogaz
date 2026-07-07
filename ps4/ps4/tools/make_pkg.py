#!/usr/bin/env python3
"""
tools/make_pkg.py  —  AutoGaz PS4 PKG Builder
"""
import os
import sys
import subprocess
import shutil
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

ROOT     = Path(__file__).resolve().parent.parent
EBOOT_SRC = ROOT / "eboot.bin"
PKGTOOL  = ROOT / "tools" / "pkgtool" / "PkgTool.Core"
STUBS_DIR = ROOT / "tools" / "pkg_stubs"
PROJ     = ROOT / "pkg_project"
OUT      = ROOT

TITLE_ID   = "AGAZ00001"
CONTENT_ID = "UP0000-AGAZ00001_00-PS4AUTOGAZ000001"
TITLE      = "AutoGaz PS4"
APP_VER    = "01.00"
VERSION    = "01.00"

def generate_icon(path: Path):
    """
    PS4 icon0.png (512x512):
    wii/icon.png (Polat Alemdar) resmi kullanılır, üzerine başlık yazısı eklenir.
    """
    W, H = 512, 512

    # wii/icon.png'yi bul ve yükle
    wii_icon = ROOT.parent / "wii" / "icon.png"
    if wii_icon.exists():
        base = Image.open(wii_icon).convert("RGB").resize((W, H), Image.LANCZOS)
    else:
        # Fallback: koyu gradient
        base = Image.new("RGB", (W, H))
        draw0 = ImageDraw.Draw(base)
        for y in range(H):
            t = y / H
            draw0.line([(0, y), (W, y)], fill=(int(10+5*(1-t)), int(10+5*(1-t)), int(30+20*(1-t))))

    # Üst ve alt kısma koyu overlay
    overlay = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    ov_draw = ImageDraw.Draw(overlay)

    # Üstte başlık için koyu gradient şerit (0-130px)
    for y in range(0, 130):
        alpha = int(210 * (1 - y / 130))
        ov_draw.line([(0, y), (W, y)], fill=(0, 0, 0, alpha))

    img = base.convert("RGBA")
    img = Image.alpha_composite(img, overlay).convert("RGB")
    draw = ImageDraw.Draw(img)

    # Altın çerçeve
    border = 10
    draw.rectangle([border, border, W - border, H - border],
                   outline=(255, 200, 0), width=5)

    # Fontlar
    try:
        font_big  = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 52)
        font_sub  = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 22)
        font_hint = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 20)
    except Exception:
        font_big = font_sub = font_hint = ImageFont.load_default()

    # Başlık gölgesi + yazı (10px aşağı indirildi)
    draw.text((W // 2 + 2, 48), "AutoGaz PS4", font=font_big,
              fill=(0, 0, 0), anchor="mm")
    draw.text((W // 2, 46), "AutoGaz PS4", font=font_big,
              fill=(255, 220, 0), anchor="mm")

    # Alt yazı (@egemenkus0 - beyaz renk, gölgeli)
    draw.text((W // 2 + 1, 93), "@egemenkus0", font=font_sub,
              fill=(0, 0, 0), anchor="mm")
    draw.text((W // 2, 92), "@egemenkus0", font=font_sub,
              fill=(255, 255, 255), anchor="mm")

    path.parent.mkdir(parents=True, exist_ok=True)
    img.save(path, "PNG")
    print("  ✓ icon0.png generated from wii/icon.png (Polat theme)")


def main():
    print("==============================================================")
    print("  AutoGaz PS4 — PKG Builder")
    print("==============================================================")

    if not EBOOT_SRC.exists():
        print(f"[ERROR] eboot.bin not found: {EBOOT_SRC}")
        print("  Please build the project first (run 'make').")
        sys.exit(1)

    # 1. Dizin yapısı
    sce_sys = PROJ / "sce_sys"
    sce_sys.mkdir(parents=True, exist_ok=True)

    sce_module = PROJ / "sce_module"
    sce_module.mkdir(parents=True, exist_ok=True)

    # eboot.bin kopyala
    shutil.copy2(EBOOT_SRC, PROJ / "eboot.bin")
    print(f"  ✓ Copied signed eboot.bin ({EBOOT_SRC.stat().st_size:,} bytes)")

    # Stub PRX modülleri kopyala
    libc_stub  = STUBS_DIR / "libc.prx"
    fios2_stub = STUBS_DIR / "libSceFios2.prx"

    if not libc_stub.exists() or not fios2_stub.exists():
        print(f"[ERROR] Stub modules not found in {STUBS_DIR}!")
        print("  Bunları ps4brew/tools/pkg_stubs/ klasöründen kopyalayın.")
        sys.exit(1)

    shutil.copy2(libc_stub,  sce_module / "libc.prx")
    shutil.copy2(fios2_stub, sce_module / "libSceFios2.prx")
    print("  ✓ Copied stub PRX modules (libc.prx, libSceFios2.prx)")

    # RAW görüntü dosyalarını kopyala (/app0/images/*.raw)
    images_dst = PROJ / "images"
    images_dst.mkdir(exist_ok=True)
    images_src = ROOT / "images"
    for raw_file in ["before.raw", "after.raw", "polat.raw", "music.raw"]:
        src_path = images_src / raw_file
        if not src_path.exists():
            print(f"[ERROR] Raw image not found: {src_path}")
            print("  Please run 'make images' first.")
            sys.exit(1)
        shutil.copy2(src_path, images_dst / raw_file)
    raw_sizes = sum((images_dst / f).stat().st_size for f in ["before.raw", "after.raw", "polat.raw", "music.raw"])
    print(f"  ✓ Copied raw images ({raw_sizes / 1024:.0f} KB total)")

    # İkon oluştur
    generate_icon(sce_sys / "icon0.png")

    # 2. param.sfo oluştur
    sfo_path = sce_sys / "param.sfo"
    if sfo_path.exists():
        sfo_path.unlink()

    print("  Building param.sfo...")
    subprocess.run([str(PKGTOOL), "sfo_new", str(sfo_path)], check=True)
    subprocess.run([str(PKGTOOL), "sfo_setentry", str(sfo_path), "APP_VER",
                    "--type", "utf8",    "--value", APP_VER,    "--maxsize", "8"], check=True)
    subprocess.run([str(PKGTOOL), "sfo_setentry", str(sfo_path), "ATTRIBUTE",
                    "--type", "integer", "--value", "0"], check=True)
    subprocess.run([str(PKGTOOL), "sfo_setentry", str(sfo_path), "CATEGORY",
                    "--type", "utf8",    "--value", "gd",        "--maxsize", "4"], check=True)
    subprocess.run([str(PKGTOOL), "sfo_setentry", str(sfo_path), "CONTENT_ID",
                    "--type", "utf8",    "--value", CONTENT_ID,  "--maxsize", "48"], check=True)
    subprocess.run([str(PKGTOOL), "sfo_setentry", str(sfo_path), "SYSTEM_VER",
                    "--type", "integer", "--value", "0"], check=True)
    subprocess.run([str(PKGTOOL), "sfo_setentry", str(sfo_path), "TITLE",
                    "--type", "utf8",    "--value", TITLE,       "--maxsize", "128"], check=True)
    subprocess.run([str(PKGTOOL), "sfo_setentry", str(sfo_path), "TITLE_ID",
                    "--type", "utf8",    "--value", TITLE_ID,    "--maxsize", "12"], check=True)
    subprocess.run([str(PKGTOOL), "sfo_setentry", str(sfo_path), "VERSION",
                    "--type", "utf8",    "--value", VERSION,     "--maxsize", "8"], check=True)
    print("  ✓ param.sfo generated")

    # 3. GP4 dosyası oluştur
    gp4_path = PROJ / "ps4autogaz.gp4"
    gp4_content = f"""<?xml version="1.0" encoding="utf-8" standalone="yes"?>
<psproject fmt="gp4" version="1000">
  <volume>
    <volume_type>pkg_ps4_app</volume_type>
    <volume_id>PS4AUTOGAZ</volume_id>
    <volume_ts>2026-07-04 00:00:00</volume_ts>
    <package content_id="{CONTENT_ID}"
             passcode="00000000000000000000000000000000"
             storage_type="digital25"
             app_type="full"
             entitlement_key="00000000000000000000000000000000"/>
    <c_date>2026-07-04</c_date>
  </volume>
  <files>
    <file id="1000" targ_path="sce_sys/param.sfo" orig_path="sce_sys/param.sfo" chunk="0"/>
    <file id="1200" targ_path="sce_sys/icon0.png"  orig_path="sce_sys/icon0.png"  chunk="0"/>
    <file id="1400" targ_path="eboot.bin"           orig_path="eboot.bin"           chunk="0"/>
    <file id="1500" targ_path="sce_module/libc.prx"        orig_path="sce_module/libc.prx"        chunk="0"/>
    <file id="1600" targ_path="sce_module/libSceFios2.prx" orig_path="sce_module/libSceFios2.prx" chunk="0"/>
    <file id="2000" targ_path="images/before.raw" orig_path="images/before.raw" chunk="0"/>
    <file id="2001" targ_path="images/after.raw"  orig_path="images/after.raw"  chunk="0"/>
    <file id="2002" targ_path="images/polat.raw"  orig_path="images/polat.raw"  chunk="0"/>
    <file id="2003" targ_path="images/music.raw"  orig_path="images/music.raw"  chunk="0"/>
  </files>
  <chunks>
    <chunk id="0" label="Chunk #0">
      <scenario id="0" type="sp" initial_chunk_count="1" label="Scenario #0">0</scenario>
    </chunk>
  </chunks>
  <rootdir>
    <dir targ_name="sce_sys"/>
    <dir targ_name="sce_module"/>
    <dir targ_name="images"/>
  </rootdir>
</psproject>
"""
    gp4_path.write_text(gp4_content)
    print("  ✓ ps4autogaz.gp4 generated")

    # 4. PKG üret
    print("  Building PKG via PkgTool...")
    subprocess.run([str(PKGTOOL), "pkg_build", str(gp4_path), str(OUT)], check=True)

    # Çıktıyı bul
    pkg_file     = OUT / f"{CONTENT_ID}.pkg"
    symlink_file = OUT / "ps4autogaz.pkg"

    if pkg_file.exists():
        if symlink_file.exists():
            symlink_file.unlink()
        shutil.copy2(pkg_file, symlink_file)
        print("==============================================================")
        print("  ✅ PKG Built Successfully!")
        print(f"  📦 Output: {pkg_file}")
        print(f"  📦 Link:   {symlink_file}")
        print("==============================================================")
    else:
        print("❌ PKG creation failed.")


if __name__ == "__main__":
    main()
