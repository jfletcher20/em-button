#pragma once
#include <BleKeyboard.h>
#include <stdexcept>
#include <Arduino.h>
#include "communication/emb-errors.h"

struct EmbConnectionState {
    bool keyboardConnected = false;
    bool calibrating = false;
    bool disabled = false;
};

typedef int Pin;
struct EmbButton {
    uint8_t id;
    uint8_t keyID = KEY_RETURN;
    double activation_point = 0.5;
    Pin electromagnet = 13, hall_sensor = 4;
    int active = 1, inactive = 0;
};

struct Emb {
    const char name[20] = "ESP32 Emb Keyboard";
    const char manufacturer[20] = "IoT Lab, FOI";
    const char version[10] = "v0.1.4";
    BleKeyboard keyboard = BleKeyboard(name, manufacturer, 100);
    EmbConnectionState connectionStatus;
    EmbButton keyData;
};

/*
DynamicJsonDocument parseStruct(EmbButton emb) {
    // Create a new JSON object with enough capacity for the EmbButton struct
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(5));
    
    // Add each field of the EmbButton struct to the JSON object
    doc["id"] = emb.id;
    doc["keyID"] = emb.keyID;
    doc["activation_point"] = emb.activation_point;
    doc["electromagnet"] = emb.electromagnet;
    doc["hall_sensor"] = emb.hall_sensor;
    
    return doc;
}
*/