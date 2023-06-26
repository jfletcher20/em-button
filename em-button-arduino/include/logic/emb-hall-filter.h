#pragma once
#include "data/emb-data.h"
#include <Arduino.h>

/// @brief A class that filters the hall sensor readings
class HallFilter {

    private:
        #define THRESHOLD 20
        #define DEFAULT_VALUE 1910
        
        static const int WINDOW_SIZE = 40;

        int past = DEFAULT_VALUE;

        int values[WINDOW_SIZE];
        int sum = 0;
        int localInd = 0;

        Emb* emb;

    public:
        HallFilter(Emb* emb) {
            this->emb = emb;
            // increase window size for hier accuracy
            for (int i = 0; i < WINDOW_SIZE; i++) {
                values[i] = DEFAULT_VALUE;
                sum += DEFAULT_VALUE;
            }
        }

        int getReading() {

            int value = analogRead(emb->keyData.hall_sensor);

            // Compute the moving average of the window
            sum -= values[localInd];
            sum += value;
            values[localInd] = value;
            localInd = (localInd + 1) % WINDOW_SIZE;
            int average = sum / WINDOW_SIZE;

            // Check if the difference between the value and the average is greater than the threshold
            int diff = abs(value - average);
            if (diff > THRESHOLD) {
                // The value is noisy, skip it
                return 0; // continue
            }

            // The value is not noisy, print it
            if(average > past + 10 || average < past - 10) {
                past = average;
                return average;
            }
            return 0;
        }

        int getValue() {
            return analogRead(emb->keyData.hall_sensor);
        }

        int normalize() {
            return -10 + (10 * past / (getValue() + 0.5));
        }

        int denoise() {
            return getReading();
        }

};