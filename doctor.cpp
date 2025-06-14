#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

const int MAX_DOCTORS = 100;
const int MAX_PATIENTS = 100;
const int MAX_APPOINTMENTS = 100;


bool authenticateManager() {
    string password;
    cout << "Enter password: ";
    cin >> password;
    
    return (password == "admin");
}

void deleteDoctor();
void searchDoctor();

void runDoctorModule() {
    const string MANAGER_ID = "admin123";
  
    auto authenticateManager = [&]() -> bool {
        string enteredID;
        cout << "Enter Manager ID to continue: ";
        cin >> enteredID;
        cin.ignore();
        return enteredID == MANAGER_ID;
    };

    auto addDoctor = [&]() {
        if (!authenticateManager()) {
            cout << "Unauthorized. Only managers can add doctors.\n";
            return;
        }
        Doctor doc;
        string input;

        while (true) {
            cout << "Enter Doctor ID (numeric): ";
            getline(cin, input);
            if (isNumeric(input)) {
                doc.id = stoi(input);
                break;
            }
            cout << "Invalid ID. Try again.\n";
        }

        cout << "Enter Doctor Name: ";
        getline(cin, doc.name);
        while (!isAlpha(doc.name)) {
            cout << "Invalid name. Try again: ";
            getline(cin, doc.name);
        }

        cout << "Enter Specialization: ";
        getline(cin, doc.specialization);
        while (!isAlpha(doc.specialization)) {
            cout << "Invalid specialization. Try again: ";
            getline(cin, doc.specialization);
        }

        cout << "Enter Contact Number (10 digits): ";
        getline(cin, doc.contact);
        while (!isValidContact(doc.contact)) {
            cout << "Invalid contact. Try again: ";
            getline(cin, doc.contact);
        }

        doc.status = ACTIVE_DOCTOR;
        doctors.push_back(doc);
        cout << "Doctor added successfully!\n";
    };
    auto checkDoctorAvailability = [&]() {
        int id;
        cout << "Enter Doctor ID to check availability: ";
        cin >> id;
        cin.ignore();

        for (const auto& doc : doctors) {
            if (doc.id == id) {
                cout << "\nDoctor Details:\n";
                cout << "Name: " << doc.name << "\n";
                cout << "Specialization: " << doc.specialization << "\n";
                cout << "Availability Status: ";
                switch (doc.status) {
                    case ACTIVE_DOCTOR:
                        cout << "AVAILABLE\n";
                        break;
                    case ON_LEAVE:
                        cout << "NOT AVAILABLE - Currently on leave\n";
                        break;
                    case RETIRED_DOCTOR:
                        cout << "NOT AVAILABLE - Retired\n";
                        break;
                }
                return;
            }
        }
        cout << "Doctor not found.\n";
    };
    auto editDoctor = [&]() {
        if (!authenticateManager()) {
            cout << "Unauthorized.\n";
            return;
        }
        int id;
        cout << "Enter Doctor ID to edit: ";
        cin >> id;
        cin.ignore();

        for (auto& doc : doctors) {
            if (doc.id == id) {
                int choice;
                do {
                    cout << "\nEditing Doctor: " << doc.name << "\n"
                         << "1. Name\n2. Specialization\n3. Contact\n4. Status\n5. Done\n"
                         << "Enter your choice: ";
                    cin >> choice;
                    cin.ignore();
                    switch (choice) {
                        case 1:
                            cout << "New name: ";
                            getline(cin, doc.name);
                            while (!isAlpha(doc.name)) {
                                cout << "Invalid name. Try again: ";
                                getline(cin, doc.name);
                            }
                            break;
                        case 2:
                            cout << "New specialization: ";
                            getline(cin, doc.specialization);
                            while (!isAlpha(doc.specialization)) {
                                cout << "Invalid specialization. Try again: ";
                                getline(cin, doc.specialization);
                            }
                            break;
                        case 3:
                            cout << "New contact: ";
                            getline(cin, doc.contact);
                            while (!isValidContact(doc.contact)) {
                                cout << "Invalid contact. Try again: ";
                                getline(cin, doc.contact);
                            }
                            break;
                        case 4:
                            cout << "\n1. Active\n2. On Leave\n3. Retired\nChoose status: ";
                            int status;
                            cin >> status;
                            cin.ignore();
                            if (status >= 1 && status <= 3)
                                doc.status = static_cast<DoctorStatus>(status - 1);
                            else
                                cout << "Invalid status choice.\n";
                            break;
                        case 5: break;
                        default: cout << "Invalid option.\n";
                    }
                } while (choice != 5);
                return;
            }
        }
        cout << "Doctor not found.\n";
    };

    auto displayDoctors = [&]() {
        if (doctors.empty()) {
            cout << "No doctor records to display.\n";
            return;
        }
        for (const auto& doc : doctors) {
            cout << "\nID: " << doc.id
                 << "\nName: " << doc.name
                 << "\nSpecialization: " << doc.specialization
                 << "\nContact: " << doc.contact
                 << "\nStatus: ";
            switch (doc.status) {
                case ACTIVE_DOCTOR: cout << "Active\n"; break;
                case ON_LEAVE: cout << "On Leave\n"; break;
                case RETIRED_DOCTOR: cout << "Retired\n"; break;
            }
        }
    };

    int choice;
    do {
        cout << "\n=== Doctor Management Menu ===\n"
             << "1. Add Doctor\n"
             << "2. Edit Doctor\n"
             << "3. Display Doctors\n"
             << "4. Delete Doctor\n"
             << "5 Check Doctor Availability\n"
             << "6. Search Doctor\n"
             << "7. Exit\n"
             << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addDoctor(); break;
            case 2: editDoctor(); break;
            case 3: displayDoctors(); break;
            case 4: deleteDoctor(); break;
            case 5: checkDoctorAvailability; break;
            case 6: searchDoctor(); break;
            case 7: cout << "Exiting Doctor Module...\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);
} // The main menu loop
    

    

void deleteDoctor() {
    if (!authenticateManager()) {
        cout << "Unauthorized.\n";
        return;
    }
    int id;
    cout << "Enter Doctor ID to delete: ";
    cin >> id;
    cin.ignore();

    for (auto it = doctors.begin(); it != doctors.end(); ++it) {
        if (it->id == id) {
            doctors.erase(it);
            cout << "Doctor successfully deleted.\n";
            return;
        }
    }
    cout << "Doctor not found.\n";
}
void searchDoctor(){
    int choose;
    string searchTerm;
    bool found = false;

    cout<<"\nSearch Doctor By: \n";
    cout<<"1.Doctor ID\n";
    cout<<"2. Doctor Name\n";
    cout<<"3. Doctor Specialization\n";
    cout<<"4. Doctor Contact\n";
    cout<<"Enter Your choice: ";
    cin>>choose;
    cin.ignore();

    switch(choose){
        case 1: {
            int id;
                cout << "Enter Doctor ID: ";
                cin >> id;
                cin.ignore();
                for (const auto& Doctor : doctors) {
                    if (Doctor.id == id) {
                        cout << "\nDoctor Found:\n";
                        cout << "ID: " << Doctor.id << "\n";
                        cout << "Name: " << Doctor.name << "\n";
                        cout<<"Specialization: "<< Doctor.specialization<<"\n";
                        cout << "Contact: " << Doctor.contact << "\n";
                        found = true;
                        break;
                    }
                }
            if (!found) cout << "Doctor not found.\n";
            }
            break;
     case 2:
            cout << "Enter Doctor Name: ";
            getline(cin, searchTerm);
            for (const auto& Doctor : doctors) {
                if (Doctor.name == searchTerm) {
                    cout << "\nDoctor Found:\n";
                    cout << "ID: " << Doctor.id << "\n";
                    cout << "Name: " << Doctor.name << "\n";
                    cout<<"Specialization: "<< Doctor.specialization<<"\n";
                        cout << "Contact: " << Doctor.contact << "\n";
                    found = true;
                }
            }
            if (!found) cout << "\nDoctor not found.\n";
            break;
     case 3:
            cout << "Enter Doctor Contact: ";
            getline(cin, searchTerm);
            for (const auto& Doctor : doctors) {
                if (Doctor.specialization == searchTerm) {
                  cout << "\nDoctor Found:\n";
                    cout << "ID: " << Doctor.id << "\n";
                    cout << "Name: " << Doctor.name << "\n";
                    cout<<"Specialization: "<< Doctor.specialization<<"\n";
                        cout << "Contact: " << Doctor.contact << "\n";
                    found = true;
                }
            }
            if (!found) cout << "\nDoctor not found.\n";
            break;
        case 4:
            cout << "Enter Doctor Contact: ";
            getline(cin, searchTerm);
            for (const auto& Doctor : doctors) {
                if (Doctor.contact == searchTerm) {
                  cout << "\nDoctor Found:\n";
                    cout << "ID: " << Doctor.id << "\n";
                    cout << "Name: " << Doctor.name << "\n";
                    cout<<"Specialization: "<< Doctor.specialization<<"\n";
                        cout << "Contact: " << Doctor.contact << "\n";
                    found = true;
                }
            }
            if (!found) cout << "Doctor not found.\n";
            break;
        default:
            cout << "Invalid choice.\n";
    }
}
void saveDoctorsToFile(const string& filename) {
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "Failed to open file for writing: " << filename << endl;
        return;
    }

    // Write header (optional)
    ofs << "ID,Name,Specialization,Contact,Status\n";

    for (const auto& doc : doctors) {
        ofs << doc.id << ","
            << doc.name << ","
            << doc.specialization << ","
            << doc.contact << ","
            << doc.status << "\n";
    }
    cout << "Doctor data saved to " << filename << endl;
}
void loadDoctorsFromFile(const string& filename) {
    ifstream ifs(filename);
    if (!ifs) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    getline(ifs, line); // skip header if present

    while (getline(ifs, line)) {
        stringstream ss(line);
        string token;
        Doctor doc;

        // Read ID
        if (getline(ss, token, ',')) {
            try {
                doc.id = stoi(token);
            } catch (...) {
                continue; // malformed line
            }
        } else continue;

        // Read Name
        if (getline(ss, token, ',')) {
            doc.name = token;
        } else continue;

        // Read Specialization
        if (getline(ss, token, ',')) {
            doc.specialization = token;
        } else continue;

        // Read Contact
        if (getline(ss, token, ',')) {
            doc.contact = token;
        } else continue;

        // Read Status
        if (getline(ss, token, ',')) {
            try {
                int stat = stoi(token);
                doc.status = static_cast<DoctorStatus>(stat);
            } catch (...) {
                continue; // malformed status
            }
        } else continue;

        doctors.push_back(doc);
    }
    cout << "Loaded " << doctors.size() << " doctors from " << filename << endl;
}
int doctorCount = 0;
int patientCount = 0;
int appointmentCount = 0;

