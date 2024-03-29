#pragma once
#include "emb-hall-filter.h"
#include "data/emb-data.h"
#include "communication/stp-db-connection.h"
#include <Arduino.h>

#define DELAY 100 // debouncing delay

struct EmbKeyBlock {
  bool keyLock = 0; // lock the key from being pressed temporarily
  bool isConnected = 0; // check if keyboard is connected before attempting to send data
  unsigned long blockTime = 0; // measures time
  unsigned long long int timesPressed = 0; // tracks button press count for stats
} keyBlock;

int _lastKeyPressed = -1;
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
        if(reading >= activation_point - filter.max_normalized * 0.1) {
          indexOfPressed = i; // the next might also meet criteria, so continue until it fails
        }
      }

      if(indexOfPressed == -1 && _lastKeyPressed == -1) return false; // if no action is pressed, return false

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
      if(filter.emb->connectionStatus.keyboardConnected && !keyBlock.keyLock && filter.emb->keyData.actions[indexOfPressed].keyId != -1 && indexOfPressed != 120 && filter.emb->keyData.actions[indexOfPressed].actionId != -1) {

        filter.emb->keyboard.clearWriteError();
        filter.emb->keyboard.releaseAll();
        filter.emb->keyboard.write(filter.emb->keyData.actions[indexOfPressed].keyId);

        String keys[5] = {
          "keyId",
          "actionId",
          "activation_point",
          "times_pressed",
          "time_when_pressed"
        };
        String values[5] = {
          String(filter.emb->keyData.actions[indexOfPressed].keyId),
          String(filter.emb->keyData.actions[indexOfPressed].actionId),
          String(filter.emb->keyData.actions[indexOfPressed].activation_point),
          String(keyBlock.timesPressed),
          String(millis())
        };
        
        Serial.println(STP::createResponse(100, "Pressed key " + String(indexOfPressed), keys, values, 5));
        keyBlock.keyLock = 1;

        _lastKeyPressed = indexOfPressed;

      } else if (filter.emb->connectionStatus.keyboardConnected && indexOfPressed != _lastKeyPressed) {
        
        String keys[5] = {
          "keyId",
          "actionId",
          "activation_point",
          "times_pressed",
          "time_when_pressed"
        };
        String values[5] = {
          String(-1),
          String(-1),
          String(-1),
          String(keyBlock.timesPressed),
          String(millis())
        };

        Serial.println(STP::createResponse(100, "Pressed key " + String(indexOfPressed), keys, values, 5));

        _lastKeyPressed = indexOfPressed;

      }

      return timesPressedIncreased || indexOfPressed != _lastKeyPressed;

    }

    static bool getConnectionStatusUpdate(Emb& emb) {
      bool registeredChange = false;
      // part to act as keyboard for registering keypresses on the computer
      if(emb.keyboard.isConnected() == true && !emb.connectionStatus.keyboardConnected) {
        Serial.println("STP1.0{\"status\":300,\"data\":{\"message\":\"Connected!\",\"ble_connected\":true}}");
        emb.connectionStatus.keyboardConnected = 1;
        registeredChange = true;
      } if(!emb.keyboard.isConnected() && emb.connectionStatus.keyboardConnected) {
        Serial.print("STP1.0{\"status\":300,\"data\":{\"message\":\"Disconnected! Searching for connections for ");
        Serial.print(emb.name);
        Serial.println("\",\"ble_connected\":false}}");
        emb.connectionStatus.keyboardConnected = 0;
        registeredChange = true;
      }
      return registeredChange;
    }

};