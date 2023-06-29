#pragma once // maybe make it so that only ArduinoJson is passed on multiple times?
#include "data/emb-db.h"
#include "data/emb-data.h"
#include <ArduinoJson.h>

enum class STPMethod { GET, POST, PUT, DELETE };

class STP {

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
            emb.keyID = json["keyID"] | KEY_RETURN;
            emb.activation_point = json["activation_point"] | 1650;
            emb.electromagnet = json["electromagnet"] | 13;
            emb.hall_sensor = json["hall_sensor"] | 4;
            return emb;
        }

    public:
        EmbButtonDB* database;

        STP(String database_table) {
            this->database = new EmbButtonDB(database_table);
            this->database->begin();
        }

        ~STP() {
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