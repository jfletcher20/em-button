#include <BluetoothSerial.h>
#include <BleKeyboard.h>  
#include <Arduino.h>
#include <stdio.h>
#include "String.h"

#define BUTTON_PIN 2

BleKeyboard bleKeyboard;
BluetoothSerial SerialBT;

void setup() {

  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32 Keyboard (Serial)");

  // TODO: look into following link for possible solutions on device multiconnect:
  // https://community.appinventor.mit.edu/t/ble-esp32-bluetooth-multiconnect-connect-two-mobiles-at-the-same-time/44357/3
  bleKeyboard.begin();

  Serial.println("Searching for connections...");

}

void connectionStatus();

bool keylock = 0;
bool isConnected = 0;
unsigned long blockTime = 0;

void loop() {

  if (SerialBT.available() > 0) {
    String c = SerialBT.readString();
    c.trim();
    Serial.println("Received: " + c);
  }


  connectionStatus();

  if(digitalRead(BUTTON_PIN) == LOW && keylock) {
    if(blockTime == 0) {
      blockTime = millis();
    } else {
      if(millis() - blockTime >= 100)
        keylock = blockTime = 0;
    }
  }

  if(bleKeyboard.isConnected() && digitalRead(BUTTON_PIN) == HIGH && !keylock) {

    Serial.println("Sending 'KEY_RETURN'...");

    auto key = KEY_RETURN;
    bleKeyboard.write(key);

    keylock = 1;

  }

}

void connectionStatus() {

  if (SerialBT.connected()) {
    SerialBT.println("Connected to serial!");
  }
  
  if(bleKeyboard.isConnected() && !isConnected) {
    Serial.println("Connected!");
    isConnected = 1;
  }

  if(!bleKeyboard.isConnected() && isConnected) {
    Serial.println("Device disconnected! Searching for connections...");
    isConnected = 0;
  }

}
