# AutoGaz PS4 🚀

**AutoGaz PS4**, Polat Alemdar temalı, dinamik ses ve ekran geçişlerine sahip, OpenOrbis Toolchain ile sıfırdan geliştirilmiş **60 FPS ultra akıcı** bir PlayStation 4 Homebrew uygulamasıdır.

Bu proje, yüksek performanslı render ve ses motoru optimizasyonlarıyla **PS4 FW 9.60 (GoldHEN)** üzerinde sorunsuz, sıfır gecikmeli ve tam uyumlu bir deneyim sunmak amacıyla tasarlanmıştır.

---

## ✨ Öne Çıkan Özellikler

- ⚡ **60 FPS Kilitli Ultra Akıcı Render Motoru:** Doğrudan işaretçi (direct pointer) tabanlı blit işlemi ve CPU önbellekli RAM (L1/L2 cacheable) kullanımı sayesinde sıfır gecikme ve anlık tuş tepkimeleri.
- 🎬 **Dinamik Ekran ve Ses Akışı:** 
  - **Ekran 0:** Başlangıç ekranı ve yönlendirme (`[X] auto gazlan!`).
  - **Ekran 2:** Müzik eşliğinde dinamik sayma ve akış ekranı (8.13 saniye hassas zamanlayıcı).
  - **Ekran 3:** Polat Alemdar final ekranı (`[0] devam...` - sol üst konumlandırmalı).
- 🎵 **Donanımsal Ses Oynatma:** `libSceAudioOut` kütüphanesi üzerinden 48.000 Hz Stereo PCM ham ses (`music.raw`) akışı.
- 🎨 **Gelişmiş Görsel Tasarım:**
  - Ekran sağ üst köşesinde beyaz **`@egemenkus0`** imza etiketi.
  - Her türlü arka plan görselinde mükemmel okunabilirlik sağlayan, yazılı öğelerin arkasına yerleştirilmiş %75 koyulukta yarı saydam arka plan kutuları (*Black Box*).
  - Otomatik oluşturulan ve Polat Alemdar temalı özel 512x512 PS4 menü ikonu (`icon0.png`).

---

## 📂 Klasör ve Proje Yapısı

```text
ps4autogaz/
├── README.md              # Proje dökümantasyonu (Bu dosya)
├── .gitignore             # Derleme çıktıları ve işletim sistemi artıkları için kural dosyası
├── ps4/                   # PS4 OpenOrbis C++ kaynak kodları ve derleme araçları
│   ├── Makefile           # Derleme ve PKG oluşturma yapılandırması
│   ├── main.cpp           # Ana oyun/arayüz döngüsü ve State makinesi
│   ├── graphics.cpp / .h  # 60 FPS optimize edilmiş Framebuffer render motoru
│   ├── audio.cpp / .h     # libSceAudioOut tabanlı PCM ses oynatıcı
│   ├── tools/             # Görsel/Ses dönüştürücü ve PKG paketleme betikleri (Python)
│   └── pkg_project/       # PKG şablonu, SFO yapılandırması ve zorunlu stub PRX modülleri
└── wii/                   # Orijinal PNG/OGG ham varlık (asset) dosyaları ve Wii portu kaynakları
```

---

## 🛠️ Derleme Gereksinimleri

Projenin PS4 sürümünü sıfırdan derleyip `.pkg` dosyası oluşturabilmek için sisteminizde aşağıdaki araçların kurulu olması gerekmektedir:

1. **[OpenOrbis PS4 Toolchain](https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain):** 
   - Varsayılan kurulum yolu `/opt/OpenOrbis-PS4-Toolchain` olarak ayarlanmış olmalıdır.
2. **LLVM / Clang++ & LLD Derleyicileri:** 
   - macOS (Homebrew ile `llvm`) veya Linux sistemler.
3. **Python 3.x & Pillow Kütüphanesi:** 
   - İkon ve varlık dönüşümleri için gereklidir (`pip install Pillow`).

---

## 🚀 Nasıl Derlenir (Build)

Terminalinizi açın ve aşağıdaki adımları sırasıyla takip edin:

```bash
# 1. Depoyu bilgisayarınıza klonlayın
git clone https://github.com/KULLANICI_ADINIZ/ps4autogaz.git
cd ps4autogaz/ps4

# 2. Toolchain yolunu ortama tanımlayın (Gerekirse)
export OO_PS4_TOOLCHAIN=/opt/OpenOrbis-PS4-Toolchain

# 3. Önceki derleme artıklarını temizleyin ve yeni PKG oluşturun
make clean
make pkg
```

Bu komutların tamamlanmasının ardından `ps4/` klasörü içerisinde **`ps4autogaz.pkg`** (yaklaşık 7 MB) dosyası derlenmiş ve kuruluma hazır hale gelecektir!

---

## 🎮 PS4 Üzerinde Kurulum ve Çalıştırma

1. Derleme sonucunda oluşan `ps4autogaz.pkg` dosyasını **exFAT** formatlı bir USB belleğin ana dizinine kopyalayın.
2. USB belleği **GoldHEN** yüklü (FW 9.60) PlayStation 4 konsolunuza takın.
3. PS4 menüsünden `Settings -> GoldHEN -> Package Installer` yolunu izleyin.
4. `AutoGaz PS4` paketini seçerek yükleyin ve ana menüden uygulamayı başlatın!

---

## 🕹️ Kontroller

| Tuş | İşlev |
| :--- | :--- |
| **`[ X ]` (Çarpı)** | AutoGaz akışını başlatır / Gazlar! (`[X] auto gazlan!`) |
| **`[ O ]` (Daire)** | Polat ekranından başlangıç ekranına sıfırlar ve döner (`[0] devam...`) |

---

## 👨‍💻 Geliştirici / Künye

- **Geliştirici & Tasarım:** **`@egemenkus0`**
- **Altyapı:** OpenOrbis PS4 Toolchain, GoldHEN & PlayStation 4 Homebrew SDK
