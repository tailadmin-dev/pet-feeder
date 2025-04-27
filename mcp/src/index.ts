import { McpServer } from "@modelcontextprotocol/sdk/server/mcp.js";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/server/stdio.js";
import { z } from "zod";
import mqtt from "mqtt";
import dotenv from 'dotenv';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';

// .env dosyasını farklı konumlardan yüklemeyi dene
const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);
const rootDir = dirname(__dirname); // build klasörünün bir üstü, projenin kök dizini

// Sırasıyla farklı konumlarda .env dosyası aramayı dene
// 1. Önce mevcut dizinde (build dizini) ara
dotenv.config();
// 2. Build dizininde özellikle ara
dotenv.config({ path: join(__dirname, '.env') });
// 3. Projenin kök dizininde ara
dotenv.config({ path: join(rootDir, '.env') });

// Çevre değişkenlerinin değerlerini göster - debug için
console.error(`MQTT Server: ${process.env.MQTT_SERVER}`);
console.error(`Feed Channel: ${process.env.FEED_CHANNEL}`);
console.error(`MQTT Client ID: ${process.env.MQTT_CLIENT_ID}`);

const mqtt_server = process.env.MQTT_SERVER || "mqtt://localhost:1883";
const feed_topic = process.env.FEED_CHANNEL || "pet_feeder/feed";
const mqtt_client_id = process.env.MQTT_CLIENT_ID || "ai_agent";

// MQTT MESAJI GÖNDERME
export async function sendMqttMessage(
  brokerUrl: string,
  topic: string,
  message: string,
  options?: mqtt.IClientOptions
): Promise<void> {
  return new Promise((resolve, reject) => {
    const client = mqtt.connect(brokerUrl, options);

    client.on("connect", () => {
      client.publish(topic, message, (err) => {
        client.end();
        if (err) {
          console.error(`Failed to send MQTT message: ${err}`);
          reject(err);
        } else {
          console.log(`MQTT message sent to ${topic}: ${message}`);
          resolve();
        }
      });
    });

    client.on("error", (err) => {
      console.error(`MQTT connection error: ${err}`);
      client.end();
      reject(err);
    });
  });
}



// Create server instance
const server = new McpServer({
  name: "mama-kabi",
  version: "1.0.0",
  description: "Mama kabı aracı",
  tools:{
    feed: {
      description: "Mama kabını doldur",
      parameters: {
        amount: z.number().min(0).max(5).describe("Besleme miktarı (0-5)"),
      },
    },
  }
});

// Mama kabı araçlarını tanımlayalım
server.tool(
  "feed",
  "Mama kabını doldur",
  {
    amount: z.number().min(0).max(5).describe("Besleme miktarı (0-5)").optional().default(1),
  },
  async ({ amount = 1 }) => {
    if (amount < 0 || amount > 5) {
      return {
        content: [
          {
            type: "text",
            text: "Besleme miktarı 0 ile 5 arasında olmalıdır.",
          },
        ],
      };
    }
    // MQTT mesajı gönder
    const message = JSON.stringify({ amount });
    try {
      await sendMqttMessage(mqtt_server, feed_topic, message, {
        clientId: mqtt_client_id,
      });
      return {
        content: [
          {
            type: "text",
            text: `Mama kabı ${amount} birim mama ile dolduruldu.`,
          },
        ],
      };
    } catch (error:any) {
      return {
        content: [
          {
            type: "text",
            text: `Hata oluştu: ${error}`,
          },
        ],
      };
    }
  },
);

// Start the server
async function main() {
  const transport = new StdioServerTransport();
  await server.connect(transport);
  console.error(`Mama kabı aracı ${mqtt_server} mqtt sunucusu ile başlatıldı.`);
}

main().catch((error) => {
  console.error("Fatal error in main():", error);
  process.exit(1);
});
