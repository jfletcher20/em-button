#pragma once
#include "clientside-serial-transfer-protocol.h"
#include "logic/emb-logic.h"
#include "routes.h"
#include <ArduinoJson.h>
#include <Arduino.h>

STP stp("testing");

class EmbServer {
    public:
        static STPMethod method(DynamicJsonDocument json) {
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

        static void serialLogic() {
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

        static void handleRequest(String route, DynamicJsonDocument json) {
            if (String(route) == String(routes[0])) {
                rootRoute(json);
            } else if (String(route) == String(routes[1])) {
                // Handle /db/ route
            } else if (String(route) == String(routes[2])) {
                // Handle /device/callibrate/ route
            }
        }

        static void rootRoute(DynamicJsonDocument json) {
            STPMethod req = method(json);
            switch(req) {
                default:
                    throw std::invalid_argument("400 Bad request");
            }
        }

        static void dbRoute(DynamicJsonDocument json) {
            stp.requestLogic(method(json), json);
        }

};