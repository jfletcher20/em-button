#include <Arduino.h>

#define BUTTON_PIN 21

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW) {
    Serial.println("off");
  } else {
    Serial.println("on");
  }
  delay(100); // add a small delay to avoid bouncing
}
