#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"
#include <iostream>
#include <limits>
using namespace std;

void runPatientModule() {
    int choice;
    do {
        cout << "\n--- Patient Management Module ---\n";
        cout << "1. Add Patient\n";
        cout << "2. Edit Patient\n";
        cout << "3. Delete Patient\n";
        cout << "4. List All Patients\n";
        cout << "5. Check Admission Status\n";
        cout << "6. Discharge Patient\n";
        cout << "7. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                Patient p;
                string input;

                cout << "Enter ID: ";
                getline(cin, input);
                while (!isNumeric(input)) {
                    cout << "Invalid ID. Enter a number: ";
                    getline(cin, input);
                }
                p.id = stoi(input);

                cout << "Enter Name: ";
                getline(cin, p.name);
                while (!isAlpha(p.name)) {
                    cout << "Invalid name. Enter only letters: ";
                    getline(cin, p.name);
                }

                cout << "Enter Age: ";
                getline(cin, input);
                while (!isNumeric(input)) {
                    cout << "Invalid age. Enter a number: ";
                    getline(cin, input);
                }
                p.age = stoi(input);

                cout << "Enter Gender: ";
                getline(cin, p.gender);

                cout << "Enter Contact: ";
                getline(cin, p.contact);
                while (!isValidContact(p.contact)) {
                    cout << "Invalid contact. Enter again: ";
                    getline(cin, p.contact);
                }

                cout << "Enter Medical History: ";
                getline(cin, p.medicalHistory);
                while (!isAlpha(p.medicalHistory)) {
                    cout << "Invalid input. Letters only: ";
                    getline(cin, p.medicalHistory);
                }

                p.admissionStatus = ADMITTED;
                patients.push_back(p);
                cout << "Patient added successfully!\n";
                break;
            }
            case 2: {
                int id;
                cout << "Enter Patient ID to edit: ";
                cin >> id;
                cin.ignore();
                bool found = false;
                for (auto& p : patients) {
                    if (p.id == id) {
                        found = true;
                        cout << "Editing patient " << p.name << "\n";
                        cout << "Enter new contact: ";
                        getline(cin, p.contact);
                        cout << "Enter new medical history: ";
                        getline(cin, p.medicalHistory);
                        cout << "Patient updated.\n";
                        break;
                    }
                }
                if (!found) cout << "Patient not found.\n";
                break;
            }
            case 3: {
                int id;
                cout << "Enter Patient ID to delete: ";
                cin >> id;
                cin.ignore();
                auto it = remove_if(patients.begin(), patients.end(), [id](const Patient& p) {
                    return p.id == id;
                });
                if (it != patients.end()) {
                    patients.erase(it, patients.end());
                    cout << "Patient deleted.\n";
                } else {
                    cout << "Patient not found.\n";
                }
                break;
            }
            case 4: {
                for (const auto& p : patients) {
                    cout << "\nID: " << p.id
                         << "\nName: " << p.name
                         << "\nAge: " << p.age
                         << "\nGender: " << p.gender
                         << "\nContact: " << p.contact
                         << "\nMedical History: " << p.medicalHistory
                         << "\nAdmission Status: " << (p.admissionStatus == ADMITTED ? "Admitted" : "Discharged") << endl;
                }
                break;
            }
            case 5: {
                int id;
                cout << "Enter Patient ID to check admission status: ";
                cin >> id;
                cin.ignore();
                bool found = false;
                for (auto& p : patients) {
                    if (p.id == id) {
                        found = true;
                        cout << "Admission Status: " << (p.admissionStatus == ADMITTED ? "Admitted" : "Discharged") << "\n";
                        break;
                    }
                }
                if (!found) cout << "Patient not found.\n";
                break;
            }
            case 6: {
                int id;
                cout << "Enter Patient ID to discharge: ";
                cin >> id;
                cin.ignore();
                bool found = false;
                for (auto& p : patients) {
                    if (p.id == id) {
                        found = true;
                        if (p.admissionStatus != ADMITTED) {
                            cout << "Patient is not currently admitted.\n";
                            break;
                        }
                        p.admissionStatus = DISCHARGED;

                        cout << "\n=== PATIENT DISCHARGE SUMMARY ===\n";
                        cout << "Patient ID: " << p.id << "\n";
                        cout << "Name: " << p.name << "\n";
                        cout << "Age: " << p.age << "\n";
                        cout << "Gender: " << p.gender << "\n";
                        cout << "Contact: " << p.contact << "\n";
                        cout << "Medical History: " << p.medicalHistory << "\n";
                        cout << "Admission Status: Discharged\n";
                        
                        cout << "\nRecommendations:\n";
                        cout << "1. Follow-up appointment scheduled as needed\n";
                        cout << "2. Continue prescribed medications\n";
                        cout << "3. Rest and recovery guidelines provided\n";
                        cout << "\nDischarge Date: " << getCurrentDate() << "\n";
                        cout << "=================================\n";
                        cout << "\nPatient has been successfully discharged.\n";
                        break;
                    }
                }
                if (!found) cout << "Patient not found.\n";
                break;
            }
            case 7:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 7);
}
