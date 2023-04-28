#pragma once
#include "emb-serial.h"

#define DELAY 100 // debouncing delay
// perhaps a second delay check can be added to emulate key repeat

// placed in struct to prevent confusion when using the header elsewhere
struct EmbKeyBlock {
  bool keyLock = 0; // lock the key from being pressed temporarily
  bool isConnected = 0; // check if keyboard is connected before attempting to send data
  unsigned long blockTime = 0; // measures time
  unsigned long long int timesPressed = 0; // tracks button press count for stats
} keyBlock;

void keyboardLogic(Emb emb) {

  // manages keylock duration (debouncing)
  if(digitalRead(emb.keyData.buttonData.pin) == emb.keyData.buttonData.state.inactive && keyBlock.keyLock) {
    if(keyBlock.blockTime == 0) {
      keyBlock.blockTime = millis();
    } else {
      if(millis() - keyBlock.blockTime >= DELAY)  {
        keyBlock.keyLock = keyBlock.blockTime = 0;
        keyBlock.timesPressed++;
      }
    }
  }

  // manages keypress
  if(emb.keyboard.isConnected() && digitalRead(emb.keyData.buttonData.pin) == emb.keyData.buttonData.state.active && !keyBlock.keyLock) {

    emb.keyboard.write(emb.keyData.keyID);
    test(emb);

    keyBlock.keyLock = 1;

  }

}

void getConnectionStatusUpdate(Emb& emb) {

    // part to act as keyboard for registering keypresses on the computer
    if(emb.keyboard.isConnected() == true && !emb.connectionStatus.keyboardConnected) {
        Serial.print(emb.name);
        Serial.println(": Keyboard connected!");
        emb.connectionStatus.keyboardConnected = 1;
    }

    if(!emb.keyboard.isConnected() && emb.connectionStatus.keyboardConnected) {
        Serial.print(emb.name);
        Serial.println(": Keyboard disconnected! Searching for connections...");
        emb.connectionStatus.keyboardConnected = 0;
    }

}