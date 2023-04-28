#pragma once
#include "emb-data.h"
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

void serialDataOutput(Emb emb) {
  if(Serial.available() > 0) {
    String serialData = Serial.readString();
    serialData.trim();
    Serial.println(serialData);
    if(serialData.equals("data")) {
      Serial.print("Times pressed: ");
      Serial.println(keyBlock.timesPressed);
      Serial.print("Activation point: ");
      Serial.println(emb.keyData.activation_point);
      Serial.print("Current state: ");
      Serial.print(analogRead(emb.keyData.hall_sensor));
      Serial.println(analogRead(emb.keyData.hall_sensor) <= emb.keyData.activation_point ? " (1) " : " (0) ");
    }
  }
}

void keyboardLogic(Emb emb) {

  // manages keylock duration (debouncing)
  if(keyBlock.keyLock && degree(emb) < 0.4) {
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
  if(emb.keyboard.isConnected() && degree(emb) > 0.4 && !keyBlock.keyLock) {

    emb.keyboard.write(emb.keyData.keyID);
    // test(emb);
    Serial.println(degree(emb));

    keyBlock.keyLock = 1;

  }

}

void getConnectionStatusUpdate(Emb& emb) {

  // part to act as keyboard for registering keypresses on the computer
  if(emb.keyboard.isConnected() == true && !emb.connectionStatus.keyboardConnected) {
    Serial.println(String(emb.name) + ": Keyboard connected!");
    emb.connectionStatus.keyboardConnected = 1;
  } if(!emb.keyboard.isConnected() && emb.connectionStatus.keyboardConnected) {
    Serial.println(String(emb.name) + ": Keyboard disconnected! Searching for connections...");
  }

}