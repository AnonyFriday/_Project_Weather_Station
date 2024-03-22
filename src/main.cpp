// ================================
// == Library & Variable Declaration
// ================================
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include "DHT.h"

// Light Sensor
#define PIN_LIGHT_SENSOR A3

// DHT11
#define PIN_DHT11 A4
DHT dht(PIN_DHT11, DHT11);
struct DHTData
{
  float humidity;
  float temperature;
};

// Rainfall Sensor
#define PIN_RAINFALL_SENSOR A5

// Bluetooth HC-05
// - HC05 TX - Arduino 11 (RX)
// - HC05 RX - Arduino 10 (TX)
SoftwareSerial BTserial(11, 10);

// ================================
// == Function Definition
// ================================
DHTData readDHTData();
int readLightSensor();
int readRainSensor();
void writeDataToBluetoothModule(DHTData dhtData, int lightData, int rainData);

// ================================
// == Main Program
// ================================
void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(PIN_LIGHT_SENSOR, INPUT);
  BTserial.begin(9600);
}

void loop()
{
  int valueLightSensor = readLightSensor();
  DHTData valueDht = readDHTData();
  int valueRainFall = readRainSensor();
  writeDataToBluetoothModule(valueDht, valueLightSensor, valueRainFall);

  // BTserial.print((int)valueLightSensor);
  // BTserial.print(",");
  // BTserial.print(valueDht.humidity);
  // BTserial.print(",");
  // BTserial.print(valueDht.temperature);
  // BTserial.print(",");
  // BTserial.print("No Rain");
  // BTserial.print(";");

  delay(2000);
}

// ================================
// == Function Declaration
// ================================

/**
 * Read Air Temperature and Humid Temperature sensor
 */
DHTData readDHTData()
{
  // Reading Humidity and Temperature
  DHTData dhtData;
  dhtData.humidity = dht.readHumidity();
  dhtData.temperature = dht.readTemperature();
  return dhtData;
}

/**
 * Read Light Sensor
 */
int readLightSensor()
{
  return analogRead(PIN_LIGHT_SENSOR);
}

/**
 * Rad Rainfall Sensor
 */
int readRainSensor()
{
  int sensorMin = 0;
  int sensorMax = 1024;
  int sensorReading = analogRead(PIN_RAINFALL_SENSOR);
  return sensorReading;
}

void writeDataToBluetoothModule(DHTData dhtData, int lightData, int rainData)
{
  // =============
  // Read Light Sensor data
  // =============
  Serial.println("-------------------------------------------");
  // Checking if the value is nan or not
  Serial.print("lightData: ");
  Serial.println(lightData);
  int lightThresholdLower = 500;
  int lightThresholdUpper = 850;
  if (isnan(lightData))
  {
    Serial.println("---Cannot read Light Sensor");
  }
  else
  {
    // Bluetooth printing
    BTserial.print(lightData);
    BTserial.print(",");

    if (lightData <= lightThresholdLower)
    {
      BTserial.print("Too bright,");
    }
    else if (lightData >= lightThresholdUpper)
    {
      BTserial.print("Too dark,");
    }
    else
    {
      BTserial.print("Normal Light,");
    }
  }

  // =============
  // Read DHT data
  // =============
  // Checking if the value is nan or not
  if (isnan(dhtData.humidity) || isnan(dhtData.temperature))
  {
    Serial.println("---Cannot read humidity or temperature from DHT11");
  }
  else
  {
    // Serial Printing
    Serial.print("Humidity: ");
    Serial.print(dhtData.humidity);
    Serial.print(" %, Temperature: ");
    Serial.print(dhtData.temperature);
    Serial.println("°C");

    // Bluetooth printing
    BTserial.print(dhtData.humidity);
    BTserial.print("%,");
    BTserial.print(dhtData.temperature);
    BTserial.print("*C,");
  }

  // =============
  // Rainfall Sensor Data
  // =============
  // Checking if the value is nan or not
  Serial.print("rainData: ");
  Serial.println(rainData);
  int rainThreshold = 800;
  if (isnan(rainData))
  {
    Serial.println("---Cannot read Rainfall Sensor");
  }
  else
  {
    BTserial.print(rainData);
    BTserial.print(",");
    if (rainData <= rainThreshold)
    {
      // Bluetooth printing

      BTserial.print("Is Raining;");
      Serial.println("Is Raining");
    }
    else
    {
      // Bluetooth printing
      BTserial.print("No Rain;");
      Serial.println("No Rain");
    }

    Serial.println("-------------------------------------------");
  }
}
