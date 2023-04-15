#include <Arduino.h>
#include <stdio.h>

#include "emb-logic.h"

Emb emb;

void setup() {

  pinMode(emb.keyData.buttonData.pin, INPUT);
  Serial.begin(115200);
  emb.serial.begin("ESP32 Keyboard (Serial)");

  // TODO: look into following link for possible solutions on device multiconnect:
  // https://community.appinventor.mit.edu/t/ble-esp32-bluetooth-multiconnect-connect-two-mobiles-at-the-same-time/44357/3
  emb.keyboard.begin();

  Serial.println("Searching for connections for ESP32 Keyboard and ESP32 Keyboard (Serial)...");

}

void loop() {

  getConnectionStatusUpdate(emb);
  keyboardLogic(emb);
  serialLogic(emb);

}