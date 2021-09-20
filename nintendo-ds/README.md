## **autogaz-nds**
------------------------------------------------
### **\> Compile etmek için**
- devkitpro'dan nds dependecies'leri kur
- emulator kısmına `DeSmuME_0.9.11_x64` emulatörünü koy
- Ben burda **nds_nflib** kullanıyorum benim versiyonu kullanmıcaksan yenisini hallet
- `make` de
-----------------------------------------------
### **\> Ses Ayarları**
- Ses için [bu programı](https://www.nch.com.au/switch/index.html
) kur ve seçenekler'den raw seç. sonra ayar olarak `11025 khz / 8 bit signed / mono` yap.
---------------------------------------------

***Not: Ben bunu windows 10'da çalıştırdım ancak linux bilgisayarımda çalılşıtrdığımda koddaki `#define TIMER_SPEED (BUS_CLOCK / 1024)` yüzünden zaman bugu oldu. Gerçek ds'te sorun olmaması gerek ama söylemem gerek diye düşündüm.***