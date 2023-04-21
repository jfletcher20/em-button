#include <BleKeyboard.h>  

struct EmbConnectionState {
    bool keyboardConnected = false;
    bool serialConnected = false;
};

struct EmbActiveState {
    int active = HIGH, inactive = LOW;
};

typedef int Pin;
struct EmbButtonData {
    Pin pin = 2;
    EmbActiveState state;
};

struct EmbButton {
    uint8_t keyID = 'B';
    double output_v, activation_point;
    EmbButtonData buttonData;
};

struct Emb {
    char name[20] = "ESP32 Emb Keyboard";
    char manufacturer[20] = "IoT Lab, FOI";
    EmbConnectionState connectionStatus;
    BleKeyboard keyboard = BleKeyboard(name, manufacturer, 100);
    EmbButton keyData;
};