#include <iostream> //  lets us print stuff to the screen and get input from the keyboard
#include <vector>   // This is like a special list that can grow or shrink
#include <string>   // This is how we work with words and sentences
#include <limits>   // We need this to help clean up the keyboard input
#include <algorithm> // This has some helpful tools for working with our lists

using namespace std; // This line basically says we're going to use common C++ stuff without having to type "std::" in front of it all the time


class Patient {
public:
    int id;             
    string name;         
    int age;             
    string gender;       
    string contactInfo;  
    string medicalHistory; 

    // This is like a special function that helps us create a new patient
    Patient(int id, const string& name, int age, const string& gender, const string& contactInfo, const string& medicalHistory)
        : id(id), name(name), age(age), gender(gender), contactInfo(contactInfo), medicalHistory(medicalHistory) {}

    // This is another way to create a patient, but with nothing filled in yet
    Patient() : id(0), name(""), age(0), gender(""), contactInfo(""), medicalHistory("") {}

    // shows patient's info 
    void displayPatientInfo() const {
        cout << "Patient ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Age: " << age << "\n";
        cout << "Gender: " << gender << "\n";
        cout << "Contact Information: " << contactInfo << "\n";
        cout << "Medical History: " << medicalHistory << "\n";
        cout << "------------------------" << endl; 
    }

    // checks if two patients have the same ID
    bool operator==(int otherId) const {
        return id == otherId;
    }
};

// This function helps makes sure the input is actually a number
int getValidInt(const string& question) {
    int number;
    while (true) {
        cout << question; 
        if (cin >> number) { 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clean up any extra stuff the user might have typed
            return number; // If we got a number, send it back
        } else {
            cout << "That's not a number! Please try again." << endl; 
            cin.clear(); // Reset the error so we can try again
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clean up the bad input
        }
    }
}

// This function is for getting words or sentences from the user, and making sure they typed something
string getValidString(const string& question) {
    string answer;
    while (true) {
        cout << question; 
        getline(cin, answer); 
        if (!answer.empty()) { // Check if the text they typed is not empty
            return answer; // If it's not empty, send it back
        } else {
            cout << "Input cannot be empty. Please type something." << endl; // Tell them if they didn't type anything
        }
    }
}

// This function makes sure the user enters text with only letters and spaces
string getValidName(const string& question) {
    string name;
    while (true) {
        cout << question; 
        getline(cin, name); // Read the whole line of text
        if (!name.empty()) { // Make sure they typed something
            bool allLettersOrSpaces = true; // Let's assume it's all good
            for (char c : name) { // Go through each letter in the name
                if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) { // Check if it's NOT a letter or a space
                    allLettersOrSpaces = false; // If it's something else, the name is invalid
                    break; 
                }
            }
            if (allLettersOrSpaces) { 
                return name; 
            } else {
                cout << "Invalid input. Please use only letters and spaces for the name." << endl; 
            }
        } else {
            cout << "Name cannot be empty. Please enter a name." << endl; 
        }
    }
}

// This function makes sure the user enters "male" or "female" (case-insensitive)
string getValidGender(const string& question) {
    string gender;
    while (true) {
        cout << question; // Ask for the gender
        getline(cin, gender); 
        transform(gender.begin(), gender.end(), gender.begin(), ::tolower); // Make it lowercase to easily check
        if (gender == "male" || gender == "female") { // Check if it's "male" or "female"
            return gender; 
        } else if (gender.empty()) {
            cout << "Gender cannot be empty. Please enter 'male' or 'female'." << endl; 
        } else {
            cout << "Invalid gender. Please enter 'male' or 'female'." << endl; 
        }
    }
}

// This function makes sure the user enters a 10-digit phone number
string getValidPhoneNumber(const string& question) {
    string phoneNumber;
    while (true) {
        cout << question; // Ask for the phone number
        getline(cin, phoneNumber); // Get what they typed
        if (phoneNumber.length() == 10) { // Check if it's exactly 10 characters long
            bool allDigits = true; // Let's assume it's all numbers
            for (char c : phoneNumber) { // Go through each character
                if (c < '0' || c > '9') { // Check if the character is NOT a digit (0-9)
                    allDigits = false; // If it's not a digit, the phone number is invalid
                    break; 
                }
            }
            if (allDigits) { // If all characters were digits
                return phoneNumber; // Send back the valid phone number
            } else {
                cout << "Invalid phone number. Please enter exactly 10 digits (numbers only)." << endl; // Tell them if they used other characters
            }
        } else if (phoneNumber.empty()) {
            cout << "Phone number cannot be empty. Please enter a 10-digit number." << endl; 
        } else {
            cout << "Invalid phone number. Please enter exactly 10 digits." << endl; 
        }
    }
}

