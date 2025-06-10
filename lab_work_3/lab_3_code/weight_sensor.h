#ifndef WEIGHT_SENSOR_H
#define WEIGHT_SENSOR_H

#include <HX711_ADC.h>
#include <EEPROM.h>
#include "firebase_setup.h"
#include "time_sync.h"

const int HX711_dout = 47;
const int HX711_sck = 45;
HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;

bool weightMeasurementInProgress = false;
unsigned long weightStartTime = 0;
float weightSum = 0;
int weightCount = 0;

void setupWeightSensor() {
  EEPROM.begin(512);
  LoadCell.begin();
  LoadCell.start(5000, true);
  float savedCalibrationValue;
  EEPROM.get(calVal_eepromAdress, savedCalibrationValue);
  LoadCell.setCalFactor(savedCalibrationValue > 0 ? savedCalibrationValue : 115.0);
  while (!LoadCell.update());
  LoadCell.tare();
}

void checkWeightMeasurement() {
  int activate;
  if (Firebase.getInt(firebaseData, "/weight_sensor/weight/activate")) {
    activate = firebaseData.to<int>();
    if (activate == 1 && !weightMeasurementInProgress) {
      weightMeasurementInProgress = true;
      weightStartTime = millis();
      weightSum = 0;
      weightCount = 0;
      Serial.println("⚖️ Початок зважування протягом 10 секунд...");
    }
  }

  if (weightMeasurementInProgress) {
    if (LoadCell.update()) {
      float value = LoadCell.getData();
      weightSum += value;
      weightCount++;
    }

    if (millis() - weightStartTime > 10000) {
      float averageWeight = weightCount > 0 ? weightSum / weightCount : 0;
      String timeNow = timeClient.getFormattedTime();
      String path = "/weight_sensor/weight_logs/" + String(millis());

      Firebase.setFloat(firebaseData, "/weight_sensor/weight/last_measurement", averageWeight);
      Firebase.setFloat(firebaseData, path + "/weight", averageWeight);
      Firebase.setString(firebaseData, path + "/time", timeNow);
      Firebase.setInt(firebaseData, "/weight_sensor/weight/activate", 0);

      Serial.println("✅ Середня вага: " + String(averageWeight));
      weightMeasurementInProgress = false;
    }
  }
}

#endif
