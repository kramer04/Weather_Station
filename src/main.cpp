#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT

#include <Arduino.h>
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//Blynk
const char auth[] = "RAZxagbJUrbJVQ2f133jOoYc4AdGPyxA";
//const char auth[] = "m4bgApRKveEsD69WvMiIC7i2z_UbHJRy";
BlynkTimer timer;
void ReadSensors();

//BME280
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;
float temperature, humidity, pressure, altitude, altIGN = 481.38, pnmm;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Connexion...");
  if (bme.begin(0x76))
  {
    Serial.println(F("Capteur BME280 trouvé"));
  }
  else
  {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1)
      delay(10);
  }
  delay(100);
  Blynk.setDeviceName("ESP32-WROOM Upesy");
  Blynk.begin(auth);
  timer.setInterval(5000L, ReadSensors); // read sensor every 5s
  delay(100);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}
BLYNK_WRITE(V1) // Reset
  {
    if (param.asInt() == 1)
    {
      resetFunc();
      delay(5000);
    }
  }
void ReadSensors()
{
  //Read values from the sensor:
  temperature = bme.readTemperature();
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA); //Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  pressure = pressure * exp(9.80665 / (287.05 * (temperature + 273.15 + 0.12 * humidity + 0.0065 * altIGN / 2)) * altIGN);

  Blynk.virtualWrite(V1, pressure);    // write pressure to V1 value display widget
  Blynk.virtualWrite(V2, temperature); // write temperature to V2 value display widget
  Blynk.virtualWrite(V3, humidity);
  //Blynk.virtualWrite(V4, altitude);  // write altimeter to V3 value display widget
  Blynk.virtualWrite(V4, altIGN);
}