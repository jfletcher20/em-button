#pragma once

#include "clientside-serial-transfer-protocol.h"
#include "logic/emb-hall-filter.h"
#include "logic/keyboard-logic.h"
#include "logic/emb-logic.h"
#include "display.h"
#include "routes.h"

#include <ArduinoJson.h>
#include <Arduino.h>

// STPDBCommunication stp("testing");

class EmbServer {
    private:
        void announceRequest(DynamicJsonDocument json) {
            Serial.print("\nReceived STP1.0 ");
            Serial.print(json["method"].as<String>());
            Serial.print(" ");
            Serial.print(json["route"].as<String>());
            Serial.println(" request.\n");
        }
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
            if(method == "post" || method == "POST") check = 'c';
            else if(method == "get" || method == "GET") check = 'r';
            else if(method == "put" || method == "PUT") check = 'u';
            else if(method == "delete" || method == "DELETE") check = 'd';
            else check = 'r';

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
                    announceRequest(doc);
                    handleRequest(doc["route"], doc);
                    /* example request:
                    STP1.0{"route":"/device/calibrate/","method":"get"}
                    */
                }
            }
        }

        void refreshDisplayData() {
            displayManager->drawScene();
            Serial.println(displayManager->getJson().c_str());
        }

        void handleRequest(String route, DynamicJsonDocument json) {
            if (String(route) == String(routes[0])) {
                rootRoute(json);
            } else if (String(route) == String(routes[1])) {
                // Handle /db/ route
            } else if (String(route) == String(routes[2])) {
                // Handle /device/calibrate/ route
                Serial.println("STP1.0{\"status\":200,\"data\":{\"message\":\"Beginning calibration; the device will now restart\"}}");
                filter->calibrate();
            } else if (String(route) == String(routes[3])) {
                // Handle /device/enable/ route
                *enableDevice = true;
                Serial.println("STP1.0{\"status\":200,\"data\":{\"message\":\"Device enabled\"}}");
            } else if (String(route) == String(routes[4])) {
                // Handle /device/disable/ route
                *enableDevice = false;
                Serial.println("STP1.0{\"status\":200,\"data\":{\"message\":\"Device disabled\"}}");
            } else if (String(route) == String(routes[5])) {
                // Handle /device/data/ route
                Serial.print("STP1.0{\"status\":200,\"data\":");
                Serial.println(displayManager->getJson().c_str() + String("}"));
            } else {
                Serial.println("404 Bad requrest: Route not found");
            }
            displayManager->drawScene();
        }

        void rootRoute(DynamicJsonDocument json) {
            STPMethod req = method(json);
            switch(req) {
                default:
                    Serial.println("400 Bad request");
            }
        }

        void dbRoute(DynamicJsonDocument json) {
            // stp.requestLogic(method(json), json);
            Serial.println("db route");
        }

};