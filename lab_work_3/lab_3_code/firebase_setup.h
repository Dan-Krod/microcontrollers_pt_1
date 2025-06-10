#ifndef FIREBASE_SETUP_H
#define FIREBASE_SETUP_H

#include <FirebaseESP32.h>

#define FIREBASE_HOST "----------------------"
#define FIREBASE_AUTH "----------------------"

FirebaseConfig firebase_config;
FirebaseAuth auth;
FirebaseData firebaseData;
String scheduledTime = "";

void setupFirebase() {
  firebase_config.host = FIREBASE_HOST;
  firebase_config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&firebase_config, &auth);
  Firebase.reconnectWiFi(true);
}

void fetchScheduledTime() {
  if (Firebase.getString(firebaseData, "/servo/scheduled_time")) {
    scheduledTime = firebaseData.to<String>().substring(0, 5);
    scheduledTime.trim();
    Serial.println("📌 Запланований час: [" + scheduledTime + "]");
  }
}

#endif
