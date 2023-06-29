#pragma once
#include "emb-hall-filter.h"
#include "data/emb-data.h"
#include <Arduino.h>

#define DELAY 100 // debouncing delay

struct EmbKeyBlock {
  bool keyLock = 0; // lock the key from being pressed temporarily
  bool isConnected = 0; // check if keyboard is connected before attempting to send data
  unsigned long blockTime = 0; // measures time
  unsigned long long int timesPressed = 0; // tracks button press count for stats
} keyBlock;

class KeyboardLogic {

  public:
    static void keyboardLogic(HallFilter filter) {

      int reading = filter.normalize();
      int activation_point = filter.max_normalized * filter.emb->keyData.activation_point;
      bool isPressed = filter.normalized >= activation_point;

      // manages keylock duration (debouncing)
      if(keyBlock.keyLock && isPressed) {

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
      if(filter.emb->keyboard.isConnected() && isPressed && !keyBlock.keyLock) {

        filter.emb->keyboard.write(filter.emb->keyData.keyID);
        // test(emb);
        Serial.print("made it here: ");
        Serial.print(activation_point);

        Serial.println(filter.normalize());

        keyBlock.keyLock = 1;

      }

    }

    static void getConnectionStatusUpdate(Emb& emb) {

      // part to act as keyboard for registering keypresses on the computer
      if(emb.keyboard.isConnected() == true && !emb.connectionStatus.keyboardConnected) {
        Serial.println(String(emb.name) + ": Keyboard connected!");
        emb.connectionStatus.keyboardConnected = 1;
      } if(!emb.keyboard.isConnected() && emb.connectionStatus.keyboardConnected) {
        Serial.println(String(emb.name) + ": Keyboard disconnected! Searching for connections...");
      }

    }

};