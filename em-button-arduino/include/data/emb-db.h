#pragma once
#include "communication/stp-db-connection.h"
#include "emb-data.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
/// @brief STPMethod2 is an enum class that represents the STP protocol version 1.
/// @details Necessary because STPMethod enum wouldn't load properly for some reason via imports.
enum class STPMethod2 { GET, POST, PUT, DELETE };

/// @brief STP2 is a class that handles the STP protocol version 1.
/// @details Necessary because STP class wouldn't load properly for some reason via imports.
class STP2 {
    public:
        static void announceRequest(DynamicJsonDocument json) {
            Serial.print("\nReceived STP1.0 ");
            Serial.print(json["method"].as<String>());
            Serial.print(" ");
            Serial.print(json["route"].as<String>());
            Serial.println(" request.\n");
        }
        static String createRequest(STPMethod2 method, String route, String body) {
            String request = "STP1.0{\"method\":\"";
            switch (method) {
                case STPMethod2::GET:
                    request += "GET";
                    break;
                case STPMethod2::POST:
                    request += "POST";
                    break;
                case STPMethod2::PUT:
                    request += "PUT";
                    break;
                case STPMethod2::DELETE:
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

class EmbButtonDB {
  public:
    String file = "/emb-db.data";
    String table;
    EmbButtonDB(String table) {
      this->table = table;
    }

    bool begin() {
      Serial.println(STP2::createResponse(200, "Mounting file system"));
      if(!SPIFFS.begin(true)) {
        Serial.println(STP2::createResponse(500, "An error occurred while mounting file system"));
        return false;
      }
      Serial.println(STP2::createResponse(200, "File system mounted"));
      // Serial.print("File size: ");
      // Serial.println(SPIFFS.totalBytes());
      return true;
    }

    EmbButton get(int id) {
      EmbButton emb;
      emb.id = -1;
      for (int i = 0; i < 3; i++) {
        emb.actions[i].actionId = -1;
      }
      if(!SPIFFS.exists(this->file)) {
        Serial.println(STP2::createResponse(404, "File does not exist"));
        return emb;
      }

      File file = SPIFFS.open(this->file, "r");
      if(!file) {
        Serial.println(STP2::createResponse(500, "Failed to open file for reading"));
        return emb;
      }

      while(file.available()) {
        String record = file.readStringUntil('\n');
        DynamicJsonDocument json(1024);
        deserializeJson(json, record);
        if(json["id"].as<int>() == id) {
          file.close();
          emb.id = json["id"].as<int>();
          emb.electromagnet = json["electromagnet"].as<int>();
          double strength = json["electromagnet"].as<double>();
          emb.electromagnet_power = strength > 1 ? 1 : strength < 0 ? 0 : strength;
          emb.hall_sensor = json["hall_sensor"].as<int>();
          JsonArray actions = json["actions"].as<JsonArray>();
          for (int i = 0; i < 3; i++) {
            emb.actions[i].actionId = actions[i]["actionId"].as<int>();
            emb.actions[i].keyId = actions[i]["keyId"].as<int>();
            emb.actions[i].activation_point = actions[i]["activation_point"].as<int>();
          }
          return emb;
        }
      }
      
      file.close();
      Serial.println(STP2::createResponse(404, "Record not found"));
      return emb;
    }

    template <typename T>
    bool exists(String key, T value) {
      if(!SPIFFS.exists(this->file)) {
        Serial.println(STP2::createResponse(404, "File does not exist"));
        return false;
      }

      File file = SPIFFS.open(this->file, "r");
      if(!file) {
        Serial.println(STP2::createResponse(500, "Failed to open file for reading"));
        return false;
      }

      while(file.available()) {
        String record = file.readStringUntil('\n');
        DynamicJsonDocument json(1024);
        deserializeJson(json, record);
        if(json[key].as<T>() == value) {
          file.close();
          return true;
        }
      }
      
      file.close();
      return false;

    }

    void clear() {
      File file = SPIFFS.open(this->file, "a");
      if (!file) {
        Serial.println(STP2::createResponse(500, "Failed to open file for appending (trying to clear file)"));
        return;
      }
      SPIFFS.remove(this->file);
        Serial.println(STP2::createResponse(200, "Database was cleared."));
      file.close();
    }

    int add(EmbButton emb, bool clear = false) {
      if (clear) {
        this->clear();
      }

      File file = SPIFFS.open(this->file, "a");
      if (!file) {
        Serial.println(STP2::createResponse(500, "Failed to open file for appending (trying to add record)"));
        return -1;
      }

      if (!emb.id) {
        int newID = 1;
        while (exists("id", newID))
          newID++;
        emb.id = newID;
      }

      if (exists("id", emb.id)) {
        Serial.println(STP2::createResponse(409, "Record with that ID already exists"));
        return -1;
      }

      DynamicJsonDocument json(1024);
      json["id"] = emb.id;
      json["electromagnet"] = emb.electromagnet;
      json["electromagnet_power"] = emb.electromagnet_power;
      json["hall_sensor"] = emb.hall_sensor;
      JsonArray actions = json.createNestedArray("actions");
      for (int i = 0; i < 3; i++) {
        actions[i]["actionId"] = emb.actions[i].actionId;
        actions[i]["keyId"] = emb.actions[i].keyId;
        actions[i]["activation_point"] = emb.actions[i].activation_point;
      }

      String jsonString;
      serializeJson(json, jsonString);

      file.println(jsonString);

      return emb.id;

    }

    bool update(EmbButton emb) {
      if(!remove(emb.id)) {
        return false;
      }

      File file = SPIFFS.open(this->file, "a");
      if(!file) {
        Serial.println(STP2::createResponse(500, "Failed to open file for appending (trying to update record)"));
        return false;
      }

      size_t size = sizeof(emb);
      uint8_t* buf = (uint8_t*) &emb;
      file.write(buf, size);
      file.close();
      
      return true;
    }

    bool remove(int embId) {
      if(!SPIFFS.exists(this->file)) {
        Serial.println(STP2::createResponse(404, "File does not exist"));
        return false;
      }

      File inFile = SPIFFS.open(this->file, "r");
      if(!inFile) {
        Serial.println(STP2::createResponse(500, "Failed to open input file for reading (removing record)"));
        return false;
      }

      File outFile = SPIFFS.open("/emb-db-tmp.data", "w");
      if(!outFile) {
        Serial.println(STP2::createResponse(500, "Failed to open output file for writing (removing record)"));
        inFile.close();
        return false;
      }

      bool removed = false;
      EmbButton emb;
      size_t size = sizeof(emb);
      uint8_t* buf = (uint8_t*) &emb;

      while(inFile.read(buf, size)) {
        if(emb.id != embId) {
          outFile.write(buf, size);
        } else {
          removed = true;
        }
      }

      inFile.close();
      outFile.close();

      SPIFFS.remove(this->file);
      SPIFFS.rename("/emb-db-tmp.data", this->file);

      if(removed) {
        Serial.println(STP2::createResponse(200, "Record removed"));
      } else {
        Serial.println(STP2::createResponse(404, "Record not found"));
      }
      return removed;
    }

    String* getAll() {
      
      // print all lines in the document (each newline is a new object) (objects are all json strings)
      String* database = new String[100];

      if(!SPIFFS.exists(this->file)) {
        String response = STP2::createResponse(404, "File does not exist");
        Serial.println(response);
        return database;
      }

      File file = SPIFFS.open(this->file, "r");
      if(!file) {
        String response = STP2::createResponse(500, "Failed to open file for reading");
        Serial.println(response);
        return database;
      }
      int i = 0;
      Serial.print("STP1.0{\"status\":200,\"data\":[");
      while(file.available()) {
        database[i++] = file.readStringUntil('\n');
        Serial.print(database[i - 1]);
        Serial.print(",");
      }
      Serial.println("]}");

      file.close();
      return database;
    }

};