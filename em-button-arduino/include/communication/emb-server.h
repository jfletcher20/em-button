#pragma once

#include "logic/emb-hall-filter.h"
#include "logic/keyboard-logic.h"
#include "stp-db-connection.h"
#include "logic/emb-logic.h"
#include "display.h"
#include "routes.h"

#include <ArduinoJson.h>
#include <Arduino.h>


class EmbServer {
    private:
        void announceRequest(DynamicJsonDocument json) {
            Serial.print("\nReceived STP1.0 ");
            Serial.print(json["method"].as<String>());
            Serial.print(" ");
            Serial.print(json["route"].as<String>());
            Serial.println(" request.\n");
        }

        String getRouteByKey(Routes key) {
            return routes[key];
        }

        void getRootRoute(DynamicJsonDocument json) {
            STPMethod req = method(json);
            switch(req) {
                default:
                    Serial.println(STP::createResponse(200, "Welcome to the Emb Keyboard API"));
            }
        }

        // route to return list of routes
        void getRoutesRoute() {
            String routesJson = "[";
            for(int i = 0; i < sizeof(routes) / sizeof(routes[0]); i++) {
                routesJson += "\"" + routes[i] + "\"";
                if(i < sizeof(routes) / sizeof(routes[0]) - 1) routesJson += ",";
            }
            routesJson += "]";
            Serial.println(STP::createResponse(200, "Accessed routes route successfully", "routes", routesJson));
        }

        void dbRouteManager(DynamicJsonDocument json) {
            STPMethod req = method(json);
            stp.requestLogic(req, json);
            Serial.println(STP::createResponse(200, "Accessed database route successfully"));
        }

        void getCalibrateRoute() {
            Serial.println(STP::createResponse(200, "Beginning calibration; the device will now restart"));
            calibrateFilter();
        }

        void putEnableDisableRoute(bool enable) {
            if(enable) {
                *enableDevice = true;
                Serial.println(STP::createResponse(200, "Device enabled"));
            } else {
                *enableDevice = false;
                Serial.println(STP::createResponse(200, "Device disabled"));
            }
        }

        void getDataRoute() {
            Serial.println(STP::createResponse(200, "Accessed data route successfully", "data", displayManager->getJson().c_str()));
        }

        void getElectromagnetDataRoute() {
            Serial.println(STP::createResponse(200, "Accessed electromagnet data", "electromagnet_power", String(filter->emb->keyData.electromagnet_power)));
        }

        void putElectromagnetDataRoute(DynamicJsonDocument json) {
            Serial.println(STP::createResponse(200, "Updating electromagnet power from " + String(filter->emb->keyData.electromagnet_power), "electromagnet_power", String(json["electromagnet_power"].as<double>())));
            filter->emb->keyData.electromagnet_power = constrain(json["electromagnet_power"].as<double>(), 0, 1);
            stp.database->add(this->filter->emb->keyData, true);
            calibrateFilter();
        }

        void getHallSensorDataRoute() {
            Serial.println(STP::createResponse(200, "Accessed hall sensor data", "current_value", String(filter->getDisplayValue())));
        }

        void getHallSensorDataNormalizedRoute() {
            Serial.println(STP::createResponse(200, "Accessed normalized hall sensor data", "value_normalized", String(filter->normalized)));
        }

        void putEmbDataRoute(DynamicJsonDocument json) {
            this->filter->emb->keyData = stp.embFromJson(json);
            stp.database->add(this->filter->emb->keyData, true);
            calibrateFilter();
        }
    public:
        HallFilter* filter;
        DisplayManager* displayManager;
        bool* enableDevice;
        STPDBConnection stp = STPDBConnection("emb-db");
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
                    STP::announceRequest(doc);
                    handleRequest(doc["route"], doc);
                    /* example request:
                    STP1.0{"route":"/device/calibrate/","method":"get"}
                    */
                }
            }
        }

        void localRequestLogic(String request) {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, request);
            STP::announceRequest(doc);
            handleRequest(doc["route"], doc);
        }

        void refreshDisplayData(bool printData = true) {
            displayManager->drawScene();
            if(printData) Serial.println(STP::createResponse(200, "Data refreshed successfully", "current_state", displayManager->getJson().c_str()));
        }

        void handleRequest(String route, DynamicJsonDocument json) {
            if (String(route) == getRouteByKey(Routes::ROOT)) {
                getRootRoute(json);
            } else if (String(route) == getRouteByKey(Routes::ROUTES)) {
                getRoutesRoute();
            } else if (String(route) == getRouteByKey(Routes::DB)) {
                dbRouteManager(json);
            } else if (String(route) == getRouteByKey(Routes::CALIBRATE)) {
                getCalibrateRoute();
            } else if (String(route) == getRouteByKey(Routes::ENABLE) && method(json) == STPMethod::PUT) {
                putEnableDisableRoute(true);
            } else if (String(route) == getRouteByKey(Routes::DISABLE) && method(json) == STPMethod::PUT) {
                putEnableDisableRoute(false);
            } else if (String(route) == getRouteByKey(Routes::DATA)) {
                getDataRoute();
            } else if (String(route) == getRouteByKey(Routes::ELECTROMAGNET)) {
                getElectromagnetDataRoute();
            } else if (String(route) == getRouteByKey(Routes::ELECTROMAGNET_POWER) && method(json) == STPMethod::PUT) {
                putElectromagnetDataRoute(json);
            } else if (String(route) == getRouteByKey(Routes::HALLSENSOR)) {
                getHallSensorDataRoute();
            } else if (String(route) == getRouteByKey(Routes::HALLSENSOR_NORMALIZED)) {
                getHallSensorDataNormalizedRoute();
            } else {
                Serial.println(STP::createResponse(404, "Route not found"));
            }
            displayManager->drawScene();
        }

        void calibrateFilter() {
            *enableDevice = false;
            delete filter;
            filter = new HallFilter(filter->emb);
            *enableDevice = true;
        }

};