// --- 17 & 19: Load Doctor Records & Assign Doctor ---
void manageDoctors() {
    cout << "\n--- Load & Manage Doctors (17 & 19) ---\n";

    // Load doctor records from file
    ifstream file("doctors.txt");
    if (!file) {
        cout << "No existing doctor records found.\n";
    } else {
        while (file >> doctors[doctorCount].id) {
            file.ignore();
            getline(file, doctors[doctorCount].name);
            getline(file, doctors[doctorCount].specialization);
            getline(file, doctors[doctorCount].contact);
            getline(file, doctors[doctorCount].schedule);
            doctors[doctorCount].available = true;
            doctorCount++;
        }
        file.close();
        cout << "Doctor records loaded successfully.\n";
    }

    // Assign Doctor to Patient
    int patientId, doctorId;
    cout << "\nEnter Patient ID: ";
    cin >> patientId;
    cout << "Enter Doctor ID: ";
    cin >> doctorId;

    bool patientFound = false, doctorFound = false;

    for (int i = 0; i < patientCount; i++) {
        if (patients[i].id == patientId) {
            patientFound = true;
            patients[i].assignedDoctorId = doctorId;
            break;
        }
    }

    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].id == doctorId) {
            doctorFound = true;
            break;
        }
    }

    if (!patientFound || !doctorFound) {
        cout << "Invalid Patient or Doctor ID.\n";
    } else {
        cout << "Doctor ID " << doctorId << " has been assigned to Patient ID " << patientId << ".\n";
    }
}

