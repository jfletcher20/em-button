#pragma once
#include "logic/emb-hall-filter.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <Wire.h>

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
            display.println(filter->pressed());
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
        std::string getJson() {
            DynamicJsonDocument doc(1024);
            doc["id"] = filter->emb->keyData.id;
            doc["electromagnet"] = filter->emb->keyData.electromagnet;
            doc["hall_sensor"] = filter->emb->keyData.hall_sensor;
            doc["electromagnet_power"] = filter->emb->keyData.electromagnet_power;
            JsonArray actions = doc.createNestedArray("actions");
            for(int i = 0; i < 3 && filter->emb->keyData.actions[i].actionId != -1; i++) {
                JsonObject action = actions.createNestedObject();
                action["actionId"] = filter->emb->keyData.actions[i].actionId;
                action["keyId"] = filter->emb->keyData.actions[i].keyId;
                action["activation_point"] = filter->emb->keyData.actions[i].activation_point;
            }
            doc["current_state"] = filter->getDisplayValue();
            doc["value_normalized"] = filter->normalized;
            doc["times_pressed"] = *timesPressed;
            doc["button_pressed"] = filter->pressed();
            doc["device_enabled"] = *enableDevice;
            doc["ble_connected"] = filter->emb->connectionStatus.keyboardConnected;
            doc["time_when_retrieved"] = millis();
            String json;
            serializeJson(doc, json);
            return json.c_str();
        }
        std::string* getKeys() {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, getJson());
            std::string* keys = new std::string[doc.size()];
            int i = 0;
            for(JsonPair pair : doc.as<JsonObject>()) {
                keys[i] = pair.key().c_str();
                i++;
            }
            return keys;
        }
        std::string* getValues() {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, getJson());
            std::string* values = new std::string[doc.size()];
            int i = 0;
            for(JsonPair pair : doc.as<JsonObject>()) {
                values[i] = pair.value().as<std::string>();
                i++;
            }
            return values;
        }
        int getKeysLength() {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, getJson());
            return doc.size();
        }

};