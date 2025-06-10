#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "firebase_setup.h"
#include "time_sync.h"

const int trigPin = 1;
const int echoPin = 2;
bool obstacleDetected = false;
bool obstacleWasLongEnough = false;
unsigned long obstacleStartTime = 0;

void setupUltrasonic() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void logObstacle(int durationSeconds) {
  String timestamp = timeClient.getFormattedTime();
  String path = "/obstacle_logs/" + String(millis());

  Firebase.setString(firebaseData, path + "/time", timestamp);
  Firebase.setInt(firebaseData, path + "/duration", durationSeconds);

  if (durationSeconds >= 5) {
    Firebase.setBool(firebaseData, "/notifications/animal_detected/active", true);
    Firebase.setString(firebaseData, "/notifications/animal_detected/time", timestamp);
    Firebase.setString(firebaseData, "/notifications/animal_detected/message", "🐾 Ваша тварина прийшла! Хочете глянути");
  }

  Serial.println("🧱 Перешкода записана у Firebase. Тривалість: " + String(durationSeconds) + " с");
}

void checkObstacle() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  oled_dist = distance;

  Serial.print("📏 Відстань: ");
  Serial.print(distance);
  Serial.println(" см");

  if (distance > 0 && distance <= 30.0) {
    if (!obstacleDetected) {
      obstacleStartTime = millis();
      obstacleDetected = true;
      obstacleWasLongEnough = false;
    } else if ((millis() - obstacleStartTime >= 5000) && !obstacleWasLongEnough) {
      obstacleWasLongEnough = true;
      logObstacle((millis() - obstacleStartTime) / 1000);
    }
  } else {
    obstacleDetected = false;
    obstacleWasLongEnough = false;
  }
}

#endif
