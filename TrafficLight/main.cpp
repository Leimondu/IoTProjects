// Made by: Raymond Abayon and Andrew Wu
#include <Arduino.h>
#include <Adafruit_CAP1188.h>
#include <Wire.h>
#include <SPI.h>

#define GREEN_LED 27
#define YELLOW_LED 26
#define RED_LED 25
#define BUZZER_PIN 33

Adafruit_CAP1188 cap = Adafruit_CAP1188();
//Function Initialization
void greenLight();
void yellowLight();
void redYellowLight();
void redLight();

void setup() {
  //pins initialization and light starts
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("Pins Initialized.");

  //init cap1188
  if(!cap.begin()) {
    Serial.println("CAP1188 not found");
  }
  else {
    Serial.println("CAP1188 Ready");
  }

  Serial.println("Starting Lights");
  redLight();
  redYellowLight();
  Serial.println("Light init finished.");


}

void loop() {
  //checking touched CAP1188
  uint8_t touched = cap.touched();

  //if no touch then green light
  if(touched == 0) {
    greenLight();
    Serial.println("button not pressed");
  }
  //else start the red cycle
  else {
    yellowLight();
    redLight();
    redYellowLight();
  }
}
//Green light function
void greenLight() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  Serial.println("GOING GREEN");
  tone(BUZZER_PIN, 432);
  delay(500);
  noTone(BUZZER_PIN);
  delay(2000);
}
//yellow light function
void yellowLight() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    Serial.println("GOING YELOOW");
    delay(1000);
  }  
}
//red-yellow ligt function
void redYellowLight() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    Serial.println("GOING RED-YELLOW");
    delay(1000);
  }
}
//red light function
void redLight() {
  for (int i = 0; i < 20; i++) {
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    Serial.println("GOING RED");
    tone(BUZZER_PIN, 432);
    delay(250);
    noTone(BUZZER_PIN);
    delay(250);
  }
}
