#include <FastLED.h>
#define LED_PIN     6
#define NUM_LEDS    8
CRGB leds[NUM_LEDS];

int botonpin = 00;
int botonpin = 2;
int I = 0;
int estadoboton = 0;

void setup() {
Serial.begin(9600);
pinMode(botonpin, INPUT);
FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  
  estadoboton = digitalRead(botonpin);
  if (estadoboton == HIGH)
    contador();
}

void contador(){
  (I++);
  if (I>5) I =0;

  switch(I){

    case 0:
    Serial.println("OFF");
    Off();
    
    break;
    
    case 1:
    Serial.println("Caso 1");
    blanco();
    
    break;
    
    case 2:
    Serial.println("Caso 2");
   
    verde();
    
    break;

    case 3:
    Serial.println("Caso 3");
 
    rojo();
    break;

     case 4:
    Serial.println("Caso 4");
 
    amarillo();
    break;


case 5:
    Serial.println("Caso 5");
 
    varios();
    break;
    
  }
  delay(500);
}

/*------------*/

void verde()
{
     for (int i = 0; i < 8 ; i++) {
     leds[i] = CRGB( 0, 255, 0);    
     FastLED.show();
  }
  }


void blanco()
{
     for (int i = 0; i < 8 ; i++) {
     leds[i] = CRGB( 255, 255, 255) ;    
     FastLED.show();
     
  }
  }

void rojo()
{
     for (int i = 0; i < 8 ; i++) {
     leds[i] = CRGB( 255, 0, 0);    
     FastLED.show();
     
  }
  }  

void amarillo()
{
     for (int i = 0; i < 8 ; i++) {
     leds[i] = CRGB( 255, 215, 0);    
     FastLED.show();
     
  }
  }  

void Off()
{
     for (int i = 0; i < 8 ; i++) {
     leds[i] = CRGB( 0, 0, 0);    
     FastLED.show();
     
  }
  }  

void varios()
{
     for (int i = 0; i < 8 ; i++) {
     leds[i] = CRGB( 255, 64, 0);    
     FastLED.show();
     
  }
  }    
