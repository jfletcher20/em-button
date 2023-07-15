#pragma once
#include "communication/display.h"
#include "emb-hall-filter.h"
#include "keyboard-logic.h"
#include "data/emb-data.h"

void refreshDisplayData(DisplayManager displayManager);
void serialDataOutput(DisplayManager displayManager, HallFilter* filter, bool* enableDevice) {
  if (Serial.available() > 0) {
    String serialData = Serial.readString();
    serialData.trim();
    Serial.println(serialData);
    if (serialData.equals("data")) {
    } else if(serialData.equals("calibrate")) {
      filter->calibrate();
    } else if(serialData.equals("disable")) {
      *enableDevice = false;
    } else if(serialData.equals("enable")) {
      // set the pointer "enableDevice" to true
      *enableDevice = true;
    }
    refreshDisplayData(displayManager);
  }
}

void refreshDisplayData(DisplayManager displayManager) {
  displayManager.drawScene();
  Serial.println(displayManager.getJson().c_str());
}