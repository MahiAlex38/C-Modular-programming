#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <iomanip>
using namespace std;

enum Gender { MALE, FEMALE };

struct Patient {
    int id;
    string name;
    Gender gender;
    string contact;
    string medicalHistory;
};

struct Medicine {
    int id;
    string name;
    double price;
    int stock;
};

const double CONSULTATION_FEE = 200.0;
const double ROOM_FEE = 100.0;

vector<Patient> patients;
vector<Medicine> medicines;

bool isNumeric(const string& str) {
    return regex_match(str, regex("^\d+$"));
}

Patient* searchPatient(int id) {
    for (auto& p : patients) {
        if (p.id == id) return &p;
    }
    return nullptr;
}

Medicine* searchMedicine(const string& name) {
    for (auto& m : medicines) {
        string medName = m.name;
        string inputName = name;
        transform(medName.begin(), medName.end(), medName.begin(), ::tolower);
        transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);
        if (medName == inputName) return &m;
    }
    return nullptr;
}

int getValidIntInput(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!isNumeric(input)) {
            cout << "Invalid input. Only positive whole numbers are allowed.
";
            continue;
        }
        return stoi(input);
    }
}

void displayAvailableMedicines() {
    cout << "
Available Medicines:
";
    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << "Stock
";
    cout << "---------------------------------------------------
";
    for (const auto& m : medicines) {
        cout << left << setw(5) << m.id << setw(20) << m.name << setw(10) << m.price << m.stock << endl;
    }
    cout << "---------------------------------------------------
";
}

double calculateMedicineCharges() {
    displayAvailableMedicines();
    int medCount = getValidIntInput("Enter how many types of medicines were prescribed: ");
    double total = 0.0;
    for (int i = 0; i < medCount; ++i) {
        cout << "Enter name of medicine #" << (i + 1) << ": ";
        string name;
        getline(cin, name);
        Medicine* med = searchMedicine(name);
        if (!med) {
            cout << "Medicine not found. Skipping...
";
            continue;
        }
        cout << "Available stock for " << med->name << ": " << med->stock << endl;
        int quantity = getValidIntInput("Enter quantity prescribed: ");
        if (quantity > med->stock) {
            cout << "Not enough stock for " << med->name << ". Skipping...
";
            continue;
        }
        total += med->price * quantity;
        med->stock -= quantity;
        cout << "Added " << med->name << " x " << quantity << " = " << med->price * quantity << " birr
";
    }
    cout << "Total medicine charges: " << total << " birr
";
    return total;
}

void processBilling() {
    int patientId = getValidIntInput("Enter Patient ID: ");
    Patient* patient = searchPatient(patientId);
    if (!patient) {
        cout << "Patient not found.
";
        return;
    }
    int doctorVisits = getValidIntInput("Enter number of doctor visits: ");
    int daysStayed = getValidIntInput("Enter number of days stayed: ");
    double totalConsultation = CONSULTATION_FEE * doctorVisits;
    double totalRoom = ROOM_FEE * daysStayed;
    double totalMedicine = calculateMedicineCharges();
    double total = totalConsultation + totalRoom + totalMedicine;
    cout << "
=== Billing Summary ===
";
    cout << "Consultation (" << doctorVisits << " x " << CONSULTATION_FEE << "): " << totalConsultation << " birr
";
    cout << "Room Charges (" << daysStayed << " x " << ROOM_FEE << "): " << totalRoom << " birr
";
    cout << "Medicine Charges: " << totalMedicine << " birr
";
    cout << "-----------------------------
";
    cout << "Total Bill: " << total << " birr
";
    double payment = 0;
    while (true) {
        cout << "Enter payment amount: ";
        cin >> payment;
        cin.ignore();
        if (payment >= total) {
            cout << "Payment accepted. Change: " << payment - total << " birr
";
            break;
        } else {
            cout << "Insufficient payment. Please enter at least " << total << " birr.
";
        }
    }
}

int main() {
    patients.push_back({1, "abebe", MALE, "1234567890", "None"});
    patients.push_back({2, "abebech", FEMALE, "0987654321", "Diabetic"});
    medicines.push_back({1, "Paracetamol", 20.0, 100});
    medicines.push_back({2, "Ibuprofen", 30.0, 50});
    medicines.push_back({3, "Amoxicillin", 50.0, 30});
    int choice;
    do {
        cout << "
=== Billing & Payment Menu ===
";
        cout << "1. Process Billing
";
        cout << "2. Exit
";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: processBilling(); break;
            case 2: cout << "Goodbye!
"; break;
            default: cout << "Invalid choice.
";
        }
    } while (choice != 2);
    return 0;
}
