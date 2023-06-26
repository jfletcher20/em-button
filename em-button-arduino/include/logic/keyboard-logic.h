#pragma once
#include "data/emb-data.h"
#include <Arduino.h>

bool keylock = 0; // lock the key from being pressed temporarily
bool isConnected = 0; // check if keyboard is connected before attempting to send data
unsigned long blockTime = 0; // debouncing time
unsigned long long int timesPressed = 0; // track how many times the button is pressed for stats

void keyboardLogic(Emb emb) {

  // manages keylock duration
  if(digitalRead(emb.keyData.electromagnet) == emb.keyData.inactive && keylock) {
    if(blockTime == 0) {
      blockTime = millis();
    } else {
      if(millis() - blockTime >= 100)
        keylock = blockTime = 0;
    }
  }

  // manages keypress
  if(emb.keyboard.isConnected() && digitalRead(emb.keyData.electromagnet) >= emb.keyData.active && !keylock) {

    emb.keyboard.write(emb.keyData.keyID);
    
    Serial.print("Keypress detected. ");
    // Serial.println(++timesPressed);
    ++timesPressed;

    keylock = 1;

  }

}