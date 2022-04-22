#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FastLED.h>
#include <WiFiUdp.h>
#include <TM1637Display.h>
#include <EasyBuzzer.h>
#define LED_PIN 13 // D7
#define NUM_LEDS 8 // Numero de led ocupados en tira led WS2812B
#define CLK 4      // D2                  //  puertos Gpio:
#define DIO 0      // D3
CRGB leds[NUM_LEDS];
TM1637Display display = TM1637Display(CLK, DIO); // Conexion Display CLK, DIO en ESP8266 en puertos Gpio

const char *ssid = "Casa 1";       // <----escriba  nombre de su router
const char *password = "maxi1987"; // <------escriba  el password
const char* host = "RelojMulti";

const long utcOffsetInSeconds = -14400; // varicion de segundos depende a cada pais

// Defina Server NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.shoa.cl", utcOffsetInSeconds);
int botonpin = 15;   // D8  conexion Pulsador a pin
int I = 0;           // Variable inicializada con valor 0
int estadoboton = 0; // Boton con Valor inicial en 0
int pinSound = 5;    // D1   conexion Buzzer positivo a pin

void setup()
{
  Serial.begin(115200);

  // Limpia el display:
  display.clear();
  WiFi.begin(ssid, password);

  ///// Conexion a red WIFI ///////
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conexion exitosa");

   WIFIII(); // metodo OTA
  
  ////////////////////////////////
  timeClient.begin();

  EasyBuzzer.setPin(5);
  pinMode(botonpin, INPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop()
{
  ArduinoOTA.handle();
  Horaa();
  estadoboton = digitalRead(botonpin);
  if (estadoboton == HIGH)
    contador();
}

/*------------*/
void contador()
{
  (I++);
  if (I > 5)
    I = 0;

  switch (I)
  {

  case 0:
    Serial.println("OFF");
    for (int i = 0; i < 8; i++)
    {
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
    }
    break;

  case 1:
    Serial.println("Caso 1");
    for (int i = 0; i < 8; i++)
    {
      leds[i] = CRGB(255, 255, 255); // Blanco
      FastLED.show();
    }
    break;

  case 2:
    Serial.println("Caso 2");

    for (int i = 0; i < 8; i++)
    {
      leds[i] = CRGB(255, 215, 0); //amarillo
      FastLED.show();
    }

    break;

  case 3:
    Serial.println("Caso 3");

    for (int i = 0; i < 8; i++)
    {
      leds[i] = CRGB(255, 0, 0); //rojo
      FastLED.show();
    }
    break;

  case 4:
    Serial.println("Caso 4");
    for (int i = 0; i < 8; i++)
    {
      leds[i] = CRGB(0,0,128); //azul
      FastLED.show();
    }

    break;

  case 5:
    Serial.println("Caso 5");
  
     
  for (int i = 0; i < 8; i++)
    {
      leds[i] = CRGB(0, 255, 0); //verde
      FastLED.show();
    }
    
    break;
  }
  delay(500);
}

/*------------*/
void Horaa()
{
  int A, B, C;

  timeClient.update();

  display.setBrightness(7); // Establecer el brillo:

  A = timeClient.getHours() * 100 + timeClient.getMinutes();
  B = timeClient.getSeconds();
  C = timeClient.getMinutes();
  if ((B % 2) == 0)
  {
    display.showNumberDecEx(A, 0b01000000, false, 4, 0);
  }
  else
  {
    display.showNumberDecEx(A, 0b00000000, false, 4, 0);
  }

  if ((timeClient.getHours() == 8 && C == 20)) // alarma
  {
    c_pacman_c();
  }

  EasyBuzzer.update();
  if (B == 00 && C == 00)
  {
    delay(1000);
    EasyBuzzer.beep(2000, 100, 100, 2, 300, 1);
    EasyBuzzer.stopBeep();
  }

  while (timeClient.getHours() < 01 && timeClient.getMinutes() < 59) // muestra todos los 0 en display son estos el display solo puesta 1 0
  {

    delay(250);

    estadoboton = digitalRead(botonpin);
    if (estadoboton == HIGH)
      contador();
    timeClient.update();

    display.setBrightness(7); // Establecer el brillo:

    A = timeClient.getHours() * 100 + timeClient.getMinutes();
    B = timeClient.getSeconds();
    C = timeClient.getMinutes();

    if ((B % 2) == 0)
    {
      display.showNumberDecEx(A, 0b01000000, true, 4, 0);
    }
    else
    {
      display.showNumberDecEx(A, 0b00000000, true, 4, 0);
    }
  }
}
/*------------*/

void WIFIII()
{
 ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
   ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
  
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  }

/*------------*/
void c_pacman_c()
{
  tone(pinSound, 493, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 622, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  delay(141.5095);
  tone(pinSound, 622, 212.26425);
  delay(212.26425);
  delay(70.75475);
  tone(pinSound, 523, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 1046, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 783, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 659, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 1046, 70.75475);
  delay(70.75475);
  tone(pinSound, 783, 70.75475);
  delay(70.75475);
  delay(141.5095);
  tone(pinSound, 659, 212.26425);
  delay(212.26425);
  delay(70.75475);
  tone(pinSound, 493, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 622, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  delay(141.5095);
  tone(pinSound, 622, 212.26425);
  delay(212.26425);
  delay(70.75475);
  tone(pinSound, 622, 70.75475);
  delay(70.75475);
  tone(pinSound, 659, 70.75475);
  delay(70.75475);
  tone(pinSound, 698, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 698, 70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  tone(pinSound, 783, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 783, 70.75475);
  delay(70.75475);
  tone(pinSound, 830, 70.75475);
  delay(70.75475);
  tone(pinSound, 880, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 141.5095);
  delay(141.5095);
  delay(800);
  tone(pinSound, 493, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 622, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  delay(141.5095);
  tone(pinSound, 622, 212.26425);
  delay(212.26425);
  delay(70.75475);
  tone(pinSound, 523, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 1046, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 783, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 659, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 1046, 70.75475);
  delay(70.75475);
  tone(pinSound, 783, 70.75475);
  delay(70.75475);
  delay(141.5095);
  tone(pinSound, 659, 212.26425);
  delay(212.26425);
  delay(70.75475);
  tone(pinSound, 493, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 622, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  delay(141.5095);
  tone(pinSound, 622, 212.26425);
  delay(212.26425);
  delay(70.75475);
  tone(pinSound, 622, 70.75475);
  delay(70.75475);
  tone(pinSound, 659, 70.75475);
  delay(70.75475);
  tone(pinSound, 698, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 698, 70.75475);
  delay(70.75475);
  tone(pinSound, 739, 70.75475);
  delay(70.75475);
  tone(pinSound, 783, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 783, 70.75475);
  delay(70.75475);
  tone(pinSound, 830, 70.75475);
  delay(70.75475);
  tone(pinSound, 880, 70.75475);
  delay(70.75475);
  delay(70.75475);
  tone(pinSound, 987, 141.5095);
  delay(141.5095);
  delay(800);
}
