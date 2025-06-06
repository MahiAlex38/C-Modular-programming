#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;
void addMedicine();
void updateStock();
void viewInventory();
void runPharmacyModule() {
    int choice;

    while (true) {
        cout << "\n--- Pharmacy Inventory Menu ---\n";
        cout << "1. Add Medicine\n";
        cout << "2. Update Stock\n";
        cout << "3. View Inventory\n";
        cout << "4. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addMedicine(); break;
            case 2: updateStock(); break;
            case 3: viewInventory(); break;
            case 4: return;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}

void addMedicine() {
    Medicine med;
    string input;

    cout << "Enter Medicine ID: ";
    getline(cin, input);
    med.id = stoi(input);

    cout << "Enter Medicine Name: ";
    getline(cin, med.name);

    cout << "Enter Price: ";
    getline(cin, input);
    med.price = stod(input);

    cout << "Enter Stock Quantity: ";
    getline(cin, input);
    med.stock = stoi(input);

    med.status = med.stock > 0 ? IN_STOCK : OUT_OF_STOCK;

    medicines.push_back(med);
    cout << "Medicine added successfully.\n";
}

void updateStock() {
    int id, quantity;
    cout << "Enter Medicine ID to update stock: ";
    cin >> id;
    cout << "Enter quantity to add: ";
    cin >> quantity;
    cin.ignore();

    for (auto& med : medicines) {
        if (med.id == id) {
            med.stock += quantity;
            med.status = med.stock > 0 ? IN_STOCK : OUT_OF_STOCK;
            cout << "Stock updated. New stock: " << med.stock << "\n";
            return;
        }
    }

    cout << "Medicine not found.\n";
}

void viewInventory() {
    if (medicines.empty()) {
        cout << "No medicines in inventory.\n";
        return;
    }

    for (const auto& med : medicines) {
        cout << "\n--- Medicine ---\n";
        cout << "ID: " << med.id << "\n";
        cout << "Name: " << med.name << "\n";
        cout << "Price: " << med.price << "\n";
        cout << "Stock: " << med.stock << "\n";
        cout << "Status: " << (med.status == IN_STOCK ? "In Stock" : "Out of Stock") << "\n";
    }
}