#pragma once
#include "data/emb-data.h"
#include <Arduino.h>

/// @brief A class that filters the hall sensor readings
class HallFilter {

    private:
        #define THRESHOLD 20
        #define DEFAULT_VALUE 1910
        
        static const int WINDOW_SIZE = 40;

        int values[WINDOW_SIZE];
        int sum = 0;
        int localInd = 0;

        int getReading() {

            int value = getValue();

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
            if(average > current + 10 || average < current - 10) {
                current = average;
                return average;
            }
            return 0;
        }

    public:
        Emb* emb;

        const int max_normalized = 10;
        int current = DEFAULT_VALUE;
        int normalized = 0;

        HallFilter(Emb* emb) {
            this->emb = emb;
            // increase window size for higher accuracy
            for (int i = 0; i < WINDOW_SIZE; i++) {
                values[i] = DEFAULT_VALUE;
                sum += DEFAULT_VALUE;
            }
        }

        int getValue() {
            return analogRead(emb->keyData.hall_sensor);
        }

        int normalize() {
            int reading = denoise();
            if(reading == 0) return -500;
            int result = map(map(reading, 0, 4096, -10, 10), -3, 9, -max_normalized, max_normalized);
            if(result < 0) result *= -1;
            if(result > normalized + 2 || result < normalized - 2) {
                normalized = result;
                return normalized;
            }
            return -500;
        }

        bool pressed() {
            return normalized <= emb->keyData.activation_point * max_normalized ? false : true;
        }

        int denoise() {
            return getReading();
        }

};