// --- 20: Doctor Schedule Management ---
void addDoctorSchedule() {
    if (doctorCount >= MAX_DOCTORS) {
        cout << "Doctor list full.\n";
        return;
    }
    
    cout << "\n--- Add Doctor Schedule (20) ---\n";
    cout << "Enter Doctor ID: ";
    cin >> doctors[doctorCount].id;
    cin.ignore();
    
    cout << "Enter Name: ";
    getline(cin, doctors[doctorCount].name);
    
    cout << "Enter Specialization: ";
    getline(cin, doctors[doctorCount].specialization);
    
    cout << "Enter Contact: ";
    getline(cin, doctors[doctorCount].contact);
    
    cout << "Enter Schedule (e.g. 9AM-5PM): ";
    getline(cin, doctors[doctorCount].schedule);
    
    doctors[doctorCount].available = true;
    doctorCount++;
    cout << "Doctor schedule added.\n";
}

    int choice;
    do {
        cout << "\n=== Doctor Management Menu ===\n"
             << "1. Add Doctor\n2. Edit Doctor\n3. Display Doctors\n4 Delete Doctor\n5 Check Doctor Availability\n 6. Exit\n"
             << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: addDoctor(); break;
            case 2: editDoctor(); break;
            case 3: displayDoctors(); break;
            case 4: deleteDoctor(); break;
            case 5: checkDoctorAvailability(); break;
            case 6: cout << "Exiting Doctor Module...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}
