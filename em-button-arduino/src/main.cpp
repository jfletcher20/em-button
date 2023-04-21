#include <Arduino.h>
#include <stdio.h>

#include "emb-logic.h"

Emb emb;

void setup() {

  pinMode(emb.keyData.buttonData.pin, INPUT);

  Serial.begin(115200);
  emb.keyboard.begin();

  Serial.println("Searching for connections for ESP32 Keyboard (Keys)...");

}

void loop() {

  getConnectionStatusUpdate(emb);
  keyboardLogic(emb);
  // serialLogic(emb);

}