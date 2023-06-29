#pragma once
#include "data/emb-data.h"
#include "keyboard-logic.h"
#include "emb-hall-filter.h"

void serialDataOutput(HallFilter filter) {
  if (Serial.available() > 0) {
    String serialData = Serial.readString();
    serialData.trim();
    Serial.println(serialData);
    if (serialData.equals("data")) {
      Serial.print("Times pressed: ");
      Serial.println(keyBlock.timesPressed);
      Serial.print("Activation point: ");
      Serial.println(filter.emb->keyData.activation_point);
      Serial.print("Current state: ");
      Serial.println(filter.getValue());
      Serial.print(filter.normalize());
      Serial.println(filter.pressed() ? " pressed" : " not pressed");
    }
  }
}