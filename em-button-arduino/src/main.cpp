#include "communication/emb-server.h"
#include "logic/emb-hall-filter.h"
#include "logic/emb-logic.h"
#include "data/emb-data.h"
#include <Arduino.h>
#include <stdio.h>

Emb emb;
HallFilter filter(&emb);
EmbServer server;

void setup() {

  pinMode(emb.keyData.hall_sensor, INPUT);
  pinMode(emb.keyData.electromagnet, OUTPUT);
  pinMode(2, OUTPUT); // blue LED

  Serial.begin(115200);
  emb.keyboard.begin();

  Serial.println(emb.manufacturer);
  Serial.print("Searching for connections for ");
  Serial.println(emb.name);

}

void loop() {

  serialDataOutput(filter);
  analogWrite(emb.keyData.electromagnet, 255);
  // if filter has new reading print normalize
  int newReading = filter.normalize();
  if(newReading > -11) {
    Serial.println(newReading);
  }
  getConnectionStatusUpdate(emb);
  keyboardLogic(filter);

}