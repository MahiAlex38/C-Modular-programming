#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

void runBillingModule() {
    const double CONSULTATION_FEE = 200.0;
    const double ROOM_FEE = 100.0;

    auto displayMedicines = []() {
        cout << "\nAvailable Medicines:\n";
        cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << "Stock\n";
        cout << "--------------------------------------------------\n";
        for (const auto& m : medicines) {
            cout << left << setw(5) << m.id << setw(20) << m.name << setw(10) << m.price << m.stock << endl;
        }
    };

    auto searchPatientById = [](int id) -> Patient* {
        for (auto& p : patients) {
            if (p.id == id) return &p;
        }
        return nullptr;
    };

    auto searchMedicineByName = [](const string& name) -> Medicine* {
        for (auto& m : medicines) {
            string medName = m.name;
            string inputName = name;
            transform(medName.begin(), medName.end(), medName.begin(), ::tolower);
            transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);
            if (medName == inputName) return &m;
        }
        return nullptr;
    };

    auto calculateMedicineCharges = [&]() -> double {
        displayMedicines();
        int count = getValidInt("Enter number of medicine types: ");
        double total = 0.0;
        for (int i = 0; i < count; ++i) {
            cout << "Enter medicine name: ";
            string name;
            getline(cin, name);
            Medicine* med = searchMedicineByName(name);
            if (!med) {
                cout << "Medicine not found. Skipping...\n";
                continue;
            }
            cout << "Available stock for " << med->name << ": " << med->stock << endl;
            int quantity = getValidInt("Enter quantity: ");
            if (quantity > med->stock) {
                cout << "Insufficient stock. Skipping...\n";
                continue;
            }
            total += med->price * quantity;
            med->stock -= quantity;
            cout << "Added: " << med->name << " x " << quantity << " = " << med->price * quantity << " birr\n";
        }
        return total;
    };

    auto processBilling = [&]() {
        int id = getValidInt("Enter patient ID: ");
        Patient* patient = searchPatientById(id);
        if (!patient) {
            cout << "Patient not found.\n";
            return;
        }

        int visits = getValidInt("Enter doctor visits: ");
        int stay = getValidInt("Enter days stayed: ");
        double medCharge = calculateMedicineCharges();
        double consultCharge = visits * CONSULTATION_FEE;
        double roomCharge = stay * ROOM_FEE;
        double total = consultCharge + roomCharge + medCharge;

        cout << "\n--- Billing Summary ---\n";
        cout << "Consultation Fee: " << consultCharge << " birr\n";
        cout << "Room Charge: " << roomCharge << " birr\n";
        cout << "Medicine Charge: " << medCharge << " birr\n";
        cout << "Total: " << total << " birr\n";

        double payment;
        while (true) {
            cout << "Enter payment amount: ";
            cin >> payment;
            cin.ignore();
            if (payment >= total) {
                cout << "Payment accepted. Change: " << payment - total << " birr\n";
                break;
            } else {
                cout << "Insufficient payment. Please enter at least " << total << " birr.\n";
            }
        }
    };

    int choice;
    do {
        cout << "\n--- Billing & Payment Menu ---\n";
        cout << "1. Process Billing\n";
        cout << "2. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: processBilling(); break;
            case 2: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 2);
}