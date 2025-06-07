#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "Base64.h"
#include "camera_pins.h"

#include "oled_display.h"
#include "google_drive.h"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

// Wi-Fi
const char* ssid = "your ssid";
const char* password = "your password";

// MQTT
const char* mqtt_server = "----------------";
const int mqtt_port = 8883;
const char* mqtt_user = "------------------";
const char* mqtt_pass = "------------------";

WiFiClientSecure secureClient;
PubSubClient client(secureClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800, 60000);

int photoCount = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("📥 Recieved message from theme: ");
  Serial.println(topic);

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("➡️ Message: ");
  Serial.println(message);

  if (message == "photo") {
    Serial.println("📸 Command: take photo!");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 20);
    display.println("Taking Photo!");
    display.display();

    delay(2500);

    uploadPhotoToGoogleDrive();
    photoCount++;
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5, 25);
    display.println("Oops! Wrong Command...");
    display.display();

    delay(2000);
  }

  updateDisplay(photoCount);
}

void connectToMQTT() {
  secureClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32S3Client", mqtt_user, mqtt_pass)) {
      Serial.println("✅ MQTT connected!");
      client.subscribe("esp32/test");
      client.publish("esp32/test", "ESP32-S3 is ready to receive messages");
    } else {
      Serial.print("❌ Error: ");
      Serial.print(client.state());
      Serial.println(" — retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi підключено!");

  client.setCallback(callback); 
  connectToMQTT();

  initOLED();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_HVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("❌ Помилка ініціалізації камери!");
    return;
  }
  Serial.println("✅ Камера готова до роботи!");

  startCameraServer(); 

  Serial.print("📷 Camera Ready! Open 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to view.");

  // NTP
  timeClient.begin();
}

void loop() {
  client.loop();
  timeClient.update();
}
