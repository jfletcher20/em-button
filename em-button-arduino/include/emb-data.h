#include <BleKeyboard.h>

#pragma once
#include <Arduino.h>

struct EmbConnectionState {
    bool keyboardConnected = false;
    bool serialConnected = false;
};

struct EmbStateThresholds {
    const int active = HIGH, inactive = LOW;
};

typedef int Pin;
struct EmbButtonData {
    const Pin pin = 2;
    const EmbStateThresholds state;
};

struct EmbButton {
    uint8_t id;
    uint8_t keyID = 'B';
    double output_v, activation_point;
    EmbButtonData buttonData;
    const Pin electromagnet = 4;
};

struct Emb {
    const char name[20] = "ESP32 Emb Keyboard";
    const char manufacturer[20] = "IoT Lab, FOI";
    BleKeyboard keyboard = BleKeyboard(name, manufacturer, 100);
    EmbConnectionState connectionStatus;
    EmbButton keyData;
    HardwareSerial serial; // TODO: verify if this is necessary at all
};