#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>


using namespace std;

// Declare internal appointment functions before using them
void bookAppointment();
void viewAppointments();
void cancelAppointment();
void searchPatient();
void runAppointmentModule() {
    int choice;

    while (true) {
        cout << "\n--- Appointment Management Menu ---\n";
        cout << "1. Book Appointment\n";
        cout << "2. View All Appointments\n";
        cout << "3. Cancel Appointment\n";
         cout << "4. Search Patient\n";
         cout << "5. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: bookAppointment(); break;
            case 2: viewAppointments(); break;
            case 3: cancelAppointment(); break;
            case 4: searchPatient(); break;
            case 5: return;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}

bool hasConflict(const Appointment& newAppt) {
    for (const auto& appt : appointments) {
        if (appt.doctor.id == newAppt.doctor.id && 
            appt.date == newAppt.date && 
            appt.time == newAppt.time) {
            return true;
        }
    }
    return false;
}

void bookAppointment() {
    Appointment newAppt;

    cout << "Enter Appointment ID: ";
    while (!(cin >> newAppt.appointmentID)) {
        cout << "Invalid input. Try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    // Patient info
    cout << "Enter Patient Name: ";
    getline(cin, newAppt.patient.name);
    cout << "Enter Patient Contact: ";
    getline(cin, newAppt.patient.contact);

    // Doctor info
    cout << "Enter Doctor ID: ";
    while (!(cin >> newAppt.doctor.id)) {
        cout << "Invalid input. Try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    bool doctorFound = false;
    for (const auto& doc : doctors) {
        if (doc.id == newAppt.doctor.id) {
            newAppt.doctor = doc;
            doctorFound = true;
            break;
        }
    }

    if (!doctorFound) {
        cout << "Doctor not found.\n";
        return;
    }

    if (newAppt.doctor.status != ACTIVE_DOCTOR) {
        cout << "Selected doctor is not available for appointments.\n";
        return;
    }

    cout << "Enter Appointment Date (YYYY-MM-DD): ";
    getline(cin, newAppt.date);
    cout << "Enter Appointment Time (HH:MM): ";
    getline(cin, newAppt.time);
    cout << "Enter Reason for Appointment: ";
    getline(cin, newAppt.reason);

    if (hasConflict(newAppt)) {
        cout << "Appointment slot is already booked for this doctor.\n";
        return;
    }

    appointments.push_back(newAppt);
    cout << "Appointment booked successfully.\n";
}

void viewAppointments() {
    if (appointments.empty()) {
        cout << "No appointments found.\n";
        return;
    }

    for (const auto& appt : appointments) {
        cout << "\n--- Appointment ---\n";
        cout << "ID: " << appt.appointmentID << "\n";
        cout << "Patient: " << appt.patient.name << ", Contact: " << appt.patient.contact << "\n";
        cout << "Doctor: " << appt.doctor.name << " (" << appt.doctor.specialization << ")\n";
        cout << "Date: " << appt.date << ", Time: " << appt.time << "\n";
        cout << "Reason: " << appt.reason << "\n";
    }
}

void cancelAppointment() {
    int id;
    cout << "Enter Appointment ID to cancel: ";
    cin >> id;
    cin.ignore();

    for (auto it = appointments.begin(); it != appointments.end(); ++it) {
        if (it->appointmentID == id) {
            appointments.erase(it);
            cout << "Appointment cancelled successfully.\n";
            return;
        }
    }

    cout << "Appointment not found.\n";
}

//SEARCHING PATIENT
void searchPatient() {
    int choice;
    string searchTerm;
    bool found = false;

    cout << "\nSearch Patient by:\n";
    cout << "1. Patient ID\n";
    cout << "2. Patient Name\n";
    cout << "3. Patient Contact\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            {
                int id;
                cout << "Enter Patient ID: ";
                cin >> id;
                cin.ignore();
                for (const auto& patient : patients) {
                    if (patient.id == id) {
                        cout << "\nPatient Found:\n";
                        cout << "ID: " << patient.id << "\n";
                        cout << "Name: " << patient.name << "\n";
                        cout << "Contact: " << patient.contact << "\n";
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Patient not found.\n";
            }
            break;
        case 2:
            cout << "Enter Patient Name: ";
            getline(cin, searchTerm);
            for (const auto& patient : patients) {
                if (patient.name == searchTerm) {
                    cout << "\nPatient Found:\n";
                    cout << "ID: " << patient.id << "\n";
                    cout << "Name: " << patient.name << "\n";
                    cout << "Contact: " << patient.contact << "\n";
                    found = true;
                }
            }
            if (!found) cout << "Patient not found.\n";
            break;
        case 3:
            cout << "Enter Patient Contact: ";
            getline(cin, searchTerm);
            for (const auto& patient : patients) {
                if (patient.contact == searchTerm) {
                    cout << "\nPatient Found:\n";
                    cout << "ID: " << patient.id << "\n";
                    cout << "Name: " << patient.name << "\n";
                    cout << "Contact: " << patient.contact << "\n";
                    found = true;
                }
            }
            if (!found) cout << "Patient not found.\n";
            break;
        default:
            cout << "Invalid choice.\n";
    }
}
void loadPatientsFromFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Failed to open patient data file: " << filename << std::endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string token;
        Patient p;

        // Read ID
        if (getline(ss, token, ',')) {
    try {
        p.id = stoi(token);
    } catch (const invalid_argument&) {
        cerr << "Invalid Patient ID: " << token << "\n";
        continue; // or handle error
    }
} else continue;

        // Read Name
        if (getline(ss, token, ',')) {
            p.name = token;
        } else continue;

        // Read Age
        if (getline(ss, token, ',')) {
            try {
        p.age = stoi(token);
            }
                catch (const invalid_argument&) {
        cerr << "Invalid Age: " << token << "\n";
        continue; 
    }
        } else continue;

        // Read Gender
        if (getline(ss, token, ',')) {
            p.gender = token;
        } else continue;

        // Read Contact
        if (getline(ss, token, ',')) {
            p.contact = token;
        } else continue;

        // Read Medical History
        if (getline(ss, token, ',')) {
            p.medicalHistory = token;
        } else continue;

        patients.push_back(p);
    }

    cout << "Loaded " << patients.size() << " patients from file.\n";
}
