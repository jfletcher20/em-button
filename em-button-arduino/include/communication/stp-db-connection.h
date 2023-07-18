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
        static String createRequest(STPMethod method, String route, String body) {
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
};

class STPDBConnection {

    private:
        void getOrDelete(STPMethod req, DynamicJsonDocument json) {
            switch (req) {
                case STPMethod::GET:
                    database->getAll();
                    break;
                case STPMethod::DELETE:
                    database->remove(json["id"]);
                    break;
            }
        }

        void postOrPut(STPMethod req, DynamicJsonDocument json) {
            EmbButton emb = embFromJson(json);
            switch (req) {
                case STPMethod::POST:
                    database->add(emb);
                    break;
                case STPMethod::PUT:
                    database->update(emb);
                    break;
            }
        }

        EmbButton embFromJson(DynamicJsonDocument json) {
            EmbButton emb;
            emb.id = json["id"] | 0;
            emb.electromagnet = json["electromagnet"] | 13;
            emb.hall_sensor = json["hall_sensor"] | 4;
            
            int id[3];
            int keyId[3];
            double activation_point[3];

            JsonArray actions = json["actions"];
            for (int i = 0; i < 3; i++) {
                id[i] = actions[i]["id"] | -1;
                keyId[i] = actions[i]["keyId"] | KEY_RETURN;
                activation_point[i] = actions[i]["activation_point"] | 0.5;
            }
            emb.actions = embActions(id, keyId, activation_point);

            return emb;
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

};