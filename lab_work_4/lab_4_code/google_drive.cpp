#include "google_drive.h"

String myScript = "---------------------";
String myFoldername = "&myFoldername=ESP32-CAM";
String myFilename = "&myFilename=ESP32-CAM.jpg";
String myImage = "&myFile=";

void uploadPhotoToGoogleDrive() {
  const char* myDomain = "--------------------";
  String getAll="", getBody = "";

  camera_fb_t * fb = NULL;
  int retry = 5; 

  while (retry-- && !fb) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("❌ Помилка отримання кадру! Спробую ще раз...");
      delay(500);
    }
  }

  if (!fb) {
    Serial.println("❌ Камера все ще не відповідає. Перезавантажте ESP32.");
    return;
  }

  if (fb->format != PIXFORMAT_JPEG) {
    Serial.println("❌ Формат зображення не JPEG. Виконую конвертацію...");
    bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
    esp_camera_fb_return(fb);
    fb = NULL;
    if (!jpeg_converted) {
      Serial.println("❌ Помилка конвертації в JPEG!");
      return;
    }
  }

  Serial.println("✅ Фото отримано! Передаю у Google Drive...");
  
  WiFiClientSecure client_tcp;
  client_tcp.setInsecure();

  if (client_tcp.connect(myDomain, 443)) {
    Serial.println("✅ З'єднання успішне!");

    char *input = (char *)fb->buf;
    char output[base64_enc_len(3)];
    String imageFile = "data:image/jpeg;base64,";
    Serial.println("Передача фото: " + imageFile);
    Serial.println("Base64 довжина: " + String(imageFile.length()));
    for (int i = 0; i < fb->len; i += 3) {
      int len = (i + 3 <= fb->len) ? 3 : fb->len - i;
      base64_encode(output, (char*)(fb->buf + i), len);
      imageFile += urlencode(String(output));
    }

    String Data = "myFoldername=ESP32-CAM&myFilename=ESP32-CAM.jpg&myFile=";

    client_tcp.println("POST " + myScript + " HTTP/1.1");
    client_tcp.println("Host: " + String(myDomain));
    client_tcp.println("Content-Length: " + String(Data.length() + imageFile.length()));
    client_tcp.println("Content-Type: application/x-www-form-urlencoded");
    client_tcp.println("Connection: keep-alive");
    client_tcp.println();
    
    client_tcp.print(Data);
    for (int Index = 0; Index < imageFile.length(); Index += 1000) {
      client_tcp.print(imageFile.substring(Index, Index + 1000));
    }
    esp_camera_fb_return(fb);

    int waitTime = 10000;
    long startTime = millis();
    boolean state = false;

    while ((startTime + waitTime) > millis()) {
      Serial.print(".");
      delay(100);
      while (client_tcp.available()) {
        char c = client_tcp.read();
        if (state) getBody += String(c);
        if (c == '\n') {
          if (getAll.length() == 0) state = true;
          getAll = "";
        } else if (c != '\r') {
          getAll += String(c);
        }
        startTime = millis();
      }
      if (getBody.length() > 0) break;
    }
    client_tcp.stop();
    Serial.println(getBody);
  } else {
    Serial.println("❌ Помилка підключення до Google Apps Script");
  }
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
  }
  return encodedString;
}
