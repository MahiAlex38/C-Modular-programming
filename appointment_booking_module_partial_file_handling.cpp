#include <iostream>      // For input/output operations
#include <string>        // To use string variables
#include <vector>        // To hold lists of appointments
#include <regex>         // To perform input validations
#include <algorithm>     // For string transformations like tolower

using namespace std;

// Enum for appointment status
enum AppointmentStatus { PENDING, ACTIVE, RETIRED };

// Structure for patient information
struct Patient {
    int id;
    string name;
    string gender;
    string contact;
    string medicalHistory;
};

// Structure for doctor information
struct Doctor {
    int id;
    string name;
    string specialization;
    string contact;
    AppointmentStatus status;  // Doctor status: ACTIVE, ON_LEAVE, RETIRED
};

// Structure for appointment
struct Appointment {
    int appointmentID;
    Patient patient;
    Doctor doctor;
    string date;   // Format: YYYY-MM-DD
    string time;   // Format: HH:MM
    string reason; // Reason: Consultation / Check-up
    AppointmentStatus status;
};

// Holds all appointments
vector<Appointment> appointments;

// Stub functions to simulate lookup (to be implemented via file reading in actual use)
bool patientExists(int id) {
    return id == 101; // Simulated patient exists if ID is 101
}

bool doctorExists(int id) {
    return id == 202; // Simulated doctor exists if ID is 202
}

AppointmentStatus getDoctorStatus(int id) {
    return ACTIVE; // Simulated doctor is ACTIVE
}

// Validates if a string is all letters
bool isAlpha(const string& str) {
    return regex_match(str, regex("^[A-Za-z ]+$"));
}

// Checks if input is numeric
bool isNumeric(const string& str) {
    return regex_match(str, regex("^\\d+$"));
}

// Validates contact number format (ignoring '/' or '-')
bool isValidContact(const string& str) {
    string cleaned;
    for (char ch : str) {
        if (isdigit(ch)) cleaned += ch;
    }
    return cleaned.length() == 10;
}

// Validates and provides error for date input
bool isValidDate(const string& date) {
    regex dateFormat("^\\d{4}-\\d{2}-\\d{2}$");
    if (!regex_match(date, dateFormat)) return false;

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    bool valid = true;

    if (year < 2017) {
        cout << "Please enter a valid year (2017 or later).\n";
        valid = false;
    }
    if (month < 1 || month > 12) {
        cout << "Please enter a month between 1 and 12.\n";
        valid = false;
    }
    if (day < 1 || day > 30) {
        cout << "Please enter a day between 1 and 30.\n";
        valid = false;
    }
    return valid;
}

// Validates and provides error for time input
bool isValidTime(const string& time) {
    regex timeFormat("^\\d{2}:\\d{2}$");
    if (!regex_match(time, timeFormat)) return false;

    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));

    bool valid = true;

    if (hour < 1 || hour > 23) {
        cout << "Please enter an hour between 1 and 23. We use military time.\n";
        valid = false;
    }
    if (minute < 0 || minute > 59) {
        cout << "Please enter minutes between 00 and 59.\n";
        valid = false;
    }
    return valid;
}

// Checks if an appointment already exists at the same date and time
bool isTimeSlotTaken(const string& date, const string& time) {
    for (auto& appt : appointments) {
        if (appt.date == date && appt.time == time) {
            return true;
        }
    }
    return false;
}