// This function is how we add a new patient to our list
void addPatient(vector<Patient>& patientList) {
    int patientId = getValidInt("Enter Patient ID: ");

    // Let's check if we already have a patient with this ID
    bool idExists = false;
    for (const auto& patient : patientList) {
        if (patient.id == patientId) {
            idExists = true;
            break;
        }
    }
    if (idExists) {
        cout << " We already have a patient with that ID ." << endl;
        return; // Go back to the main menu
    }

    string patientName = getValidName("Enter Patient Name (letters and spaces only): ");
    int patientAge = getValidInt("Enter Patient Age: ");
    string patientGender = getValidGender("Enter Patient Gender (male/female): ");
    string patientContact = getValidPhoneNumber("Enter Patient Contact Information (10 digits): ");
    string patientMedicalHistory = getValidName("Enter Patient Medical History (letters and spaces only): ");

    // Create a new patient using the info we got
    Patient newPatient(patientId, patientName, patientAge, patientGender, patientContact, patientMedicalHistory);
    patientList.push_back(newPatient); // Add the new patient to our list
    cout << " Patient added." << endl;
}

// This function shows us all the patients in our list
void viewAllPatients(const vector<Patient>& patientList) {
    if (patientList.empty()) {
        cout << "Looks like we don't have any patients to show yet." << endl;
        return;
    }
    cout << "--- Here are all the patients ---" << endl;
    for (const auto& patient : patientList) {
        patient.displayPatientInfo(); // Show the info for each patient
    }
}

// This function lets us change the info of a patient, with a menu to choose what to update
void updatePatient(vector<Patient>& patientList) {
    if (patientList.empty()) {
        cout << "No patients to update right now." << endl;
        return;
    }

    int idToUpdate = getValidInt("Enter the ID of the patient you want to update: ");

    // Let's try to find the patient with that ID
    Patient* foundPatient = nullptr; // We'll use this to keep track if we find the patient
    for (auto& patient : patientList) {
        if (patient.id == idToUpdate) {
            foundPatient = &patient; // patients found!
            break;
        }
    }

    if (foundPatient != nullptr) {
        int choice;
        do {
            cout << "\n--- What information would you like to update for Patient ID " << foundPatient->id << " (" << foundPatient->name << ")? ---" << endl;
            cout << "1. Name" << endl;
            cout << "2. Age" << endl;
            cout << "3. Gender" << endl;
            cout << "4. Contact Information" << endl;
            cout << "5. Medical History" << endl;
            cout << "0. Go back to the main menu" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "That's not a valid option. Please enter a number from the menu." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1; // To keep the loop going if they enter something wrong
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1:
                    foundPatient->name = getValidName("Enter new Name (letters and spaces only): ");
                    cout << "Name updated!" << endl;
                    break;
                case 2:
                    foundPatient->age = getValidInt("Enter new Age: ");
                    cout << "Age updated!" << endl;
                    break;
                case 3:
                    foundPatient->gender = getValidGender("Enter new Gender (male/female): ");
                    cout << "Gender updated!" << endl;
                    break;
                case 4:
                    foundPatient->contactInfo = getValidPhoneNumber("Enter new Contact Information (10 digits): ");
                    cout << "Contact Information updated!" << endl;
                    break;
                case 5:
                    foundPatient->medicalHistory = getValidName("Enter new Medical History (letters and spaces only): ");
                    cout << "Medical History updated!" << endl;
                    break;
                case 0:
                    cout << "Going back to the main menu." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please enter a number from the menu." << endl;
            }
        } while (choice != 0); // Keep showing the update menu until they choose to go back
    } else {
        cout << "couldn't find a patient with that ID make sure u entered the correct one." << endl;
    }
}

// This function lets us remove a patient from our list
void deletePatient(vector<Patient>& patientList) {
    if (patientList.empty()) {
        cout << "Can't delete anyone, the list is empty!" << endl;
        return;
    }

    int idToDelete = getValidInt("Enter the ID of the patient you want to delete: ");

    // Let's find the patient again
    for (size_t i = 0; i < patientList.size(); ++i) {
        if (patientList[i].id == idToDelete) {
            patientList.erase(patientList.begin() + i); // Found them, let's remove them
            cout << " Patient deleted." << endl;
            return; 
        }
    }

    // If we went through the whole list and didn't find them
    cout << "Sorry, no patient found with that ID." << endl;
}

// This function just shows the menu to the user
void displayMenu() {
    cout << "\n--- Hospital Management System ---" << endl;
    cout << "1. Add Patient" << endl;
    cout << "2. View All Patients" << endl;
    cout << "3. Update Patient" << endl;
    cout << "4. Delete Patient" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    vector<Patient> patients; // This is our list to hold all the patients
    int choice;              // This will store what the user wants to do

    do {
        displayMenu();
        // Get the user's choice
        if (!(cin >> choice)) {
            cout << " That's not a valid option. Please enter a number between 0 and 4." << endl;
            cin.clear(); // Reset the error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clean up the input
            choice = -1; // Just to make sure the loop continues
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clean up the newline after the number

        switch (choice) {
            case 1: addPatient(patients); break; 
            case 2: viewAllPatients(patients); break;
            case 3: updatePatient(patients); break; 
            case 4: deletePatient(patients); break; 
            case 0: cout << " exiting the system,thank you for visiting!" << endl; break; 
            default: cout << "that's not something I know how to do. Please try again." << endl; 
        }
    } while (choice != 0); // Keep doing this until they choose to exit

    return 0; // Everything went smoothly!
}

