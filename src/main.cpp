#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TFT_eSPI.h>
#include <time.h>

//Fonctions
void (*resetFunc)(void) = 0;
void temps();
void tft_capteurs();
void ReadSensors();
String get_encryption_type(wifi_auth_mode_t encryptionType);

const char *auth = "QbuWGqrS9LD3XFkChXXbUyidgTqTDAm7";
String ssid;
//const char *id = "JASON-2.4G";
const char *mdp = "CodedeCryptagedeSecurite";
const int led0 = 32;

uint16_t moment;

BlynkTimer timer;

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;
float temperature, humidity, pressure, altitude, altIGN = 481.38, pnmm;
int x = 180, y = 0, offst = 30;

TFT_eSPI tft = TFT_eSPI();
#define TFT_GREY 0x3186 // New colour
#define TFT_TOMATE 0xD942
#define TFT_JADE 0x8752
#define TFT_ANIS 0x9F4A
#define TFT_AZUR 0x769E
#define TFT_GRIS 0xCE79
#define TFT_GREGE 0xBD73
#define TFT_CELADON 0x8532
#define TFT_NOISETTE 0x92A5
#define TFT_MENTHE 0x57D1
#define TFT_SAUGE 0x6CEE
#define VERT_PRINTEMPS 0x07EF
#define CHARTREUSE 0xC7A6
#define font 1

time_t heure;
const int decalage = 0; // la valeur dépend du fuseau horaire. Normalement 2 pour la France.
const int daylight = 2; // décalage de l'heure d'été

void setup()
{
  Serial.begin(115200);
  delay(100);
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLUE);
  tft.setCursor(0, 0, font);
  tft.setTextColor(TFT_JADE);
  tft.println("Initialise l\x27\x82\x63ran");
  delay(100);

  if (bme.begin(0x76))
  {
    Serial.println(F("Capteur BME280 trouvé"));
    tft.println("Capteur BME280 trouv\x82");
  }
  else
  {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    tft.println("Capteur BME280 non trouv\x82");
    tft.println("Arr\x88t programme");
    while (1)
      delay(10);
  }

  WiFi.mode(WIFI_STA);

  //SCANNE LES RESEAUX WIFI

  Serial.println("uPesy WiFi Scan");
  tft.println("WiFi scan");
  Serial.println("[*] Scanning WiFi network");
  tft.print("[*] Scanne WiFi:");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("[*] Scan done");
  tft.println("Ok! fait");
  if (n == 0)
  {
    Serial.println("[-] No WiFi networks found");
    tft.println("Pas de WiFi");
  }
  else
  {
    Serial.println((String) + n + " WiFi networks found\n");
    tft.println((String) + n + " WiFi trouv\x82s");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID, RSSI and WiFi Encryption chaque network trouvé
      Serial.print(i + 1);
      tft.print(i + 1);
      Serial.print(": ");
      tft.print(" : ");
      Serial.print(WiFi.SSID(i));
      tft.print(WiFi.SSID(i));
      Serial.print(" (");
      tft.print(" (");
      Serial.print(WiFi.RSSI(i));
      tft.print(WiFi.RSSI(i));
      Serial.print(" dB) [");
      tft.println(" db)");
      tft.print(" [");
      Serial.print(get_encryption_type(WiFi.encryptionType(i)));
      tft.print(get_encryption_type(WiFi.encryptionType(i)));
      Serial.println("]");
      tft.println("]");
      delay(10);
      if (WiFi.SSID(i) == "JASON-2.4G" || WiFi.SSID(i) == "Freebox-905C49_2.4G")
      {
        ssid = WiFi.SSID(i);
        Serial.println((String) "Se connecte à " + ssid + "\n");
        tft.println((String) "Se connecte \x85 " + ssid);
        break;
      }
    }
  }
  Serial.println("");
  char *id = const_cast<char *>(ssid.c_str()); //string to char
  //********************************
  WiFi.begin(id, mdp);
  uint32_t startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000)
  {
    delay(1000);
    Serial.print('.');
    tft.print(".");
  }
  Serial.println();
  tft.println();
  // Check connection
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("WiFi connecté l'adresse IP : "));
    tft.println("WiFi connect\x82 \x85");
    Serial.println(WiFi.localIP());
    tft.println(WiFi.localIP());
  }
  else
  {
    Serial.print(F("Connexion WiFi vers : "));
    tft.println("Connexion WiFi vers");
    Serial.print(id);
    tft.print(id);
    Serial.println(F(" Echec !"));
    tft.println(" Echec");
    WiFi.disconnect();
    while (1)
      delay(10);
  }
  delay(500);
  //********************************
  configTime(decalage * 3600, daylight * 3600, "fr.pool.ntp.org"); //serveurs canadiens ca.pool.ntp.org
  // en Europe, europe.pool.ntp.org ou fr.pool.ntp.org

  Serial.println("Attente date et heure");
  tft.println("Attente date et heure");
  while (time(nullptr) <= 100000)
  {
    Serial.print(".");
    tft.print(".");
    delay(1000);
  }
  Serial.println();
  tft.println();
  //********************************
  Blynk.config(auth);
  Blynk.disconnect();
  delay(1000);
  Blynk.connect();
  tft.println("Se connecte \x85 Blynk");
  startTime = millis();
  while (millis() - startTime < 30000UL)
  {
    Serial.print(".");
    tft.print(".");
    if (Blynk.connected())
      break;
  }
  Serial.println();
  tft.println();
  if (Blynk.connected())
  {
    Serial.println(F("BLYNK service connected..."));
    tft.println("Blynk connect\x82...");
  }
  else
  {
    Serial.println(F("TIMEOUT ERROR ! Déconnexion..."));
    Blynk.disconnect();
  }

  //Blynk.begin(auth, id, mdp);
  //tft.println("Se connecte \x85 Blynk");
  timer.setInterval(5000L, ReadSensors); // read sensor every 5s

  delay(3000);
  tft.fillScreen(TFT_GREY);
  tft.setTextColor(TFT_AZUR, TFT_GREY); // Adding a background colour erases previous text automatically
  tft.setCursor(0, 0, font);
  tft.print("Temp\x82rature : ");
  tft.setCursor(0, 30, font);
  tft.print("Humidit\x82 : ");
  tft.setCursor(0, 60, font);
  tft.print("Pression : ");
  tft.setCursor(0, 90, font);
  tft.print("Altitude : ");
  tft.setCursor(0, 120);
  tft.print("Alt. corrig\x82\x65 :");
  tft.setCursor(0, 150);
  tft.print("Pression NMM :");

  tft.drawLine(0, 180, 320, 180, TFT_TOMATE);
  //tft.setCursor(40, 192);
  //tft.setTextColor(TFT_SAUGE, TFT_GREY);
  //tft.setTextFont(2);
  //tft.print("ESP32-Wroom Upesy");

  //tft.setTextColor(VERT_PRINTEMPS, TFT_GREY);
}
void loop()
{
  Blynk.run();
  timer.run();
}

