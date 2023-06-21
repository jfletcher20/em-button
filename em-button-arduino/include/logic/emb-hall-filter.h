
#pragma once
#include "emb-data.h"
#include <Arduino.h>

#define THRESHOLD 20
#define WINDOW_SIZE 40
#define DEFAULT_READING 1900

int latest = DEFAULT_READING;

class HallFilter {

    public:
        static void getReading(int HALL_PIN) {

            int values[WINDOW_SIZE];
            int sum = 0;
            int index = 0;

            int previous = DEFAULT_READING + THRESHOLD / 2;

            // Fill the window with initial readings
            for (int i = 0; i < WINDOW_SIZE; i++) {
                values[i] = DEFAULT_READING + i;
                sum += values[i];
            }

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
                // latest = average;
                return; // continue
            }

            // The value is not noisy, print it
            if(average > previous + 10 || average < previous - 10) {
                latest = average;
                previous = average;
            }
        }
        static int getValue(Emb emb) {
            return analogRead(emb.keyData.hall_sensor);
        }

};

int degree(Emb emb) {
    return -10 + (10 * DEFAULT_READING / (HallFilter::getValue(emb) + 0.5));
}

void denoise(int HALL_PIN) {
    HallFilter::getReading(HALL_PIN);
}
