#pragma once // maybe make it so that only ArduinoJson is passed on multiple times?
#include "data/emb-db.h"
#include "data/emb-data.h"
#include "request-types.h"

class STP {

    private:
        void getOrDelete(request req, DynamicJsonDocument json) {
            switch (req) {
                case request::GET:
                    database->printAll(json);
                    break;
                case request::DELETE:
                    database->remove(json["id"]);
                    break;
            }
        }

        void postOrPut(request req, DynamicJsonDocument json) {
            EmbButton emb = embFromJson(json);
            switch (req) {
                case request::POST:
                    database->add(emb);
                    break;
                case request::PUT:
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

        void requestLogic(request req, DynamicJsonDocument json) {
            if (req == request::GET || req == request::DELETE) {
                getOrDelete(req, json);
            } else {
                postOrPut(req, json);
            }
        }

};