#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <limits>
#include <sstream>

#define KEY_RETURN 100

using namespace std;

fstream file;

struct EmbConnectionState {
    bool keyboardConnected = false;
    bool serialConnected = false;
};

// struct EmbStateThresholds {
//     const int active = 0x1, inactive = 0x0;
// };

typedef int Pin;
// struct EmbButtonData {
//     const Pin pin = 2;
//     const EmbStateThresholds state;
// };

struct EmbButton {
    uint8_t id = 0;
    uint8_t keyId = ' ';
    double output_resistance, activation_point = 1650;
    // EmbButtonData buttonData;
    Pin electromagnet = 13, hall_sensor = 4;
    //std::function<void()> delegateFunction;
};

struct Emb {
    const char name[20] = "ESP32 Emb Keyboard";
    const char manufacturer[20] = "IoT Lab, FOI";
    // BleKeyboard keyboard = BleKeyboard(name, manufacturer, 100);
    EmbConnectionState connectionStatus;
    EmbButton keyData;
};

void r() {
	cout << " Returning to menu..." << endl << endl;
}

void c(int choice) {
	if(choice > 0 && choice < 10)
		cout << " Running option " << choice << endl << endl;
	else
		cout << " ERROR: Failed to recognize option " << choice << "." << endl << endl;
}

int count_inputs(fstream &file_) {
	
	int counter = 0;
	
	file_.open("emb-db.dat", ios::in | ios::binary);
	
	if(!file_) {
		file_.open("emb-db.dat", ios::out | ios::binary);
		counter = 0;
		file_.close();
		file_.clear();
	} else {
		file_.seekg(0, ios::end);
		counter = file_.tellg() / sizeof(EmbButton);
		file_.close();
		file_.clear();
	}
	
	return counter;
	
}

void inputAttr(std::string &attr) {
    std::getline(std::cin, attr);
}

