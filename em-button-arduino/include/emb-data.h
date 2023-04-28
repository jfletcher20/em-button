#include <BleKeyboard.h>

#pragma once
#include <stdexcept>
#include <Arduino.h>
#include "emb-errors.h"

struct EmbConnectionState {
    bool keyboardConnected = false;
    bool serialConnected = false;
};

struct EmbStateThresholds {
    const int active = 0x1, inactive = 0x0;
};

typedef int Pin;
struct EmbButtonData {
    const Pin pin = 2;
    const EmbStateThresholds state;
};

struct EmbButton {
    uint8_t id;
    uint8_t keyID = KEY_RETURN;
    double output_resistance, activation_point = 1650;
    EmbButtonData buttonData;
    const Pin electromagnet = 6, hall_sensor = 4;
    std::function<void()> delegateFunction;
};

struct Emb {
    const char name[20] = "ESP32 Emb Keyboard";
    const char manufacturer[20] = "IoT Lab, FOI";
    BleKeyboard keyboard = BleKeyboard(name, manufacturer, 100);
    EmbConnectionState connectionStatus;
    EmbButton keyData;
};