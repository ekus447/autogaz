#!/usr/bin/env python3
"""
PS4 Brew — Klog Capturer & Filter
Connects to GoldHEN's Klog server on port 3232 and filters out the VSH/ShellUI log spam,
showing only process launches, crashes, kernel prints, and signals.

Kullanım:
    python3 tools/capture_logs.py 192.168.68.109
"""

import sys
import socket

def main():
    if len(sys.argv) < 2:
        print("Kullanım: python3 tools/capture_logs.py <PS4_IP>")
        sys.exit(1)
        
    host = sys.argv[1]
    port = 3232
    
    print(f"📡 GoldHEN Klog sunucusuna bağlanılıyor ({host}:{port})...")
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(15)
        s.connect((host, port))
        s.settimeout(None)
        print("✅ Bağlandı! Loglar filtrelenerek gösteriliyor. Uygulamayı PS4'te başlatabilirsiniz.")
        print("Ctrl+C ile çıkabilirsiniz.\n")
    except Exception as e:
        print(f"❌ Bağlantı hatası: {e}")
        print("→ PS4'te GoldHEN Klog ayarının aktif olduğundan emin olun.")
        sys.exit(1)
        
    buffer = ""
    try:
        while True:
            data = s.recv(4096)
            if not data:
                print("\n📡 PS4 bağlantıyı kapattı.")
                break
            
            buffer += data.decode("utf-8", errors="ignore")
            while "\n" in buffer:
                line, buffer = buffer.split("\n", 1)
                line = line.strip()
                
                # VSH, ShellUI ve gereksiz sistem loglarını filtrele
                if any(x in line for x in [
                    "[SceShellUI]", "[SceShellCore]", "[Theme/I]", "[SceHidConfigService]",
                    "[ProfileCacheManager]", "GetPlusInfoJob", "[UpdateUserInfoforPlus]",
                    "TrophyNet", "TrophyPlugin", "TrophyRootScene", "ConnectionManager",
                    "[NpPush]", "FMEM", "VM Stats", "Libc Heap Status", "No user input",
                    "[Crash Reporter]", "[CRASHREPORT_UI", "Coredump Progress", "Coredump Complete",
                    "[SceLncService] ConvertToSysCoreKillReason", "unmountAllForShellCore",
                    "[SceWorkaroundCtl]", "[SceSystemStateMgr]"
                ]):
                    continue
                
                # Boş veya sadece tarih/saat olan satırları atla
                if not line or line.startswith("<118>D/"):
                    continue
                    
                # İlginç satırları renklendirerek yazdır
                if "App Crash" in line or "signal" in line or "SIG" in line or "fault" in line or "rip:" in line:
                    print(f"\033[91m🚨 {line}\033[0m")
                elif "EXEC" in line or "processSpawn" in line or "launchApp" in line:
                    print(f"\033[92m🚀 {line}\033[0m")
                elif "GoldHEN" in line:
                    print(f"\033[96m👑 {line}\033[0m")
                else:
                    print(line)
                    
    except KeyboardInterrupt:
        print("\n👋 Çıkış yapılıyor...")
    finally:
        s.close()

if __name__ == "__main__":
    main()
