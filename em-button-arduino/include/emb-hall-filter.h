#pragma once
#include <Arduino.h>

#define THRESHOLD 20
#define WINDOW_SIZE 40

void denoise(int HALL_PIN) {

    int values[WINDOW_SIZE];
    int sum = 0;
    int index = 0;

    int past = 1910;

    // Fill the window with initial readings
    for (int i = 0; i < WINDOW_SIZE; i++) {
        values[i] = 1900 + i;
        sum += values[i];
    }

    // Start reading values and applying denoising
    while (true) {
        // Read the current value
        int value = analogRead(HALL_PIN);

        // Compute the moving average of the window
        sum -= values[index];
        sum += value;
        values[index] = value;
        index = (index + 1) % WINDOW_SIZE;
        int average = sum / WINDOW_SIZE;

        // Check if the difference between the value and the average is greater than the threshold
        int diff = abs(value - average);
        if (diff > THRESHOLD) {
            // The value is noisy, skip it
            // Serial.println(value);
            continue;
        }

        // The value is not noisy, print it
        if(average > past + 10 || average < past - 10) {
            Serial.println(average);
            past = average;
        }
    }
}