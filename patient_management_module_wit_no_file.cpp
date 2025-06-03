#include <iostream>
#include <string>
#include <vector>
#include <cctype>   // For isdigit and isalpha
#include <limits>   // For numeric_limits
#include <algorithm> // For std::remove

struct Patient {
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string contact;
    std::string medicalHistory;
};

std::vector<Patient> patients; // In-memory storage for patients

// Let's check if this contact is all digits and just 10 of them. If yes, it's valid!
bool isValidContact(const std::string& contact) {
    if (contact.length() != 10) return false;
    for (char c : contact) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Let's make sure the name only has alphabets and spaces, okay?
bool isAlpha(const std::string& str) {
    for (char c : str) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

// Checking if the gender is either "male" or "female" – no other options allowed!
bool isValidGender(const std::string& gender) {
    return (gender == "male" || gender == "female");
}

// Is the ID a number? Let's make sure it's all digits.
bool isValidId(const std::string& idStr) {
    for (char c : idStr) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Helper function to check if a patient ID already exists
bool patientIdExists(int id) {
    for (const auto& patient : patients) {
        if (patient.id == id) {
            return true;
        }
    }
    return false;
}

// Oops! It's time to add a brand new patient! Here we go...
void addPatient() {
    Patient newPatient;
    std::string idStr;

    // Let's get the Patient ID – it has to be a number and unique!
    while (true) {
        std::cout << "Enter Patient ID: ";
        std::cin >> idStr;
        if (!isValidId(idStr)) {
            std::cout << "Oops! Invalid Patient ID. It should be a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        newPatient.id = std::stoi(idStr);
        if (patientIdExists(newPatient.id)) {
            std::cout << "Oops! Patient ID already exists. Please enter a unique ID.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(); // Clear out that pesky newline
        break;
    }

    // Time for the Patient's Name – it should only have alphabets and spaces!
    while (true) {
        std::cout << "Enter Patient Name: ";
        std::getline(std::cin, newPatient.name);
        if (!isAlpha(newPatient.name)) {
            std::cout << "Oops! Invalid Name. It should contain only alphabets and spaces.\n";
            continue;
        }
        break;
    }

    // Next, let's ask for the Age – this one should be a number.
    while (true) {
        std::cout << "Enter Age: ";
        if (std::cin >> newPatient.age) {
            std::cin.ignore();
            break;
        } else {
            std::cout << "Oops! Invalid Age. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // Gender check – only "male" or "female" allowed.
    while (true) {
        std::cout << "Enter Gender (male/female): ";
        std::getline(std::cin, newPatient.gender);
        if (!isValidGender(newPatient.gender)) {
            std::cout << "Oops! Invalid Gender. It should be either 'male' or 'female'.\n";
            continue;
        }
        break;
    }

    // Now for the Contact Number – make sure it's exactly 10 digits!
    while (true) {
        std::cout << "Enter Contact Number (10 digits): ";
        std::getline(std::cin, newPatient.contact);
        if (!isValidContact(newPatient.contact)) {
            std::cout << "Oops! Invalid Contact Number. It should be exactly 10 digits.\n";
            continue;
        }
        break;
    }

    // Medical History time! It should only contain alphabets and spaces.
    while (true) {
        std::cout << "Enter Medical History: ";
        std::getline(std::cin, newPatient.medicalHistory);
        if (!isAlpha(newPatient.medicalHistory)) {
            std::cout << "Oops! Invalid Medical History. It should contain only alphabets and spaces.\n";
            continue;
        }
        break;
    }

    patients.push_back(newPatient); // Add the patient to the vector
    std::cout << "Yay! Patient added successfully!\n";
}

// Let's update this patient's details. Oops, let's make sure everything is right first!
bool editPatient(int id) {
    for (auto& patient : patients) {
        if (patient.id == id) {
            int choice;
            std::cout << "Found the patient! What would you like to edit?\n";
            std::cout << "1. Name\n";
            std::cout << "2. Age\n";
            std::cout << "3. Gender\n";
            std::cout << "4. Contact\n";
            std::cout << "5. Medical History\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            std::cin.ignore();  // Clear out that pesky newline

            switch (choice) {
                case 1:
                    while (true) {
                        std::cout << "Enter new Name: ";
                        std::getline(std::cin, patient.name);
                        if (!isAlpha(patient.name)) {
                            std::cout << "Oops! Invalid Name. It should contain only alphabets and spaces.\n";
                            continue;
                        }
                        break;
                    }
                    break;
                case 2:
                    while (true) {
                        std::cout << "Enter new Age: ";
                        if (std::cin >> patient.age) {
                            std::cin.ignore();
                            break;
                        } else {
                            std::cout << "Oops! Invalid Age. Please enter a number.\n";
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }
                    break;
                case 3:
                    while (true) {
                        std::cout << "Enter new Gender (male/female): ";
                        std::getline(std::cin, patient.gender);
                        if (!isValidGender(patient.gender)) {
                            std::cout << "Oops! Invalid Gender. It should be either 'male' or 'female'.\n";
                            continue;
                        }
                        break;
                    }
                    break;
                case 4:
                    while (true) {
                        std::cout << "Enter new Contact Number (10 digits): ";
                        std::getline(std::cin, patient.contact);
                        if (!isValidContact(patient.contact)) {
                            std::cout << "Oops! Invalid Contact Number. It should be exactly 10 digits.\n";
                            continue;
                        }
                        break;
                    }
                    break;
                case 5:
                    while (true) {
                        std::cout << "Enter new Medical History: ";
                        std::getline(std::cin, patient.medicalHistory);
                        if (!isAlpha(patient.medicalHistory)) {
                            std::cout << "Oops! Invalid Medical History. It should contain only alphabets and spaces.\n";
                            continue;
                        }
                        break;
                    }
                    break;
                default:
                    std::cerr << "Oops! Invalid choice.\n";
                    return false;
            }

            std::cout << "Hooray! Patient record updated successfully.\n";
            return true;
        }
    }

    std::cout << "Oops! Patient with ID " << id << " not found.\n";
    return false;
}

// Let's delete this patient from the records. Bye-bye, patient!
bool deletePatient(int id) {
    for (auto it = patients.begin(); it != patients.end(); ++it) {
        if (it->id == id) {
            patients.erase(it);
            std::cout << "Patient with ID " << id << " deleted successfully.\n";
            return true;
        }
    }

    std::cout << "Oops! Patient with ID " << id << " not found.\n";
    return false;
}

// Let's list all the patients. It's showtime!
void listAllPatients() {
    if (patients.empty()) {
        std::cout << "No patients records found.\n";
        return;
    }

    for (const auto& patient : patients) {
        std::cout << "ID: " << patient.id << "\n";
        std::cout << "Name: " << patient.name << "\n";
        std::cout << "Age: " << patient.age << "\n";
        std::cout << "Gender: " << patient.gender << "\n";
        std::cout << "Contact: " << patient.contact << "\n";
        std::cout << "Medical History: " << patient.medicalHistory << "\n";
        std::cout << "---------------------------------\n";
    }
}

int main() {
    int choice;

    // Menu time! Pick an option and let’s go!
    while (true) {
        std::cout << "\n--- Patient Management System ---\n";
        std::cout << "1. Add Patient\n";
        std::cout << "2. Edit Patient\n";
        std::cout << "3. Delete Patient\n";
        std::cout << "4. List All Patients\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();  // Clear out that pesky newline

        // Handle the user's choice
        if (choice == 1) {
            addPatient();  // Add a new patient
        } else if (choice == 2) {
            int id;
            std::cout << "Enter Patient ID to edit: ";
            std::cin >> id;
            std::cin.ignore();
            editPatient(id);  // Edit an existing patient
        } else if (choice == 3) {
            int id;
            std::cout << "Enter Patient ID to delete: ";
            std::cin >> id;
            std::cin.ignore();
            deletePatient(id);  // Delete a patient
        } else if (choice == 4) {
            listAllPatients();  // List all patients
        } else if (choice == 5) {
            break;  // Exit the program
        } else {
            std::cout << "Oops! Invalid choice! Please try again.\n";  // Handle invalid input
        }
    }

    return 0;
}

