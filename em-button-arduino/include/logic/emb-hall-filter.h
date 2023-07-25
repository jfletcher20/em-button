#pragma once
#include "data/emb-data.h"
#include "esp_system.h"
#include <Arduino.h>

/// @brief A class that filters the hall sensor readings
class HallFilter {

    private:
        #define THRESHOLD 20
        int DEFAULT_VALUE = 1910;
        
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
            current = DEFAULT_VALUE = getValue();
            for (int i = 0; i < WINDOW_SIZE; i++) {
                values[i] = DEFAULT_VALUE;
                sum += DEFAULT_VALUE;
            }
        }

        void calibrate() {
            ESP.restart();
        }

        int getValue() {
            return analogRead(emb->keyData.hall_sensor);
        }

        int getDisplayValue() {
            return current;
        }

        int normalize() {
            int reading = denoise();
            if(reading == 0) return -500;
            // int result = map(reading, DEFAULT_VALUE, 4096, 0, max_normalized); // map function sucks
            int result = (reading - DEFAULT_VALUE) * max_normalized / (4096 - DEFAULT_VALUE);

            // minimum result is 1000
            // if(result < 0) result *= -1;
            if(result > normalized + 1 || result < normalized - 1) {
                normalized = result;
                return normalized;
            }
            return -500;
        }

        int pressed() {
            // return the index of the action that is pressed
            int activation_point, indexOfPressed = -1;
            for(int i = 0; i < 3 && emb->keyData.actions[i].actionId != -1; i++) {
                // get the activation point for the current action
                activation_point = max_normalized * emb->keyData.actions[i].activation_point;
                // if the reading meets it, the index is the current one
                if(normalized >= activation_point) {
                    indexOfPressed = i; // the next might also meet criteria, so continue until it fails
                }
            }
            return indexOfPressed;
            // return normalized <= emb->keyData.activation_point * max_normalized ? false : true;
        }

        int denoise() {
            return getReading();
        }

};