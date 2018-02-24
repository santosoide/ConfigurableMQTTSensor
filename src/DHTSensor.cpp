#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "DHTSensor.h"
#include "SensorManager.h"

const char PIN_KEY[] = "Pin";

DHTSensor::DHTSensor(const SensorConfiguration &sensorConfig) : _sensorConfig(sensorConfig), dht((sensorConfig[PIN_KEY][0]-48), DHT22) {
  dht.begin();
};

Sensor* createDHTSensor(const SensorConfiguration& sensorConfig) {
  return new DHTSensor(sensorConfig);
}

void DHTSensor::registerSensor() {
  SensorConfigInfo *configInfo = new SensorConfigInfo[1];
  strncpy(configInfo[0].configName, PIN_KEY, sizeof(PIN_KEY));
  Serial.println(configInfo[0].configName);
  configInfo[0].configType = INTEGER;
  SensorManager::getInstance().registerSensor("DHT22", configInfo, 1, &createDHTSensor);
}

SensorConfigurationStruct DHTSensor::getConfig() {
  SensorConfigurationStruct config;
  _sensorConfig.sensorName.toCharArray(config.sensorName, 128);
  _sensorConfig.sensorType.toCharArray(config.sensorType,128);
  return config;
}

int DHTSensor::getSensorCount() {
  return 2;
}

String DHTSensor::getName(int index) {
  if (index == 0) {
    return "temperature";
  } else {
    return "humidity";
  }
}

bool DHTSensor::getValue(const int index, String& value) {
  float f_value = 0;
  if (index == 0) {
    f_value = dht.readTemperature();
  } else {
    f_value = dht.readHumidity();
  }
  if (isnan(f_value)) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }
  value = String(f_value);
  Serial.println("Read: " + value);
  return true;
}
