#include "logic/emb-hall-filter.h"
#include "data/emb-data.h"

#include <Arduino.h>
#include <stdio.h>

Emb emb;
HallFilter* filter = new HallFilter(&emb);

void setup() {

  pinMode(4, INPUT);

  Serial.begin(115200);
  Serial.println("Begun...");

  // Fill the window with initial readings

}

void loop() {

  // getConnectionStatusUpdate(emb);
  int val = filter->denoise();
  if(val)
    Serial.println(val);
  // keyboardLogic(emb);

}