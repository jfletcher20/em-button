#include <Arduino.h>
#include <stdio.h>

#include "emb-hall-filter.h"
#include "emb-logic.h"

#define HALL_PIN 4

Emb emb;

void setup() {

  pinMode(emb.keyData.buttonData.pin, INPUT);
  pinMode(4, INPUT);

  Serial.begin(115200);
  emb.keyboard.begin();

  Serial.println("Searching for connections for ESP32 Keyboard (Keys)...");

}

void loop() {

  getConnectionStatusUpdate(emb);
  keyboardLogic(emb);
  denoise(HALL_PIN);

}