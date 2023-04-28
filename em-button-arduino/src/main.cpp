#include "emb-hall-filter.h"
#include "emb-logic.h"
#include "emb-data.h"

#include <Arduino.h>
#include <stdio.h>

Emb emb;

void setup() {

  pinMode(emb.keyData.buttonData.pin, INPUT);
  pinMode(4, INPUT);

  Serial.begin(115200);
  emb.keyboard.begin();

  Serial.println(emb.manufacturer);
  Serial.print("Searching for connections for ");
  Serial.println(emb.name);

}

void loop() {

  serialDataOutput(emb);

  getConnectionStatusUpdate(emb);
  denoise(emb.keyData.hall_sensor);
  keyboardLogic(emb);

}