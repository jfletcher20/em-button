#pragma once
#include "emb-hall-filter.h"
#include "data/emb-data.h"
#include "emb-logic.h"
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
    static bool keyboardLogic(HallFilter& filter) {

      bool timesPressedIncreased = false;
      int reading = filter.normalize();

      int activation_point, indexOfPressed = -1;
      for(int i = 0; i < 3 && filter.emb->keyData.actions[i].actionId != -1; i++) {
        // get the activation point for the current action
        activation_point = filter.max_normalized * filter.emb->keyData.actions[i].activation_point;
        // if the reading meets it, the index is the current one
        if(reading >= activation_point) {
          indexOfPressed = i; // the next might also meet criteria, so continue until it fails
        }
      }

      if(indexOfPressed == -1) return false; // if no action is pressed, return false

      // manages keylock duration (debouncing)
      if(keyBlock.keyLock) {
        if(keyBlock.blockTime == 0) {
          keyBlock.blockTime = millis();
        } else {
          if(millis() - keyBlock.blockTime >= DELAY) {
            keyBlock.keyLock = keyBlock.blockTime = 0;
            keyBlock.timesPressed++;
            timesPressedIncreased = true;
          }
        }
      }

      // manages keypress
      if(filter.emb->connectionStatus.keyboardConnected && !keyBlock.keyLock) {

        filter.emb->keyboard.clearWriteError();
        filter.emb->keyboard.releaseAll();
        filter.emb->keyboard.write(filter.emb->keyData.actions[indexOfPressed].keyId);

        Serial.println("✓");
        keyBlock.keyLock = 1;

      }


      return timesPressedIncreased;

    }

    static bool getConnectionStatusUpdate(Emb& emb) {
      bool registeredChange = false;
      // part to act as keyboard for registering keypresses on the computer
      if(emb.keyboard.isConnected() == true && !emb.connectionStatus.keyboardConnected) {
        Serial.println(String(emb.name) + ": Keyboard connected!");
        emb.connectionStatus.keyboardConnected = 1;
        registeredChange = true;
      } if(!emb.keyboard.isConnected() && emb.connectionStatus.keyboardConnected) {
        Serial.println(String(emb.name) + ": Keyboard disconnected! Searching for connections...");
        emb.connectionStatus.keyboardConnected = 0;
        registeredChange = true;
      }
      return registeredChange;
    }

};