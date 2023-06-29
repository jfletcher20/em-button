#pragma once
#include "emb-data.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>

class EmbButtonDB {
  private:
    File databaseFile;
    
  public:
    String file = "/emb-db.data";
    String table;
    EmbButtonDB(String table) {
      this->table = table;
    }

    void begin() {
      if(!SPIFFS.begin(true)) {
        Serial.println("Failed to mount file system");
        return;
      }
      Serial.println("File system mounted");
      Serial.print("File size: ");
      Serial.println(SPIFFS.totalBytes());
    }

    bool exists(int id) {
      if(!SPIFFS.exists(this->file)) {
        Serial.println("File does not exist");
        return false;
      }

      File file = SPIFFS.open(this->file, "r");
      if(!file) {
        Serial.println("Failed to open file for reading");
        return false;
      }

      EmbButton emb;
      size_t size = sizeof(emb);
      uint8_t* buf = (uint8_t*) &emb;

      while(file.read(buf, size)) {
        if(emb.id == id) {
          file.close();
          return true;
        }
      }

      file.close();
      return false;

    }

    void add(EmbButton emb) {
      File file = SPIFFS.open(this->file, "a");
      if (!file) {
        Serial.println("Failed to open file for appending");
        return;
      }

      if (!emb.id) {
        int newID = 1;
        while (exists(newID))
          newID++;
        emb.keyID = newID;
      }

      // Check if the ID is already in the database
      if (exists(emb.id)) {
        Serial.println("ID already exists in database");
        return;
      }

      size_t size = sizeof(emb);
      uint8_t* buf = (uint8_t*) &emb;
      file.write(buf, size);
      file.close();

      Serial.println("Record added");

    }

    bool update(EmbButton emb) {
      if(!remove(emb.id)) {
        return false;
      }

      File file = SPIFFS.open(this->file, "a");
      if(!file) {
        Serial.println("Failed to open file for appending");
        return false;
      }

      size_t size = sizeof(emb);
      uint8_t* buf = (uint8_t*) &emb;
      file.write(buf, size);
      file.close();
      Serial.println("Record updated");
      return true;
    }

    bool remove(int embId) {
      if(!SPIFFS.exists(this->file)) {
        Serial.println("File does not exist");
        return false;
      }

      File inFile = SPIFFS.open(this->file, "r");
      if(!inFile) {
        Serial.println("Failed to open input file");
        return false;
      }

      File outFile = SPIFFS.open("/emb-db-tmp.data", "w");
      if(!outFile) {
        Serial.println("Failed to open output file");
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
        Serial.println("Record removed");
      } else {
        Serial.println("Record not found");
      }
      return removed;
    }

    void printAll(DynamicJsonDocument queryDoc) {

      Serial.println("Printing all data:");
      Serial.println("----------------------------");

      if(queryDoc.containsKey("id")) {
        
        EmbButton emb;
        size_t size = sizeof(emb);
        uint8_t* buf = (uint8_t*) &emb;

        File inFile = SPIFFS.open(this->file, "r");
        File outFile = SPIFFS.open("/emb-db-tmp.data", "w");
        while(inFile.read(buf, size)) {
          if(emb.id != queryDoc["id"]) {
            outFile.write(buf, size);
          }
        }

        inFile.close();
        outFile.close();

        return;

      }

      // Open the database file for reading
      File file = SPIFFS.open("/emb_db.txt", "r");
      if (!file) {
        Serial.println("Failed to open file for reading");
        return;
      }

      // Read the file line by line and print the data
      String line;
      while (file.available()) {
        line = file.readStringUntil('\n');
        Serial.println(line);
      }

      // Close the file
      file.close();

      Serial.println("----------------------------");
    }

};