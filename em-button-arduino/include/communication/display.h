#pragma once
#include "logic/emb-hall-filter.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class DisplayManager {
    private:
        void setTextSettings() {
            display.setTextSize(1);
            display.setTextColor(WHITE);
        }
        void copyright() {
            display.println("");
            // display.println(filter->emb->manufacturer);
            display.println(filter->emb->name);
            display.println(filter->emb->version);
        }
        void currentValue() {
            display.print("Current: ");
            display.print(filter->getDisplayValue());
            display.print(" (");
            display.print(filter->normalized);
            display.println(")");
        }
        void isPressed() {
            display.print("Pressed (x");
            display.print(*timesPressed);
            display.print("): ");
            display.println(filter->pressed() ? "true" : "false");
        }
        bool enabledStatus() {
            display.print("Device is on: ");
            display.println(*enableDevice ? "true" : "false");
            return *enableDevice;
        }
        void connectionStatus() {
            display.print("Connected: ");
            display.println(filter->emb->connectionStatus.keyboardConnected ? "true" : "false");
        }
    public:
        HallFilter* filter;
        bool* enableDevice;
        unsigned long long int* timesPressed;
        DisplayManager(HallFilter* filter, bool* enableDevice, unsigned long long int* timesPressed) {
            this->filter = filter;
            this->enableDevice = enableDevice;
            this->timesPressed = timesPressed;
        }
        void setup() {
            // initialize the OLED object
            if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
                Serial.println(F("Display attempt met with failure."));
            }
            setTextSettings();
        }
        void drawScene() { // TODO: make it draw on "user" botton press
            display.clearDisplay();
            display.setCursor(0, 0);
            if(enabledStatus()) {
                connectionStatus();
                currentValue();
                isPressed();
            }
            copyright();
            display.display();
        }
};