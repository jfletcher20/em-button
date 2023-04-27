#include "emb-hall-filter.h"
#include "emb-logic.h"
#include "emb-data.h"

#include <Arduino.h>
#include <stdio.h>

#define HALL_PIN 4

Emb emb;

void setup() {

  pinMode(emb.keyData.buttonData.pin, INPUT);
  pinMode(4, INPUT);

  // Serial.begin(115200)
  emb.serial.begin(115200);
  emb.keyboard.begin();

  emb.serial.println("Searching for connections for ESP32 Keyboard (Keys)...");

}

void loop() {

  getConnectionStatusUpdate(emb);
  keyboardLogic(emb);
  denoise(HALL_PIN);

}