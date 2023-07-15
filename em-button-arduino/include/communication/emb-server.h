#pragma once

#include "clientside-serial-transfer-protocol.h"
#include "logic/emb-hall-filter.h"
#include "logic/keyboard-logic.h"
#include "logic/emb-logic.h"
#include "routes.h"

#include <ArduinoJson.h>
#include <Arduino.h>

// STP stp("testing");

class EmbServer {
    public:
        HallFilter* filter;
        DisplayManager* displayManager;
        bool* enableDevice;
        EmbServer(HallFilter* filter, DisplayManager* displayManager, bool* enableDevice) {
            this->filter = filter;
            this->displayManager = displayManager;
            this->enableDevice = enableDevice;
        }

        STPMethod method(DynamicJsonDocument json) {
            String method = json["method"];
            char check;
            if(method == "post") check = 'c';
            else if(method == "get") check = 'r';
            else if(method == "put") check = 'u';
            else if(method == "delete") check = 'd';

            STPMethod req;
            switch(check) {
                case 'c':
                    req = STPMethod::POST;
                    break;
                case 'r':
                    req = STPMethod::GET;
                    break;
                case 'u':
                    req = STPMethod::PUT;
                    break;
                case 'd':
                    req = STPMethod::DELETE;
                    break;
                default:
                    throw std::invalid_argument("Invalid request method");
            }
            return req;
        }

        void serialLogic() {
            if (Serial.available() > 0) {
                String c = Serial.readString();
                c.trim();
                if (c.startsWith("STP1.0")) {
                    String jsonData = c.substring(6);
                    DynamicJsonDocument doc(1024);
                    deserializeJson(doc, jsonData);
                    handleRequest(doc["route"], doc);
                }
            }
        }

        void handleRequest(String route, DynamicJsonDocument json) {
            if (String(route) == String(routes[0])) {
                rootRoute(json);
            } else if (String(route) == String(routes[1])) {
                // Handle /db/ route
            } else if (String(route) == String(routes[2])) {
                // Handle /device/calibrate/ route
                filter->calibrate();
            } else if (String(route) == String(routes[3])) {
                // Handle /device/enable/ route
                *enableDevice = true;
            } else if (String(route) == String(routes[4])) {
                // Handle /device/disable/ route
                *enableDevice = false;
            } else if (String(route) == String(routes[4])) {
                // Handle /device/data/ route
                // send arduinojson data via serial monitor
                Serial.println(displayManager->getJson().c_str());
            } else {
                throw std::invalid_argument("404 Not found");
            }
            displayManager->drawScene();
        }

        void rootRoute(DynamicJsonDocument json) {
            STPMethod req = method(json);
            switch(req) {
                default:
                    throw std::invalid_argument("400 Bad request");
            }
        }

        void dbRoute(DynamicJsonDocument json) {
            // stp.requestLogic(method(json), json);
            Serial.println("db route");
        }

};