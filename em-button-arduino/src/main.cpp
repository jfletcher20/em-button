#include "headers.h"
#include <Arduino.h>
#include <stdio.h>

bool enableDevice = true;

Emb emb;
HallFilter* filter = new HallFilter(&emb);
DisplayManager displayManager(filter, &enableDevice, &keyBlock.timesPressed);

EmbServer embServer(filter, &displayManager, &enableDevice);

void setup() {

  pinMode(0, INPUT_PULLDOWN);
  pinMode(emb.keyData.hall_sensor, INPUT);
  pinMode(emb.keyData.electromagnet, OUTPUT);
  pinMode(2, OUTPUT); // blue LED

  Serial.begin(115200);

  // init emb by reading from database
  DynamicJsonDocument json(1024);
  String embString = embServer.stp.database->getAll()[0];
  deserializeJson(json, embString);

  emb.keyData = embServer.stp.embFromJson(json);
  delete filter;
  filter = new HallFilter(&emb);

  emb.keyboard.begin();

  displayManager.setup();
  displayManager.drawScene();

  Serial.print("STP1.0{\"status\":200,\"data\":{\"message\":\"Device is running. Designed as part of bacchelor's thesis by Joshua Lee Fletcher, ");
  Serial.print(emb.manufacturer);
  Serial.println(", 2023\"}}");
  Serial.print("STP1.0{\"status\":300,\"data\":{\"message\":\"Searching for connections for ");
  Serial.print(emb.name);
  Serial.println("\"}}");

}

bool pressed = false;
// save time since pressed
unsigned long timePressed = 0;
void userButtonLogic();

int counter = 0;
int lastRecordedTime = 0;
void _enableDevice();
void _disableDevice();
void loop() {

  embServer.serialLogic();

  userButtonLogic(); // user button should always be available regardless of whether 
  if (enableDevice) {
    analogWrite(emb.keyData.electromagnet, (int)(emb.keyData.electromagnet_power * 255));
    int newReading = filter->normalize();
    if(newReading > -11) {
      displayManager.drawScene();
    }
    if(KeyboardLogic::getConnectionStatusUpdate(emb)) {
      displayManager.drawScene();
    }
    if(KeyboardLogic::keyboardLogic(*filter)) {
      counter++;
      if(counter > 15) { // if it refreshes too often, it causes lag
        counter = 0;
        displayManager.drawScene();
      }
    }
  } else {
    analogWrite(emb.keyData.electromagnet, 0);
  }

  // use lastrecordedtime to disable the device after 45 seconds, and re-enable it after twice that time
  if(millis() - lastRecordedTime > 45000 && enableDevice) {
    _disableDevice();
    lastRecordedTime = millis();
  } else if(millis() - lastRecordedTime > 90000 && !enableDevice) {
    _enableDevice();
    lastRecordedTime = millis();
  }

}


void _disableDevice() {
    *displayManager.enableDevice = enableDevice = false;
    displayManager.drawScene();
}

void _enableDevice() {
    *displayManager.enableDevice = enableDevice = true;
    displayManager.drawScene();
}

void userButtonLogic() {
  if(digitalRead(0) == HIGH && !pressed && millis() - timePressed > 100) {
    // Serial.println("\nUser button pressed. Refreshing data...");
    embServer.refreshDisplayData();
    pressed = true;
    timePressed = millis();
  } else if(digitalRead(0) == LOW && pressed && millis() - timePressed > 100) {
    pressed = false;
    timePressed = 0;
    display.clearDisplay();
  }
}