void temps()
{
  tft.setCursor(10, 192);
  tft.setTextFont(2);
  tft.setTextColor(CHARTREUSE, TFT_GREY);

  time(&heure);
  struct tm *timeinfo;
  String contenu;
  timeinfo = localtime(&heure);
  moment = timeinfo->tm_mday;
  contenu = ((moment < 10) ? "0" + String(moment) : String(moment)) + "-";
  moment = timeinfo->tm_mon + 1;
  contenu += ((moment < 10) ? "0" + String(moment) : String(moment)) + "-" + String(timeinfo->tm_year + 1900) + " | ";
  moment = timeinfo->tm_hour;
  contenu += ((moment < 10) ? "0" + String(moment) : String(moment)) + ":";
  moment = timeinfo->tm_min;
  contenu += ((moment < 10) ? "0" + String(moment) : String(moment)) + ":";
  moment = timeinfo->tm_sec;
  contenu += (moment < 10) ? "0" + String(moment) : String(moment);

  tft.print(contenu);
  Serial.println(contenu);
}

BLYNK_WRITE(V0) // Reset
{
  if (param.asInt() == 1)
  {
    resetFunc();
    delay(5000);
  }
}

void tft_capteurs()
{
  tft.setTextColor(VERT_PRINTEMPS, TFT_GREY);
  tft.setCursor(x, y, font);
  //temperature = bme.readTemperature();
  tft.print(temperature);
  tft.print(" C");
  y = offst;
  tft.setCursor(x, y, font);
  //humidity = bme.readHumidity();
  tft.print(humidity);
  tft.print(" %");
  y += offst;
  tft.setCursor(x, y, font);
  //pressure = bme.readPressure() / 100.0F;
  tft.print(pressure);
  tft.print(" hpa");
  y += offst;
  tft.setCursor(x, y, font);
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  tft.print(altitude);
  tft.print(" m");
  y += offst;
  tft.setCursor(x, y);
  tft.print(altIGN);
  tft.print(" m");
  y += offst;
  tft.setCursor(x, y);
  //pnmm = pressure * exp(9.80665 / (287.05 * (temperature + 273.15 + 0.12 * humidity + 0.0065 * altIGN / 2)) * altIGN);
  tft.print(pnmm);
  tft.print(" hpa");
  y = 0;
}

void ReadSensors()
{
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  humidity = bme.readHumidity();
  pnmm = pressure * exp(9.80665 / (287.05 * (temperature + 273.15 + 0.12 * humidity + 0.0065 * altIGN / 2)) * altIGN);

  Blynk.virtualWrite(V1, pnmm);        // write pressure to V1 value display widget
  Blynk.virtualWrite(V2, temperature); // write temperature to V2 value display widget
  Blynk.virtualWrite(V3, humidity);
  //Blynk.virtualWrite(V4, altitude);  // write altimeter to V3 value display widget
  Blynk.virtualWrite(V4, altIGN);

  tft_capteurs();
  temps();
}

String get_encryption_type(wifi_auth_mode_t encryptionType)
{
  String encryptage;
  switch (encryptionType)
  {
  case (WIFI_AUTH_OPEN):
    encryptage = "Open";
    break;
  case (WIFI_AUTH_WEP):
    encryptage = "WEP";
    break;
  case (WIFI_AUTH_WPA_PSK):
    encryptage = "WPA_PSK";
    break;
  case (WIFI_AUTH_WPA2_PSK):
    encryptage = "WPA2_PSK";
    break;
  case (WIFI_AUTH_WPA_WPA2_PSK):
    encryptage = "WPA_WPA2_PSK";
  case (WIFI_AUTH_WPA2_ENTERPRISE):
    encryptage = "WPA2_ENTERPRISE";
    break;
  default:
    encryptage = "Nothing found";
    break;
  }
  return encryptage;
}
/*
# calcul du point de rosée  (formule de Heinrich Gustav Magnus-Tetens)
    alpha = math.log(humidite / 100.0) + (17.27 * temperature) / (237.3 + temperature)
    rosee = (237.3 * alpha) / (17.27 - alpha)
*/