sudo apt install python3 python3-pip zenity
pip3 install playsound
curl 'https://cdn.glitch.com/87229131-2a45-44d2-a487-bfa155b45f62%2Fassets.tar.gz?v=1627798416678' -o "assets.tar.gz"
tar -xf assets.tar.gz
rm assets.tar.gz
curl -O https://ekush.glitch.me/api/autogaz/linux/autogaz.sh
sudo chmod -R 755 ./autogaz.sh
zenity --info --text "Gerekli dosyalar indirildi. Çalıştırmak için autogaz.sh yi açman yeterli (tabi assets klasörüyle aynı klasörde olması gerekli)"
rm ./installer.sh
