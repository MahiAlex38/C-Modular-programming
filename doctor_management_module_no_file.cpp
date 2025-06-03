#include <iostream>  // For input and output
#include <string>    // To use string data type
#include <regex>     // To perform pattern matching for validation
#include <vector>    // To store list of doctors in memory

using namespace std;

// Enum to represent doctor's current working status
enum DoctorStatus { ACTIVE, ON_LEAVE, RETIRED };

// Structure to hold doctor information
struct Doctor {
    int id;  // Doctor ID
    string name;  // Doctor Name
    string specialization;  // Doctor Specialization
    string contact;  // Doctor Contact Number
    DoctorStatus status;  // Doctor Status
};

// List to store doctors in memory
vector<Doctor> doctors;

// Hardcoded manager ID for authentication
const string MANAGER_ID = "admin123";

// Validates if a string contains only letters and spaces
bool isAlpha(const string& str) {
    return regex_match(str, regex("^[A-Za-z ]+$"));
}

// Validates if a string contains only numbers (for ID)
bool isNumeric(const string& str) {
    return regex_match(str, regex("^\\d+$"));
}

// Validates contact number to be exactly 10 digits, ignoring '-' and '/'
bool isValidContact(const string& str) {
    string cleaned;
    for (char ch : str) {
        if (isdigit(ch)) cleaned += ch;  // Keep only digits
    }
    return cleaned.length() == 10;  // Contact must be 10 digits
}

// Authenticate manager before sensitive actions
bool authenticateManager() {
    string enteredID;
    cout << "Enter Manager ID to continue: ";
    cin >> enteredID;
    return enteredID == MANAGER_ID;  // Check if entered ID matches the hardcoded manager ID
}

// Add a new doctor record
void addDoctor() {
    if (!authenticateManager()) {
        cout << "Unauthorized. Only managers can add doctors.\n";
        return;  // Exit if manager ID is invalid
    }

    Doctor doc;
    string input;

    cout << "Enter Doctor ID (numeric): ";
    cin >> input;
    while (!isNumeric(input)) {
        cout << "Invalid ID. Enter numbers only: ";
        cin >> input;  // Validate doctor ID
    }
    doc.id = stoi(input);  // Convert string to integer

    cin.ignore();  // Clear leftover newline from input buffer

    cout << "Enter Doctor Name (letters only): ";
    getline(cin, doc.name);
    while (!isAlpha(doc.name)) {
        cout << "Invalid name. Enter letters only: ";
        getline(cin, doc.name);  // Validate name
    }

    cout << "Enter Specialization (letters only): ";
    getline(cin, doc.specialization);
    while (!isAlpha(doc.specialization)) {
        cout << "Invalid specialization. Enter letters only: ";
        getline(cin, doc.specialization);  // Validate specialization
    }

    cout << "Enter Contact Number (10 digits, can include '-' or '/'): ";
    getline(cin, doc.contact);
    while (!isValidContact(doc.contact)) {
        cout << "Invalid contact. Re-enter: ";
        getline(cin, doc.contact);  // Validate contact number
    }

    doc.status = ACTIVE;  // Default status is Active
    cout << "Default status is ACTIVE.\n";

    doctors.push_back(doc);  // Add doctor to the list
    cout << "Doctor added successfully!\n";
}

// Edit doctor status
void editStatus(Doctor& doc) {
    int statusChoice;

    cout << "\nCurrent status: ";
    if (doc.status == ACTIVE) cout << "Active";
    else if (doc.status == ON_LEAVE) cout << "On Leave";
    else cout << "Retired";

    cout << "\nSelect new status:\n";
    cout << "1. Active\n";
    cout << "2. On Leave\n";
    cout << "3. Retired\n";
    cout << "4. Cancel\n";
    cout << "Enter your choice: ";
    cin >> statusChoice;

    switch (statusChoice) {
        case 1: doc.status = ACTIVE; cout << "Status changed to Active.\n"; break;
        case 2: doc.status = ON_LEAVE; cout << "Status changed to On Leave.\n"; break;
        case 3: doc.status = RETIRED; cout << "Status changed to Retired.\n"; break;
        case 4: cout << "Status update canceled.\n"; break;
        default: cout << "Invalid choice. Status not changed.\n";
    }
}

