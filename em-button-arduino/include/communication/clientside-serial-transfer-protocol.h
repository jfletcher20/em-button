#pragma once // maybe make it so that only ArduinoJson is passed on multiple times?
#include "emb-db.h"
#include "emb-data.h"

enum class request { GET, POST, PUT, DELETE };

class STP {

    public:
        EmbButtonDB database;

        STP(String database_table) {
            this->database = new EmbButtonDB(database_table);
            this->database.begin();
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

            const char* method = json["method"];

            std::string methodLower = method;
            std::transform(methodLower.begin(), methodLower.end(), methodLower.begin(), ::toLower);

            request req;
            switch(methodLower) {
                case "get":
                    req = request::GET;
                    break;
                case "post":
                    req = request::POST;
                    break;
                case "put":
                    req = request::PUT;
                    break;
                case "delete":
                    req = request::DELETE;
                    break;
            }

            String route = "/";
            if(json.containsKey("route") && !json["route"].isNull() && json["route"].is<String>())
                route += json["route"].as<String>();

            if (req == request::GET || req == request::DELETE) {

                JsonObject query = json["query"];
                DynamicJsonDocument queryDoc(1024);
                for (JsonPair pair : query)
                    queryDoc[pair.key()] = pair.value();

                switch (req) {
                    case request::GET:
                        database.printAll(route, queryDoc);
                        break;
                    case request::DELETE:
                        database.remove(route, queryDoc);
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
                        database.add(route, emb);
                        break;
                    case request::PUT:
                        database.update(route, emb);
                        break;
                }

            }
        }

};