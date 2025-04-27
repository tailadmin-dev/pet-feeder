# Pet Feeder için MCP (Model Context Protocol) Sunucusu

Bu sunucu, evcil hayvan besleyicisini (Pet Feeder) yapay zeka asistanları üzerinden kontrol etmek için bir Model Context Protocol (MCP) uygulamasıdır. MCP entegrasyonu sayesinde, AI asistanlarınız aracılığıyla evcil hayvanınızın beslenmesini yönetebilirsiniz.

## 🔍 Özellikler

- **AI Asistan Entegrasyonu:** MCP protokolü ile AI asistanların mama kabınızı kontrol etmesini sağlar
- **MQTT Desteği:** ESP32/ESP8266 besleyici cihazlarıyla MQTT üzerinden iletişim kurar
- **Esnek Kontrol:** Besleme miktarını 0-5 arası ölçekte ayarlama imkanı
- **TypeScript ile Geliştirilmiş:** Modern ve tip-güvenli kod yapısı

## 📋 Gereksinimler

- Node.js (v16 veya üstü)
- MQTT Broker (örn. Mosquitto, CloudMQTT, HiveMQ)
- ESP32 veya ESP8266 tabanlı Pet Feeder donanımı (esp32/ veya esp8266/ klasörlerindeki kodlar ile programlanmış)

## 🚀 Kurulum

1. Paket bağımlılıklarını yükleyin:
   ```bash
   npm install
   ```

2. `.env.example` dosyasını `.env` olarak kopyalayın ve MQTT ayarlarınızı yapılandırın:
   ```bash
   cp .env.example .env
   ```

3. `.env` dosyasını ihtiyaçlarınıza göre düzenleyin:
   ```
   MQTT_SERVER="mqtt://your-broker-address:1883"
   FEED_CHANNEL="pet_feeder/feed"
   MQTT_CLIENT_ID="ai_agent"
   ```

4. Uygulamayı derleyin:
   ```bash
   npm run build
   ```

   > **Not:** Derleme işlemi otomatik olarak `.env.example` dosyasını build klasörüne `.env` olarak kopyalar. Değişkenlerinizi build/.env içinde tanımlayınız.

## 📱 Kullanım

1. Sunucuyu çalıştırın:
   ```bash
   node build/index.js
   ```

   > **Not:** Uygulama çalışırken `.env` dosyasını şu konumlarda arar:
   > - Projenin kök dizininde
   > - Uygulamanın çalıştığı mevcut dizinde
   >
   > Eğer farklı bir dizinden çalıştırıyorsanız, o dizinde de `.env` dosyası oluşturmalı veya ortam değişkenlerini doğrudan işletim sistemi üzerinden tanımlamalısınız.

2. Sunucu çalıştığında, MCP protokolü destekleyen AI asistanlar aşağıdaki aracı kullanabilir:

   - **feed:** Besleyiciye belirli bir miktarda mama vermesini emreder
     - **amount**: Besleme miktarı (0-5 arası, belirtilmezse varsayılan olarak 1 birim)

## 🛠️ MCP Aracı API'si

Bu MCP sunucusu aşağıdaki aracı sunar:

### feed

Evcil hayvan besleyicisini belirtilen miktarda mama vermesi için tetikler.

**Parametreler:**
- `amount` (number, opsiyonel): Verilecek mama miktarı, 0-5 arası bir değer olmalıdır. Belirtilmezse varsayılan olarak 1 birim kullanılır.

**Örnek Kullanım:**
- "Kedimi besle" → 1 birim mama verir (varsayılan)
- "Kedimi 2 birim mama ile besle" → 2 birim mama verir

## 🔄 MQTT İletişimi

Sunucu, MQTT üzerinden aşağıdaki formatta mesaj gönderir:

```json
{
    "amount": 2
}
```

Bu mesaj, `.env` dosyasında belirtilen kanala (varsayılan: `pet_feeder/feed`) gönderilir ve ESP32/ESP8266 tarafından alınarak işlenir.

## 🤖 Model Context Protocol (MCP) Hakkında

MCP, AI asistanların uygulamalarınız ve cihazlarınızla etkileşimde bulunmasını sağlayan açık bir protokoldür. Daha fazla bilgi için [MCP resmi web sitesini](https://modelcontextprotocol.io/) ziyaret edebilirsiniz.

## ⚠️ Sorun Giderme

- **MQTT Bağlantı Sorunları:**
  - MQTT broker adresinizin doğru olduğundan emin olun
  - Firewall ayarlarınızın MQTT portuna (genellikle 1883) izin verdiğini kontrol edin
  - MQTT broker'ınızın çalıştığından emin olun

- **MCP Araç Çağrısı Çalışmıyor:**
  - Konsol çıktılarını kontrol edin
  - MQTT mesajlarının doğru kanala gönderildiğinden emin olun
  - ESP32/ESP8266 cihazınızın çalıştığından ve aynı MQTT kanalına abone olduğundan emin olun

- **Ortam Değişkenleri Sorunu:**
  - `.env` dosyasının doğru konumda olduğundan emin olun
  - Uygulamayı farklı bir dizinden çalıştırıyorsanız, `.env` dosyasını o dizine kopyalayın veya tam yol belirtin

## 🔧 Geliştirme

Projeye katkıda bulunmak isterseniz:

1. Bu depoyu fork edin
2. Yeni bir özellik dalı oluşturun (`git checkout -b yeni-ozellik`)
3. Değişikliklerinizi commit edin (`git commit -am 'Yeni özellik eklendi'`)
4. Dalınızı push edin (`git push origin yeni-ozellik`)
5. Bir Pull Request oluşturun

## 📄 Lisans

Bu proje MIT Lisansı altında lisanslanmıştır.
