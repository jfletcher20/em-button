#include "emb-serial.h"

bool keylock = 0; // lock the key from being pressed temporarily
bool isConnected = 0; // check if keyboard is connected before attempting to send data
unsigned long blockTime = 0; // debouncing time
unsigned long long int timesPressed = 0; // track how many times the button is pressed for stats

void keyboardLogic(Emb emb) {

  // manages keylock duration (debouncing)
  if(digitalRead(emb.keyData.buttonData.pin) == emb.keyData.buttonData.state.inactive && keylock) {
    if(blockTime == 0) {
      blockTime = millis();
    } else {
      if(millis() - blockTime >= 100)
        keylock = blockTime = 0;
    }
  }

  // manages keypress
  if(emb.keyboard.isConnected() && digitalRead(emb.keyData.buttonData.pin) == emb.keyData.buttonData.state.active && !keylock) {

    emb.keyboard.write(emb.keyData.keyID);
    test();

    keylock = 1;

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