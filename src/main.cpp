// ================================
// == Library & Variable Declaration
// ================================
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include "DHT.h"

// DHT11
#define PIN_DHT11 A4
DHT dht(PIN_DHT11, DHT11);
struct DHTData
{
  float humidity;
  float temperature;
};

// Light Sensor
#define PIN_LIGHT_SENSOR A3
int valueLightSensor;

// Rainfall Sensor
#define PIN_RAINFALL_SENSOR A5
int valueRainFall;

// Bluetooth HC-05
#define PIN_TXD 3
#define PIN_RXD 2
SoftwareSerial hc05(PIN_RXD, PIN_TXD); // RXD | TXD

// ================================
// == Function Definition
// ================================
DHTData readDHTData();
int readLightSensor();
int readRainSensor();

// ================================
// == Main Program
// ================================
void setup()
{
  Serial.begin(9600);

  // DHT11
  dht.begin();

  // MS-CDS05
  pinMode(PIN_LIGHT_SENSOR, INPUT);

  // HC-05
  hc05.begin(9600);

  Serial.println(hc05.available());

  Serial.println("--The bluetooth gates are open.\nConnect to HC-05 from any other bluetooth device with 1234 as pairing key!.--\n");
}

void loop()
{
  // =============
  // Read DHT data
  // =============
  // DHTData valueDht = readDHTData();

  // =============
  // Read Light Sensor data
  // =============
  // valueLightSensor = readLightSensor();

  // =============
  // Rainfall Sensor Data
  // =============
  // valueRainFall = readRainSensor();

  // =============
  // Bluetooth Module
  // =============
  // if (hc05.available())
  // {
  //   // Feed data from bluetooth to terminal
  //   Serial.write(valueLightSensor);

  //   // Feed data from terminal to bluetooth
  // }

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

  // Checking if the value is nan or not
  if (isnan(dhtData.humidity) || isnan(dhtData.temperature))
  {
    Serial.println("Cannot read humidity or temperature from DHT11");
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(dhtData.humidity);
    Serial.print(" %, Temperature: ");
    Serial.print(dhtData.temperature);
    Serial.println(" °C");
  }

  return dhtData;
}

/**
 * Read Light Sensor
 */
int readLightSensor()
{
  // Checking if the value is nan or not
  if (isnan(valueLightSensor))
  {
    Serial.println("Cannot read Light Sensor");
  }
  else
  {
    Serial.print("Light Value Sensor: ");
    Serial.println(valueLightSensor);
  }

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

  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);

  // Checking if the value is nan or not
  if (isnan(valueRainFall))
  {
    Serial.println("Cannot read Rainfall Sensor");
  }
  else
  {
    Serial.println("Rain Fall sensor");
    Serial.println(sensorReading);
    switch (valueRainFall)
    {
    case 0:
      Serial.println("Rain Warning");
      break;
    case 1:
      Serial.println("Not Raining");
      break;
    case 2:
      Serial.println("Light Rain");
      break;
    case 3:
      Serial.println("Moderate to Heavy Rain");
      break;
    }
  }
  return range;
}