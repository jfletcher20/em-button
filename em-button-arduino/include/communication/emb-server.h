#pragma once
#include "clientside-serial-transfer-protocol.h"
#include "logic/emb-logic.h"
#include "request-types.h"
#include "routes.h"
#include <ArduinoJson.h>
#include <Arduino.h>

STP stp("testing");

class EmbServer {
    public:
        static request method(DynamicJsonDocument json) {
            char* method = json["method"];
            char check;
            if(method == "post") check = 'c';
            else if(method == "get") check = 'r';
            else if(method == "put") check = 'u';
            else if(method == "delete") check = 'd';

            request req;
            switch(check) {
                case 'c':
                    req = request::POST;
                    break;
                case 'r':
                    req = request::GET;
                    break;
                case 'u':
                    req = request::PUT;
                    break;
                case 'd':
                    req = request::DELETE;
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

        static void handleRequest(char* route, DynamicJsonDocument json) {
            if (strcmp(route, routes[0]) == 0) {
                rootRoute(json);
            } else if (strcmp(route, routes[1]) == 0) {
                // Handle /db/ route
            } else if (strcmp(route, routes[2]) == 0) {
                // Handle /device/callibrate/ route
            }
        }

        static void rootRoute(DynamicJsonDocument json) {
            request req = method(json);
            switch(req) {
                default:
                    throw std::invalid_argument("400 Bad request");
            }
        }

        static void dbRoute(DynamicJsonDocument json) {
            stp.requestLogic(method(json), json);
        }

};