// Function to book an appointment
void bookAppointment() {
    Appointment appt;
    string input;

    // Get Appointment ID
    cout << "Enter Appointment ID (numeric): ";
    cin >> input;
    while (!isNumeric(input)) {
        cout << "Invalid. Enter numbers only: ";
        cin >> input;
    }
    appt.appointmentID = stoi(input);
    cin.ignore();

    // Get Patient ID and verify if patient exists
    cout << "Enter Patient ID: ";
    getline(cin, input);
    while (!isNumeric(input) || !patientExists(stoi(input))) {
        cout << "Invalid or patient not found. Please enter a valid patient ID: ";
        getline(cin, input);
    }
    appt.patient.id = stoi(input);

    // Get Doctor ID and verify if doctor exists
    cout << "Enter Doctor ID: ";
    getline(cin, input);
    while (!isNumeric(input) || !doctorExists(stoi(input))) {
        cout << "Invalid or doctor not found. Please enter a valid doctor ID: ";
        getline(cin, input);
    }
    appt.doctor.id = stoi(input);

    // Fetch doctor status
    AppointmentStatus docStatus = getDoctorStatus(appt.doctor.id);
    if (docStatus == RETIRED) {
        cout << "Doctor is retired and cannot take appointments.\n";
        return;
    } else if (docStatus == PENDING) {
        cout << "Doctor is currently on leave.\n";
        return;
    }

    // Get and validate appointment date
    cout << "Enter Appointment Date (YYYY-MM-DD): ";
    getline(cin, appt.date);
    while (!isValidDate(appt.date)) {
        cout << "Try again: ";
        getline(cin, appt.date);
    }

    // Get and validate appointment time
    cout << "Enter Appointment Time (HH:MM): ";
    getline(cin, appt.time);
    while (!isValidTime(appt.time)) {
        cout << "Try again: ";
        getline(cin, appt.time);
    }

    // Check for double booking
    if (isTimeSlotTaken(appt.date, appt.time)) {
        cout << "Another appointment already exists at this time.\n";
        return;
    }

    // Choose appointment reason
    cout << "Enter reason for appointment (Consultation/Check-up): ";
    getline(cin, appt.reason);
    while (appt.reason != "Consultation" && appt.reason != "Check-up") {
        cout << "Only 'Consultation' or 'Check-up' allowed. Try again: ";
        getline(cin, appt.reason);
    }

    // Assign simulated patient and doctor info
    appt.patient.name = "Simulated Patient";
    appt.patient.gender = "Male";
    appt.patient.contact = "9876543210";
    appt.patient.medicalHistory = "No major history";

    appt.doctor.name = "Simulated Doctor";
    appt.doctor.specialization = "Cardiology";
    appt.doctor.contact = "0123456789";

    appt.status = ACTIVE;

    appointments.push_back(appt);  // Save the appointment

    // Show appointment summary
    cout << "\n=== Appointment Booked Successfully ===\n";
    cout << "Date: " << appt.date << ", Time: " << appt.time << "\n";
    cout << "Reason: " << appt.reason << "\n";

    cout << "\n--- Patient Info ---\n";
    cout << "ID: " << appt.patient.id << ", Name: " << appt.patient.name << "\n";
    cout << "Gender: " << appt.patient.gender << ", Contact: " << appt.patient.contact << "\n";
    cout << "Medical History: " << appt.patient.medicalHistory << "\n";

    cout << "\n--- Doctor Info ---\n";
    cout << "ID: " << appt.doctor.id << ", Name: " << appt.doctor.name << "\n";
    cout << "Specialization: " << appt.doctor.specialization << ", Contact: " << appt.doctor.contact << "\n";
}

// Cancel appointment by ID
void cancelAppointment() {
    if (appointments.empty()) {
        cout << "No appointments to cancel.\n";
        return;
    }

    string input;
    cout << "Enter Appointment ID to cancel: ";
    cin >> input;
    while (!isNumeric(input)) {
        cout << "Invalid ID. Enter numbers only: ";
        cin >> input;
    }

    int id = stoi(input);
    for (size_t i = 0; i < appointments.size(); ++i) {
        if (appointments[i].appointmentID == id) {
            appointments.erase(appointments.begin() + i);
            cout << "Appointment canceled successfully.\n";
            return;
        }
    }

    cout << "No appointment found with that ID.\n";
}

// Main menu
int main() {
    int choice;
    do {
        cout << "\n=== Appointment Management Menu ===\n";
        cout << "1. Book Appointment\n";
        cout << "2. Cancel Appointment\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(); // clear input buffer

        switch (choice) {
            case 1: bookAppointment(); break;
            case 2: cancelAppointment(); break;
            case 3: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return 0;
}


