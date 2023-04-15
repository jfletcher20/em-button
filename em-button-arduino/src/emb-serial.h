#include "emb-data.h"

void serialLogic(Emb emb) {
  if (emb.serial.available() > 0) {
    String c = emb.serial.readString();
    c.trim();
    Serial.println("Received: " + c);
  }
}