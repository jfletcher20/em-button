#pragma once // maybe make it so that only ArduinoJson is passed on multiple times?
#include "data/emb-db.h"
#include "data/emb-data.h"
#include <ArduinoJson.h>

enum class STPMethod { GET, POST, PUT, DELETE };

class STP {
    public:
        static void announceRequest(DynamicJsonDocument json) {
            Serial.print("\nReceived STP1.0 ");
            Serial.print(json["method"].as<String>());
            Serial.print(" ");
            Serial.print(json["route"].as<String>());
            Serial.println(" request.\n");
        }
        static String createRequest(STPMethod method, String route, String body = "{}") {
            String request = "STP1.0{\"method\":\"";
            switch (method) {
                case STPMethod::GET:
                    request += "GET";
                    break;
                case STPMethod::POST:
                    request += "POST";
                    break;
                case STPMethod::PUT:
                    request += "PUT";
                    break;
                case STPMethod::DELETE:
                    request += "DELETE";
                    break;
            }
            request += "\",\"route\":\"";
            request += route + "\",\"body\":";
            request += body + "}";
            return request;
        }
        static String createResponse(int status, String message) {
            String response = "STP1.0{\"status\":";
            response += status;
            response += ",\"data\":{\"message\":\"";
            response += message + "\"}}";
            return response;
        }
        static String createResponse(int status, String message, String dataKey, String data) {
            String response = "STP1.0{\"status\":";
            response += status;
            response += ",\"data\":{\"message\":\"";
            response += message + "\",\"" + dataKey + "\":" + data + "}}";
            return response;
        }
        // function to create response from array of datakeys and array of data
        static String createResponse(int status, String message, String dataKeys[], String data[], int size) {
            String response = "STP1.0{\"status\":" + String(status) + ",\"data\":{\"message\":\"" + message + "\",";
            for (int i = 0; i < size; i++) {
                response += "\"" + dataKeys[i] + "\":" + data[i];
                if (i < size - 1) {
                    response += ",";
                }
            }
            response += "}}";
            return response;
        }
};

class STPDBConnection {

    private:
        void getOrDelete(STPMethod req, DynamicJsonDocument json) {
            switch (req) {
                case STPMethod::GET:
                    database->getAll();
                    break;
                case STPMethod::DELETE:
                    // database->remove(json["id"]);
                    database->clear();
                    break;
            }
        }

        void postOrPut(STPMethod req, DynamicJsonDocument json) {
            EmbButton emb = embFromJson(json);
            switch (req) {
                case STPMethod::POST:
                    database->add(emb, true);
                    break;
                case STPMethod::PUT:
                    database->update(emb);
                    break;
            }
        }

    public:
        EmbButtonDB* database;

        STPDBConnection(String database_table) {
            this->database = new EmbButtonDB(database_table);
            this->database->begin();
        }

        ~STPDBConnection() {
            delete this->database;
        }

        void requestLogic(STPMethod req, DynamicJsonDocument json) {
            if (req == STPMethod::GET || req == STPMethod::DELETE) {
                getOrDelete(req, json);
            } else {
                postOrPut(req, json);
            }
        }

        EmbButton embFromJson(DynamicJsonDocument json) {
            EmbButton emb;
            emb.id = json["id"] | 0;
            emb.electromagnet = json["electromagnet"] | 13;
            emb.electromagnet_power = json["electromagnet_power"] | 1.0;
            emb.hall_sensor = json["hall_sensor"] | 4;
            
            int id[3];
            int keyId[3];
            double activation_point[3];

            JsonArray actions = json["actions"];
            for (int i = 0; i < 3; i++) {
                DynamicJsonDocument action(100);
                deserializeJson(action, actions[i].as<String>());
                id[i] = action["actionId"] | -1;
                keyId[i] = action["keyId"] | KEY_RETURN;
                activation_point[i] = action["activation_point"] | 0.5;
            }
            emb.actions = embActions(id, keyId, activation_point);

            return emb;
        }

};