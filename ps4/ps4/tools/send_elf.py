#!/usr/bin/env python3
"""
PS4 Brew — ELF Sender (HTTP POST)
GoldHEN 2.4b18+ BinLoader → port 9090 HTTP POST ile ELF gönderir.
Raw TCP değil, HTTP POST kullanır.

Kullanım:
    python3 tools/send_elf.py 192.168.1.XXX
"""

import sys, urllib.request, urllib.error
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ELF  = ROOT / "ps4brew" / "x64" / "Debug" / "ps4brew.elf"
OELF = ROOT / "ps4brew" / "x64" / "Debug" / "ps4brew.oelf"

ELF_PORT = 9090

def send_elf(host: str, port: int = ELF_PORT):
    # Raw ELF tercih et, yoksa OELF kullan
    if ELF.exists():
        target = ELF
        fmt = "raw ELF"
    elif OELF.exists():
        target = OELF
        fmt = "OELF"
    else:
        print("[HATA] ELF bulunamadı. Önce 'make' çalıştırın.")
        sys.exit(1)

    data = target.read_bytes()
    url  = f"http://{host}:{port}/"

    print(f"""
╔══════════════════════════════════════════════╗
║       PS4 Brew — ELF Sender (HTTP POST)      ║
╚══════════════════════════════════════════════╝
  Hedef : {url}
  Dosya : {target.name}  [{fmt}]
  Boyut : {len(data):,} bytes ({len(data)/1024:.1f} KB)
""")

    print("  Gönderiliyor...", end="", flush=True)
    try:
        req = urllib.request.Request(
            url,
            data=data,
            method="POST",
            headers={"Content-Type": "application/octet-stream"}
        )
        with urllib.request.urlopen(req, timeout=15) as resp:
            status = resp.status
            powered = resp.headers.get("X-Powered-By", "?")
            print(f" HTTP {status}")
            print(f"\n  ✅  Kabul edildi! ({powered})")
            print(f"\n  🎮  PS4 ekranına bak — uygulama başlatılıyor!")
    except urllib.error.HTTPError as e:
        print(f" HTTP {e.code}")
        print(f"\n  [HATA] Sunucu reddetti: {e}")
        if e.code == 405:
            print("  → GoldHEN henüz tam aktif olmamış olabilir.")
            print("  → Jailbreak'ten sonra 30-60 sn bekle, tekrar dene.")
    except urllib.error.URLError as e:
        print(f"\n  [HATA] Bağlanamadı: {e.reason}")
        print(f"  → GoldHEN aktif mi? PS4 açık mı?")
        print(f"  → Aynı ağda mısın?")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Kullanım: python3 tools/send_elf.py <PS4_IP> [port]")
        print("Örnek:    python3 tools/send_elf.py 192.168.68.109")
        sys.exit(1)
    host = sys.argv[1]
    port = int(sys.argv[2]) if len(sys.argv) > 2 else ELF_PORT
    send_elf(host, port)
