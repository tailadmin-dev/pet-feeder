# Pet Feeder Projesi

Bu proje, evcil hayvanlarınızın beslenme zamanlarını otomatikleştirmek için bir ESP32 tabanlı bir besleyici geliştirmeyi amaçlamaktadır.

## Özellikler

- **Otomatik Besleme:** Belirli zamanlarda evcil hayvanınızı besler.
- **Wi-Fi Bağlantısı:** Uzaktan kontrol ve izleme imkanı.
- **Mobil Uygulama:** Besleme zamanlarını ayarlamak ve izlemek için mobil uygulama desteği.
- **Sensörler:** Yem miktarını ve evcil hayvanın beslenme durumunu izlemek için sensörler.

## Gereksinimler

- ESP32 Geliştirme Kartı
- Servo Motor
- Sensörler (IR Sensör)
- 3D Yazıcı (Opsiyonel, besleyici parçalarını üretmek için)

## Kurulum

1. ESP32 kartınızı bilgisayarınıza bağlayın.
2. Gerekli kütüphaneleri yükleyin:
    ```bash
    arduino-cli lib install "WiFi"
    arduino-cli lib install "Servo"
    ```
3. example.config.cpp dosyasının adını config.cpp olarak değiştirerek wifi ve mqtt bilgilerinizi giriniz.
4. Çalışma sırasında gereken tüm ayar parametreleri config.cpp içinde açıklamalı olarak yer almaktadır. Buradaki parametreleri değiştirdikten sonra esp32 kartınıza yeniden yükleyin.
5. Proje dosyalarını ESP32 kartınıza yükleyin.
6. Mobil uygulamayı indirip kurun veya Wi-Fi üzerinden ESP32 kartınıza bağlanın.

## Kullanım

1. Mama kabının kurulumunu yapın.
2. Mama doldurun ve en az 3 tur manuel besleme yaparak IR sensörün mamayı görmesini sağlayın.
3. Besleme zamanlarında otomatik olarak çalışacaktır.

## Katkıda Bulunma

Katkıda bulunmak için lütfen bir pull request gönderin veya bir issue açın.

## Lisans

Bu proje MIT Lisansı ile lisanslanmıştır. Daha fazla bilgi için `LICENSE` dosyasına bakın.
