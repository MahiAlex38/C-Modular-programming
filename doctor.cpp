#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

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
