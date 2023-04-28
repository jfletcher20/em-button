#pragma once // maybe make it so that only ArduinoJson is passed on multiple times?
#include <ArduinoJson.h>
#include "emb-db.h"

// void serialLogic(Emb emb) {
//   if (Serial.available() > 0) {
//     String c = Serial.readString();
//     c.trim();
//     Serial.println("Received: " + c);
//   }
// }

void test(Emb emb) {

    EmbButton embButton = emb.keyData;

    DynamicJsonDocument doc(1024);
    doc["id"] = embButton.id;
    doc["keyID"] = embButton.keyID;
    doc["output_v"] = embButton.output_v;
    doc["activation_point"] = embButton.activation_point;
    doc["buttonData"]["pin"] = embButton.buttonData.pin;
    doc["buttonData"]["state"]["active"] = embButton.buttonData.state.active;
    doc["buttonData"]["state"]["inactive"] = embButton.buttonData.state.inactive;

    String json;
    serializeJson(doc, json);

    // Send the JSON string over serial
    Serial.println(json);

}