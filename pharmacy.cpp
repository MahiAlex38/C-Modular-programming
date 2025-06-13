#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <fstream>

using namespace std;
void addMedicine();
void updateStock();
void viewInventory();
void generateUsageReport() {
    if (medicines.empty()) {
        cout << "No medicines in inventory.\n";
        return;
    }

    // Create a copy of medicines vector for sorting
    vector<Medicine> sortedMedicines = medicines;
    
    // Sort by times dispensed in descending order
    sort(sortedMedicines.begin(), sortedMedicines.end(),
         [](const Medicine& a, const Medicine& b) {
             return a.timesDispensed > b.timesDispensed;
         });

    cout << "\n=== Medicine Usage Report ===\n";
    cout << setfill('=') << setw(80) << "\n" << setfill(' ');
    cout << left << setw(5) << "ID"
         << setw(20) << "Name"
         << setw(15) << "Times Used"
         << setw(15) << "Initial Stock"
         << "Current Stock\n";
    cout << setfill('-') << setw(80) << "\n" << setfill(' ');

    for (const auto& med : sortedMedicines) {
        cout << left << setw(5) << med.id
             << setw(20) << med.name
             << setw(15) << med.timesDispensed
             << setw(15) << med.initialStock
             << med.stock << "\n";
    }

    cout << setfill('=') << setw(80) << "\n" << setfill(' ');

    // Calculate and display total statistics
    int totalDispensed = 0;
    for (const auto& med : medicines) {
        totalDispensed += med.timesDispensed;
    }

    cout << "\nTotal Medicines Dispensed: " << totalDispensed << "\n";
    if (!sortedMedicines.empty()) {
        cout << "Most Prescribed Medicine: " << sortedMedicines[0].name
             << " (" << sortedMedicines[0].timesDispensed << " times)\n";
    }
}
void editMedicine() {
    int id;
    cout << "Enter Medicine ID to edit: ";
    cin >> id;
    cin.ignore();

    for (auto& med : medicines) {
        if (med.id == id) {
            int choice;
            do {
                cout << "\n--- Edit Medicine ---\n";
                cout << "1. Update Name\n";
                cout << "2. Update Price\n";
                cout << "3. Update Stock\n";
                cout << "4. Return to Pharmacy Menu\n";
                cout << "Enter your choice: ";
                cin >> choice;
                cin.ignore();

                switch (choice) {
                    case 1: {
                        cout << "Enter new name: ";
                        getline(cin, med.name);
                        cout << "Medicine name updated.\n";
                        break;
                    }
                    case 2: {
                        string input;
                        cout << "Enter new price: ";
                        getline(cin, input);
                        try {
                            med.price = stod(input);
                            cout << "Price updated.\n";
                        } catch (...) {
                            cout << "Invalid input. Price not updated.\n";
                        }
                        break;
                    }
                    case 3: {
                        string input;
                        cout << "Enter new stock quantity: ";
                        getline(cin, input);
                        try {
                            int newStock = stoi(input);
                            med.stock = newStock;
                            med.initialStock = newStock; // optional, depends on logic
                            med.status = med.stock > 0 ? IN_STOCK : OUT_OF_STOCK;
                            cout << "Stock updated.\n";
                        } catch (...) {
                            cout << "Invalid input. Stock not updated.\n";
                        }
                        break;
                    }
                    case 4:
                        cout << "Returning to Pharmacy Menu.\n";
                        break;
                    default:
                        cout << "Invalid choice. Try again.\n";
                }
            } while (choice != 4);
            return;
        }
    }
    cout << "Medicine with ID " << id << " not found.\n";
}
void deleteMedicine() {
    int id;
    cout << "Enter Medicine ID to delete: ";
    cin >> id;
    cin.ignore();

    // Find medicine index
    auto it = std::remove_if(medicines.begin(), medicines.end(),
        [id](constMedicine&med) { return med.id == id; });

    if (it != medicines.end()) {
        medicines.erase(it, medicines.end());
        cout << "Medicine with ID " << id << " has been deleted from inventory.\n";
    } else {
        cout << "Medicine with ID " << id << " not found.\n";
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
    med.initialStock = med.stock;
    med.timesDispensed = 0;

    med.status = med.stock > 0 ? IN_STOCK : OUT_OF_STOCK;

    medicines.push_back(med);
    cout << "Medicine added successfully.\n";
}

void updateStock() {
    int id, quantity;
    cout << "Enter Medicine ID to update stock: ";
    cin >> id;
    cout << "Enter quantity (negative for dispensing): ";
    cin >> quantity;
    cin.ignore();

    for (auto& med : medicines) {
        if (med.id == id) {
            if (quantity < 0 && abs(quantity) > med.stock) {
                cout << "Error: Not enough stock available.\n";
                return;
            }
            med.stock += quantity;
            if (quantity < 0) {
                med.timesDispensed += abs(quantity);
            } else {
                med.initialStock += quantity;
            }
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

    cout << "\n=== Current Inventory ===\n";
    cout << setfill('-') << setw(80) << "\n" << setfill(' ');
    cout << left << setw(5) << "ID" 
         << setw(20) << "Name"
         << setw(10) << "Price"
         << setw(10) << "Stock"
         << setw(15) << "Times Used"
         << "Status\n";
    cout << setfill('-') << setw(80) << "\n" << setfill(' ');

    for (const auto& med : medicines) {
        cout << left << setw(5) << med.id
             << setw(20) << med.name
             << setw(10) << fixed << setprecision(2) << med.price
             << setw(10) << med.stock
             << setw(15) << med.timesDispensed
             << (med.status == IN_STOCK ? "In Stock" : "Out of Stock") << "\n";
    }
}
void searchMedicineById() {
    int id;
    cout << "Enter Medicine ID to search: ";
    cin >> id;
    cin.ignore();

    for (const auto& med : medicines) {
        if (med.id == id) {
            cout << "\nMedicine Details:\n";
            cout << "ID: " << med.id << "\n";
            cout << "Name: " << med.name << "\n";
            cout << "Price: " << fixed << setprecision(2) << med.price << "\n";
            cout << "Stock: " << med.stock << "\n";
            cout << "Times Dispensed: " << med.timesDispensed << "\n";
            cout << "Status: " << (med.status == IN_STOCK ? "In Stock" : "Out of Stock") << "\n";
            return;
        }
    }
    cout << "Medicine with ID " << id << " not found.\n";
}
void lowStockAlert() {
    const int threshold = 10; // Define your threshold here
    bool lowStockFound = false;

    cout << "\n=== Low Stock Alert ===\n";

    for (const auto& med : medicines) {
        if (med.stock < threshold) {
            if (!lowStockFound) {
                cout << "Medicines with stock below " << threshold << ":\n";
                lowStockFound = true;
            }
            cout << "- ID: " << med.id
                 << ", Name: " << med.name
                 << ", Current Stock: " << med.stock << "\n";
        }
    }

    if (!lowStockFound) {
        cout << "All medicines are sufficiently stocked.\n";
    }
}
void saveInventoryToFile() {
    ofstream outFile("medicines.txt");
    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return;
    }

    for (const auto& med : medicines) {
        outFile << med.id << "," // ID
                << med.name << ","
                << med.price << ","
                << med.stock << ","
                << med.initialStock << ","
                << med.timesDispensed << ","
                << (med.status == IN_STOCK ? "IN_STOCK" : "OUT_OF_STOCK") << "\n";
    }
    outFile.close();
    cout << "Inventory saved successfully.\n";
}
void loadInventoryFromFile() {
    ifstream inFile("medicines.txt");
    if (!inFile) {
        cout << "Error opening file for reading.\n";
        return;
    }

    medicines.clear(); // Clear existing data
    string line;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        // Parse each line
        istringstream iss(line);
        string token;
        Medicine med;

        getline(iss, token, ',');
        med.id = stoi(token);

        getline(iss, med.name, ',');

        getline(iss, token, ',');
        med.price = stod(token);

        getline(iss, token, ',');
        med.stock = stoi(token);

        getline(iss, token, ',');
        med.initialStock = stoi(token);

        getline(iss, token, ',');
        med.timesDispensed = stoi(token);

        getline(iss, token, ',');
        med.status = (token == "IN_STOCK") ? IN_STOCK : OUT_OF_STOCK;

        medicines.push_back(med);
    }

    inFile.close();
    cout << "Inventory loaded successfully.\n";
}

void runPharmacyModule() {
    int choice;

    while (true) {
        cout << "\n=== Pharmacy Inventory Menu ===\n";
        cout << "1. Add Medicine\n";
        cout << "2. Update Stock\n";
        cout << "3. View Inventory\n";
        cout << "4. Generate Usage Report\n";
        cout<<"5. Edit Medicine\n";
        cout<<"6. Delete Medicine\n";
        cout<<"7. Search Medicine\n";
        cout<<"8 Stock threshold\n";
        cout << "9. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addMedicine(); break;
            case 2: updateStock(); break;
            case 3: viewInventory(); break;
            case 4: generateUsageReport(); break;
            case 5: editMedicine(); break;
            case 6:deleteMedicine();break;
            case 7: searchMedicine(); break;
            case 8: lowStockAlert(); break;
            case 9: return;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}
