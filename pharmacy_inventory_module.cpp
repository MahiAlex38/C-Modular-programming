#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

enum StockStatus { IN_STOCK, OUT_OF_STOCK };

struct Medicine {
    int id;
    string name;
    double price;
    int stock;
    StockStatus status;
};

vector<Medicine> medicines;

bool isNumeric(const string& str) {
    return regex_match(str, regex("^\d+$"));
}

bool isAlpha(const string& str) {
    return regex_match(str, regex("^[A-Za-z ]+$"));
}

bool isValidPrice(double price) {
    return price > 0;
}

void addMedicine() {
    Medicine newMedicine;
    string input;
    cout << "Enter Medicine ID: ";
    while (true) {
        getline(cin, input);
        if (isNumeric(input)) {
            newMedicine.id = stoi(input);
            break;
        } else {
            cout << "Invalid input. ID must be a number. Try again: ";
        }
    }
    cout << "Enter Medicine Name: ";
    while (true) {
        getline(cin, newMedicine.name);
        if (isAlpha(newMedicine.name)) {
            break;
        } else {
            cout << "Invalid name. Name should contain only letters. Try again: ";
        }
    }
    cout << "Enter Medicine Price: ";
    while (true) {
        getline(cin, input);
        try {
            newMedicine.price = stod(input);
            if (isValidPrice(newMedicine.price)) {
                break;
            } else {
                cout << "Price must be a positive number. Try again: ";
            }
        } catch (...) {
            cout << "Invalid input. Price must be a number. Try again: ";
        }
    }
    cout << "Enter Medicine Stock Quantity: ";
    while (true) {
        getline(cin, input);
        if (isNumeric(input)) {
            newMedicine.stock = stoi(input);
            break;
        } else {
            cout << "Invalid input. Stock must be a number. Try again: ";
        }
    }
    newMedicine.status = (newMedicine.stock > 0) ? IN_STOCK : OUT_OF_STOCK;
    medicines.push_back(newMedicine);
    cout << "Medicine added successfully!
";
}

Medicine* searchMedicine(int id) {
    for (auto& med : medicines) {
        if (med.id == id) {
            return &med;
        }
    }
    return nullptr;
}

Medicine* searchMedicine(const string& name) {
    for (auto& med : medicines) {
        if (med.name == name) {
            return &med;
        }
    }
    return nullptr;
}

void processPurchase(int medicineId, int quantity) {
    Medicine* med = searchMedicine(medicineId);
    if (med != nullptr) {
        if (med->status == OUT_OF_STOCK) {
            cout << "This medicine is out of stock!
";
        } else if (med->stock >= quantity) {
            med->stock -= quantity;
            if (med->stock == 0) {
                med->status = OUT_OF_STOCK;
            }
            cout << "Stock updated. New stock for " << med->name << ": " << med->stock << "
";
        } else {
            cout << "Not enough stock available. Only " << med->stock << " units left.
";
        }
    } else {
        cout << "Medicine with ID " << medicineId << " not found.
";
    }
}

void processBatchPurchase() {
    int numItems;
    cout << "Enter the number of medicines to purchase: ";
    cin >> numItems;
    for (int i = 0; i < numItems; ++i) {
        int medicineId, quantity;
        cout << "
Enter Medicine ID: ";
        cin >> medicineId;
        cout << "Enter quantity to purchase: ";
        cin >> quantity;
        processPurchase(medicineId, quantity);
    }
}

void displayInventory() {
    cout << "
=== Medicine Inventory ===
";
    for (auto& med : medicines) {
        cout << "ID: " << med.id << ", Name: " << med.name
             << ", Price: " << med.price << ", Stock: " << med.stock
             << ", Status: " << (med.status == IN_STOCK ? "In Stock" : "Out of Stock") << endl;
    }
}

int main() {
    medicines.push_back({1, "Ibuprofen", 20.0, 100, IN_STOCK});
    medicines.push_back({2, "Paracetamol", 15.0, 50, IN_STOCK});
    medicines.push_back({3, "Aspirin", 10.0, 30, IN_STOCK});
    int choice;
    do {
        cout << "
=== Pharmacy & Inventory Menu ===
";
        cout << "1. Add Medicine
";
        cout << "2. Deduct Stock (simulate purchase)
";
        cout << "3. Process Batch Purchase
";
        cout << "4. Display Inventory
";
        cout << "5. Exit
";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: addMedicine(); break;
            case 2:
                int medicineId, quantity;
                cout << "Enter Medicine ID: ";
                cin >> medicineId;
                cout << "Enter quantity to purchase: ";
                cin >> quantity;
                processPurchase(medicineId, quantity);
                break;
            case 3: processBatchPurchase(); break;
            case 4: displayInventory(); break;
            case 5: cout << "Goodbye!
"; break;
            default: cout << "Invalid choice. Try again.
";
        }
    } while (choice != 5);
    return 0;
}