void inputAttr(uint8_t& attr) {
    while (!(cin >> attr)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter an integer." << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void inputAttr(Pin& attr) {
    while (!(cin >> attr)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter an integer." << endl;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void inputAttr(double &attr) {
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    if (!(ss >> attr)) {
        std::cout << "Invalid input. Please enter a number." << std::endl;
        inputAttr(attr);
    }
}

EmbButton search(int id);
void input() {
	
	EmbButton struc;
	int counter = count_inputs(file);
	
	cout << " Input #" << counter << endl << endl;
	
	struc.id = counter;
	
	cout << "\t KEY:               >> "; inputAttr(struc.keyId);
	cout << "\t EM-PIN:            >> "; inputAttr(struc.electromagnet);
	cout << "\t HALL-PIN:          >> "; inputAttr(struc.hall_sensor);
	cout << "\t DELEGATE:          >> *not taking inputs"; cout << endl; //inputAttr(struc.delegateFunctiondelegate);
	cout << "\t ACTIVATION-POINT:  >> "; inputAttr(struc.activation_point);
	
	file.open("emb-db.dat", ios::out | ios::app | ios::binary);
	file.write((char *) &struc, sizeof(EmbButton));
	file.close();
	file.clear();
	
}


void add_default() {
	
	EmbButton struc;
	int counter = count_inputs(file);
	file.open("emb-db.dat", ios::out | ios::app | ios::binary);
	
	file.write((char *) &struc, sizeof(EmbButton));
	file.close();
	file.clear();
	
}

void output() {
	
	EmbButton struc;
	
	int limit = 1000;
	
	file.open("emb-db.dat", ios::in | ios::binary);
	
	while(limit) {
		
		file.read((char *) &struc, sizeof(EmbButton));
		
		if(file.eof())
			break;
		
		if(struc.id < 10)
			cout << "    <-----------------------------[0" << (int) struc.id << "]----------------------------->" << endl << endl;
		else
			cout << "    <-----------------------------[" << (int) struc.id << "]----------------------------->" << endl << endl;
		
		cout << "\t KEY:               << " << struc.keyId << endl;
		cout << "\t EM-PIN:            << " << struc.electromagnet << endl;
		cout << "\t HALL-PIN:          << " << struc.hall_sensor << endl;
		cout << "\t DELEGATE:          << *not defined for now" << endl; //<< struc.delegateFunctiondelegate << endl;
		cout << "\t ACTIVATION-POINT:  << " << struc.activation_point << endl << endl;
		
		limit--;
		
	}
	
	file.close();
	file.clear();
	
	if(limit < 1)
		cout << " Output cancelled because the file is too big. File has " << count_inputs(file) - 1000 << " entries remaining." << endl << endl;
	
	cout << "    <-[discovered: " << int(1000 - limit) << " entries]-------------------------------------->" << endl << endl;
	
}

EmbButton search(int criteria) {
	
	EmbButton struc;
	
	int counter = 0;
	
	file.open("emb-db.dat", ios::in | ios::binary);
	
	while(true) {
		
		file.read((char *) &struc, sizeof(EmbButton));
		
		if(file.eof())
			break;
		
		if(struc.id == criteria) {
			
			if(struc.id < 10)
				cout << "    <-----------------------------[0" << (int) struc.id << "]----------------------------->" << endl << endl;
			else
				cout << "    <-----------------------------[" << (int) struc.id << "]----------------------------->" << endl << endl;
		
			cout << "\t KEY:               << " << struc.keyId << endl;
			cout << "\t EM-PIN:            << " << struc.electromagnet << endl;
			cout << "\t HALL-PIN:          << " << struc.hall_sensor << endl;
			cout << "\t DELEGATE:          << *not defined for now" << endl; // struc.delegateFunctiondelegate << endl;
			cout << "\t ACTIVATION-POINT:  << " << struc.activation_point << endl << endl;
			
			counter++;
			
		}
		
	}
	
	if(counter)
		cout << "    <-------------------------------------------------------------->" << endl << endl;
	
	file.close();
	file.clear();
	
	if(counter == 0)
		cout << " NOTE: Found no entries with that ID." << endl << endl;
	
	return struc;
	
}

void find() {
	
	int id;
	
	cout << ">> Input search criteria (ID): "; cin >> id; cout << endl;
	
	EmbButton instance = search(id);

}

void delete_entry(int criteria) {
	
    fstream file("emb-db.dat", ios::in | ios::out | ios::binary);
    EmbButton struc;

    // Search for the entry
    int entry_found = false;
    while (file.read((char*)&struc, sizeof(EmbButton))) {
        if (struc.id == criteria) {
            entry_found = true;
            break;
        }
    }

    if (!entry_found) {
        cout << " No entry with ID " << criteria << " found." << endl;
        return;
    }

    // Overwrite the entry with zeros
    file.seekp(-static_cast<int>(sizeof(EmbButton)), ios::cur);  // Move the put pointer back to the beginning of the current record
    streampos pos = file.tellp();  // Remember the current position
    EmbButton empty;  // Zero-initialized struct
    file.write(reinterpret_cast<const char*>(&empty), sizeof(EmbButton));  // Overwrite the current record with zeros
    file.flush();  // Commit the changes to the file
    cout << " Entry with ID " << criteria << " deleted." << endl;

    // Shift the rest of the records to fill the gap
    while (file.read((char*)&struc, sizeof(EmbButton))) {
        file.seekp(-static_cast<int>(sizeof(EmbButton)), ios::cur);  // Move the put pointer back to the beginning of the current record
        file.write(reinterpret_cast<const char*>(&struc), sizeof(EmbButton));  // Write the current record to the previous record position
        file.seekp(0, ios::cur);  // Move the put pointer to the end of the current record
    }

    file.close();
    
}

void del() {
	
	int id;
	cout << ">> Input ID to delete: "; cin >> id; cout << endl;
	delete_entry(id);
	
}

void clear_create_doc() {
	
	char str[255];
	
	fstream clr;
	clr.open("emb-db.dat", ios::out);
	clr.close();
	clr.clear();
	
}

bool update() {
    int id;

    cout << ">> Input ID to update: "; cin >> id; cout << endl;

    EmbButton re = search(id);

	re.id = id;
    cout << "\t KEY:               >> ";
    inputAttr(re.keyId);
    cout << "\t EM-PIN:            >> ";
    inputAttr(re.electromagnet);
    cout << "\t HALL-PIN:          >> ";
    inputAttr(re.hall_sensor);
    cout << "\t DELEGATE:          >> *not taking inputs";
    cout << endl; //inputAttr(struc.delegateFunctiondelegate);
    cout << "\t ACTIVATION-POINT:  >> ";
    inputAttr(re.activation_point);
	
    fstream file("emb-db.dat", ios::in | ios::out | ios::binary);
    EmbButton struc;

    // Search for the entry
    int entry_found = false;
    while (file.read((char*)&struc, sizeof(EmbButton))) {
        if (struc.id == id) {
            entry_found = true;
            break;
        }
    }

    if (!entry_found) {
        cout << " No entry with ID " << id << " found." << endl;
        return false;
    }

    // Overwrite the entry with zeros
    file.seekp(-static_cast<int>(sizeof(EmbButton)), ios::cur);  // Move the put pointer back to the beginning of the current record
    streampos pos = file.tellp();  // Remember the current position
    file.write(reinterpret_cast<const char*>(&re), sizeof(EmbButton));  // Overwrite the current record with zeros
    file.flush();  // Commit the changes to the file
    cout << " Entry with ID " << id << " deleted." << endl;

    // Shift the rest of the records to fill the gap
    while (file.read((char*)&struc, sizeof(EmbButton))) {
        file.seekp(-static_cast<int>(sizeof(EmbButton)), ios::cur);  // Move the put pointer back to the beginning of the current record
        file.write(reinterpret_cast<const char*>(&struc), sizeof(EmbButton));  // Write the current record to the previous record position
        file.seekp(0, ios::cur);  // Move the put pointer to the end of the current record
    }

    file.close();

    cout << " Successfully updated record with ID " << id << endl;

    return true;
}


int main() {
	
	bool cont = true;
	int choice;
	
	while(cont) {
		
		cout << ">===============================[ menu ]===============================<" << endl << endl;
		
		cout << " 1. Get" << endl;
		cout << " 2. Get {id}" << endl;
		cout << " 3. Post" << endl;
		cout << " 4. Put" << endl;
		cout << " 5. Delete {id}" << endl << endl;
		
		cout << " 6. Clear/Create document" << endl << endl;
		cout << " 7. Restore defaults" << endl << endl;
		
		cout << " 9. End program" << endl << endl;
		
		cout << ">> "; inputAttr(choice); cout << endl;
		
		switch(choice) {
			case 1:
				c(choice);
				output();
				r();
				break;
			case 2:
				c(choice);
				find();
				r();
				break;
			case 3:
				c(choice);
				input();
				r();
				break;
			case 4:
				c(choice);
				update();
				r();
				break;
			case 5:
				c(choice);
				del();
				r();
				break;
			case 6:
				c(choice);
				clear_create_doc();
				r();
				break;
			case 7:
				c(choice);
				clear_create_doc();
				add_default();
				r();
				break;
			case 9:
				c(choice);
				cout << " Ending the program..." << endl << endl;
				cont = false;
				break;
			default:
				c(choice);
				r();
				break;
		}
		
	}
	
	system("Pause");
	return 0;
	
}
