#pragma once // maybe make it so that only ArduinoJson is passed on multiple times?
#include "data/emb-db.h"
#include "data/emb-data.h"
#include <ArduinoJson.h>

enum class STPMethod { GET, POST, PUT, DELETE };

class STPDBCommunication {

    private:
        void getOrDelete(STPMethod req, DynamicJsonDocument json) {
            switch (req) {
                case STPMethod::GET:
                    database->printAll(json);
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

        STPDBCommunication(String database_table) {
            this->database = new EmbButtonDB(database_table);
            this->database->begin();
        }

        ~STPDBCommunication() {
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