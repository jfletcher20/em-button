#include <BluetoothSerial.h>
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
    uint8_t keyID = KEY_RETURN;
    double output_v, activation_point;
    EmbButtonData buttonData;
};

struct Emb {
    EmbConnectionState connectionStatus;
    BleKeyboard keyboard;
    BluetoothSerial serial;
    EmbButton keyData;
};

void getConnectionStatusUpdate(Emb emb) {

    // part to act as serial for wireless communication with device
    if(emb.serial.connected() && !emb.connectionStatus.serialConnected) {
        Serial.println("Serial connected!");
        emb.serial.println("Bluetooth monitor: Serial connected!");
        emb.connectionStatus.serialConnected = 1;
    }

    if(!emb.serial.connected() && emb.connectionStatus.serialConnected) {
        Serial.println("Serial disconnected! Searching for connections...");
        emb.serial.println("Bluetooth monitor: Serial disconnected! Searching for connections...");
        emb.connectionStatus.serialConnected = 0;
    }

    // part to act as keyboard for registering keypresses on the computer
    if(emb.keyboard.isConnected() && !emb.connectionStatus.keyboardConnected) {
        Serial.println("Keyboard connected!");
        emb.connectionStatus.keyboardConnected = 1;
    }

    if(!emb.keyboard.isConnected() && emb.connectionStatus.keyboardConnected) {
        Serial.println("Keyboard disconnected! Searching for connections...");
        emb.connectionStatus.keyboardConnected = 0;
    }

}
