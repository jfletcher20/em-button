#pragma once // maybe make it so that only ArduinoJson is passed on multiple times?
#include "data/emb-db.h"
#include "data/emb-data.h"

enum class request { GET, POST, PUT, DELETE };

class STP {

    public:
        EmbButtonDB* database;

        STP(String database_table) {
            this->database = new EmbButtonDB(database_table);
            this->database->begin();
        }

        ~STP() {
            delete this->database;
        }

        void serialLogic(Emb emb) {
            if (Serial.available() > 0) {
                String c = Serial.readString();
                c.trim();
                if (c.startsWith("STP1.0")) {
                    String jsonData = c.substring(6);
                    DynamicJsonDocument doc(1024);
                    deserializeJson(doc, jsonData);
                    requestLogic(doc);
                }
            }
        }

        void requestLogic(DynamicJsonDocument json) {

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

            if (req == request::GET || req == request::DELETE) {

                JsonObject query = json["query"];
                DynamicJsonDocument queryDoc(1024);
                for (JsonPair pair : query)
                    queryDoc[pair.key()] = pair.value();

                switch (req) {
                    case request::GET:
                        database->printAll(queryDoc);
                        break;
                    case request::DELETE:
                        database->remove(queryDoc["id"]);
                        break;
                }

            } else {

                EmbButton emb;
                
                emb.id = json["id"] | 0;
                emb.keyID = json["keyID"] | KEY_RETURN;
                emb.activation_point = json["activation_point"] | 1650;
                emb.electromagnet = json["electromagnet"] | 13;
                emb.hall_sensor = json["hall_sensor"] | 4;

                switch (req) {
                    case request::POST:
                        database->add(emb);
                        break;
                    case request::PUT:
                        database->update(emb);
                        break;
                }

            }
        }

};