// Edit doctor record by Doctor ID
void editDoctor() {
    if (!authenticateManager()) {
        cout << "Unauthorized. Only managers can edit doctors.\n";
        return;  // Exit if manager ID is invalid
    }

    if (doctors.empty()) {
        cout << "No doctor records to edit.\n";
        return;  // Exit if no doctors are in the list
    }

    int doctorId;
    cout << "Enter Doctor ID to edit: ";
    cin >> doctorId;  // Input doctor ID to edit

    // Find the doctor by ID
    bool found = false;
    for (Doctor& doc : doctors) {
        if (doc.id == doctorId) {
            found = true;
            string input;
            int choice;

            do {
                cout << "\nEditing Doctor: " << doc.name << "\n";
                cout << "1. Name\n";
                cout << "2. Specialization\n";
                cout << "3. Contact\n";
                cout << "4. Status\n";
                cout << "5. Done\n";
                cout << "Enter your choice: ";
                cin >> choice;
                cin.ignore();  // Clear input buffer

                switch (choice) {
                    case 1:
                        cout << "Enter new name: ";
                        getline(cin, doc.name);
                        while (!isAlpha(doc.name)) {
                            cout << "Invalid name. Enter letters only: ";
                            getline(cin, doc.name);  // Validate name
                        }
                        break;
                    case 2:
                        cout << "Enter new specialization: ";
                        getline(cin, doc.specialization);
                        while (!isAlpha(doc.specialization)) {
                            cout << "Invalid specialization. Enter letters only: ";
                            getline(cin, doc.specialization);  // Validate specialization
                        }
                        break;
                    case 3:
                        cout << "Enter new contact number: ";
                        getline(cin, doc.contact);
                        while (!isValidContact(doc.contact)) {
                            cout << "Invalid contact. Re-enter: ";
                            getline(cin, doc.contact);  // Validate contact number
                        }
                        break;
                    case 4:
                        editStatus(doc);  // Allow status change
                        break;
                    case 5:
                        cout << "Editing complete.\n";
                        break;
                    default:
                        cout << "Invalid choice.\n";
                }
            } while (choice != 5);
            break;
        }
    }

    if (!found) {
        cout << "No doctor found with the given ID.\n";  // Display error if doctor is not found
    }
}

// Display all doctors
void displayDoctors() {
    if (doctors.empty()) {
        cout << "No doctor records to display.\n";
        return;  // Exit if no doctors are in the list
    }

    for (int i = 0; i < doctors.size(); ++i) {
        Doctor& doc = doctors[i];  // Get the doctor details
        cout << "\nDoctor ID: " << doc.id << "\n";
        cout << "Name: " << doc.name << "\n";
        cout << "Specialization: " << doc.specialization << "\n";
        cout << "Contact: " << doc.contact << "\n";
        cout << "Status: ";
        if (doc.status == ACTIVE) cout << "Active\n";
        else if (doc.status == ON_LEAVE) cout << "On Leave\n";
        else cout << "Retired\n";
    }
}

// View doctors by selected status
void viewDoctorsByStatus() {
    if (doctors.empty()) {
        cout << "No doctor records available.\n";
        return;  // Exit if no doctors are in the list
    }

    int statusChoice;
    cout << "\nSelect status to view:\n";
    cout << "1. Active\n";
    cout << "2. On Leave\n";
    cout << "3. Retired\n";
    cout << "Enter your choice: ";
    cin >> statusChoice;

    DoctorStatus filterStatus;
    switch (statusChoice) {
        case 1: filterStatus = ACTIVE; break;
        case 2: filterStatus = ON_LEAVE; break;
        case 3: filterStatus = RETIRED; break;
        default:
            cout << "Invalid choice.\n";
            return;  // Exit if status choice is invalid
    }

    bool found = false;
    for (const Doctor& doc : doctors) {
        if (doc.status == filterStatus) {
            cout << "\nID: " << doc.id
                 << "\nName: " << doc.name
                 << "\nSpecialization: " << doc.specialization
                 << "\nContact: " << doc.contact << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No doctors found with the selected status.\n";
    }
}

// Main menu for user interaction
int main() {
    int choice;
    do {
        cout << "\n--- Doctor Management Menu ---\n";
        cout << "1. Add Doctor\n";
        cout << "2. Edit Doctor\n";
        cout << "3. Display All Doctors\n";
        cout << "4. View Doctors by Status\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Handle user choice
        switch (choice) {
            case 1: addDoctor(); break;
            case 2: editDoctor(); break;
            case 3: displayDoctors(); break;
            case 4: viewDoctorsByStatus(); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);  // Loop until user chooses to exit

    return 0;
}


