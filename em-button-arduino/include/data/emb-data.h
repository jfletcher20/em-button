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

struct EmbAction {
    int8_t actionId;
    uint8_t keyId = KEY_RETURN;
    double activation_point = 0.5;
};

// @brief Takes in 3 arrays of 3 elements each and returns an array of EmbAction structs
EmbAction* embActions(int id[3], int keyId[3], double activation_point[3]) {
    // define array and allocate values for 3 actions
    EmbAction* embActions = new EmbAction[3];
    for (int i = 0; i < 3; i++) {
        embActions[i].actionId = id[i];
        embActions[i].keyId = keyId[i];
        embActions[i].activation_point = activation_point[i];
    }
    return embActions;
}

typedef int Pin;
struct EmbButton {
    uint8_t id = 0;
    Pin electromagnet = 13, hall_sensor = 4;
    EmbAction* actions = embActions(new int[3]{0, 1, 2}, new int[3]{'0', '1', '2'}, new double[3]{0.2, 0.5, 0.7});
};

struct Emb {
    const char name[20] = "ESP32 Emb Keyboard";
    const char manufacturer[20] = "IoT Lab, FOI";
    const char version[10] = "v0.2.1";
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
    doc["keyId"] = emb.keyId;
    doc["activation_point"] = emb.activation_point;
    doc["electromagnet"] = emb.electromagnet;
    doc["hall_sensor"] = emb.hall_sensor;
    
    return doc;
}
*/