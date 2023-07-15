#include "headers.h"
#include <Arduino.h>
#include <stdio.h>

bool enableDevice = true;

Emb emb;
HallFilter filter(&emb);
DisplayManager displayManager(&filter, &enableDevice, &keyBlock.timesPressed);

void setup() {

  pinMode(0, INPUT_PULLDOWN);
  pinMode(emb.keyData.hall_sensor, INPUT);
  pinMode(emb.keyData.electromagnet, OUTPUT);
  pinMode(2, OUTPUT); // blue LED

  Serial.begin(115200);
  emb.keyboard.end();
  emb.keyboard.begin();

  displayManager.setup();
  displayManager.drawScene();

  Serial.println(emb.manufacturer);
  Serial.print("Searching for connections for ");
  Serial.println(emb.name);

}

bool pressed = false;
// save time since pressed
unsigned long timePressed = 0;
void userButtonLogic();

void loop() {

  serialDataOutput(displayManager, &filter, &enableDevice);
  userButtonLogic();

  if(enableDevice) {
    analogWrite(emb.keyData.electromagnet, 255);
    // if filter has new reading print normalize
    int newReading = filter.normalize();
    if(newReading > -11) {
      Serial.print("printing: ");
      Serial.println(newReading);
      refreshDisplayData(displayManager);
    }
    KeyboardLogic::getConnectionStatusUpdate(emb, displayManager);
    KeyboardLogic::keyboardLogic(filter);
  }

}

void userButtonLogic() {
  if(digitalRead(0) == HIGH && !pressed && millis() - timePressed > 100) {
    Serial.println("\nUser button pressed. Refreshing data...");
    refreshDisplayData(displayManager);
    pressed = true;
    timePressed = millis();
  } else if(digitalRead(0) == LOW && pressed && millis() - timePressed > 100) {
    pressed = false;
    timePressed = 0;
    display.clearDisplay();
  }
}