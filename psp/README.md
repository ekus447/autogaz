### **PSP GAZ**

### *!! bu biraz zahmetli olacak!!*

-----------------------------------------------------

### > compile etmek için:
- öncelikle [bu adresten pspsdk indiricisini](https://sourceforge.net/projects/minpspw/files/SDK%20%2B%20devpak/pspsdk%200.10.0/pspsdk-setup-0.10.0.exe/download) indir
- environment vairablelarına ***/bin ekle (ör: `C:\pspsdk\bin`)
- ppsspp indirip emulator kısmına koy
- artık `make` diyebilirsin

----------------------------------------------------

### > güzel bir build:
- [Bu adresten](https://filetrip.net/psp-downloads/tools-utilities/download-pbp-unpacker-094-f6777.html) psp repacker indirip extractla
- sonra onları oluşturduğun eboot.pbp ile yapıp ayarla
- resim falan koy işte
- sonra save diyip yeni eboot.pbp yap
- artık zip yapabilirsin ama klasöre `resources` klasörünü de eklemeyi unutma!!

----------------------------------------------------

### > Special THX:

- https://web.archive.org/web/20080130075401/http://www.psp-programming.com/tutorials/c/lesson06.htm
- https://gbatemp.net/threads/how-to-unpack-repack-homebrew-pbp.328306/

--------------------------------------------------

*NOT: seste bazen kasma olabiliyo haberin ola ses baya zorladı kodlarken!!!!*

*NOT2: musicplayer.c kodunu yukarıdaki 1. linkteki yerden aldım ancak satır 615'e `Stream.buffer = 0;` kodunu ekledim*