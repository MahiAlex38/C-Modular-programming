#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"
#include <pharmacy>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
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

    // Function to find a bill by patient ID
Bill* findBillByPatientId(int patientId) {
    for (auto& bill : bills) {
        if (bill.patientId == patientId) {
            return &bill;
        }
    }
    return nullptr;
}




// Function to edit a bill
void editBill() {
    int patientId = getValidInt("Enter patient ID to edit their bill: ");
    Bill* bill = findBillByPatientId(patientId);
    if (!bill) {
        cout << "Bill not found for this patient.\n";
        return;
    }

    // Display current bill details
    cout << "\nCurrent Bill Details:\n";
    cout << "Number of visits: " << bill->visits << "\n";
    cout << "Number of days stayed: " << bill->stayDays << "\n";
    cout << "Medicines:\n";
    for (const auto& medPair : bill->medicines) {
        cout << "- " << medPair.first->name << " x " << medPair.second << "\n";
    }
    cout << "Total amount: " << bill->totalAmount << " birr\n";

    int editchoice;
    do {
        cout << "\n--- Edit Options ---\n";
        cout << "1. Modify number of doctor visits\n";
        cout << "2. Modify days stayed\n";
        cout << "3. Modify medicines\n";
        cout << "4. Finish editing\n";
        choice = getValidInt("Enter your choice: ");

        switch (choice) {
            case 1:
                bill->visits = getValidInt("Enter new number of visits: ");
                break;
            case 2:
                bill->stayDays = getValidInt("Enter new number of days: ");
                break;
            case 3:
                // Clear current medicines and re-add
                bill->medicines.clear();
                {
                    // Re-add medicines
                    int medCount = getValidInt("Enter number of medicines to add: ");
                    for (int i = 0; i < medCount; ++i) {
                        cout << "Enter medicine name: ";
                        string medName;
                        getline(cin, medName);
                        Medicine* med = searchMedicineByName(medName);
                        if (!med) {
                            cout << "Medicine not found. Skipping...\n";
                            continue;
                        }
                        int qty = getValidInt("Enter quantity: ");
                        if (qty > med->stock) {
                            cout << "Insufficient stock. Skipping...\n";
                            continue;
                        }
                        bill->medicines.emplace_back(med, qty);
                    }
                }
                // Recalculate total
                double newTotal = 0;
                for (const auto& medPair : bill->medicines) {
                    newTotal += medPair.first->price * medPair.second;
                }
                bill->totalAmount = newTotal + (bill->visits * CONSULTATION_FEE) + (bill->stayDays * ROOM_FEE);
                cout << "Medicines updated. New total: " << bill->totalAmount << " birr\n";
                break;
            case 4:
                cout << "Finished editing.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);
}
// Function to delete a bill
void deleteBill() {
    int patientId = getValidInt("Enter patient ID to delete their bill: ");
    int index = findBillIndexByPatientId(patientId);
    if (index == -1) {
        cout << "Bill not found for this patient.\n";
        return;
    }
    // Remove the bill from the vector
    bills.erase(bills.begin() + index);
    cout << "Bill for patient ID " << patientId << " has been deleted.\n";
}
    void displayBill(const Bill* bill) {
    if (!bill) {
        cout << "Bill not found.\n";
        return;
    }
    cout << "\n--- Bill Details ---\n";
    cout << "Patient ID: " << bill->patientId << "\n";
    cout << "Number of Visits: " << bill->visits << "\n";
    cout << "Days Stayed: " << bill->stayDays << "\n";
    cout << "Medicines:\n";
    for (const auto& medPair : bill->medicines) {
        cout << "- " << medPair.first->name << " x " << medPair.second << "\n";
    }
    cout << "Total Amount: " << bill->totalAmount << " birr\n";
    cout << "Payment Status: " << (bill->paymentStatus == PAID ? "PAID" : "UNPAID") << "\n";
    if (bill->paymentStatus == PAID) {
        cout << "Payment Date: " << bill->paymentDate;
    }
}

// Usage example
void searchBill() {
    int patientId = getValidInt("Enter patient ID to search for their bill: ");
    Bill* bill = searchBillByPatientId(patientId);
    displayBill(bill);
}
void billingRecords() {
    ofstream outFile("bills.txt");
    if (!outFile.is_open()) {
        cout << "Failed to open file for writing.\n";
        return;
    }

    for (const auto& bill : bills) {
        outFile << "PatientID: " << bill.patientId << "\n";
        outFile << "Visits: " << bill.visits << "\n";
        outFile << "DaysStayed: " << bill.stayDays << "\n";
        outFile << "Medicines:\n";
        for (const auto& medPair : bill.medicines) {
            outFile << "  - " << medPair.first->name << " x " << medPair.second << "\n";
        }
        outFile << "TotalAmount: " << bill.totalAmount << "\n";
        outFile << "PaymentStatus: " << (bill.paymentStatus == PAID ? "PAID" : "UNPAID") << "\n";
        if (bill.paymentStatus == PAID) {
            outFile << "PaymentDate: " << bill.paymentDate;
        }
        outFile << "--------------------------\n"; // Separator for each bill
    }

    outFile.close();
    cout << "Billing records successfully saved to bills.txt\n";
}
    void loadBillingRecordsFromFile() {
    ifstream inFile("bills.txt");
    if (!inFile) {
        cerr << "Error opening bills.txt for reading.\n";
        return;
    }

    bills.clear(); // Clear existing records

    string line;
    Bill currentBill;
    currentBill.medicines.clear();
    currentBill.paymentStatus = UNPAID;

    while (getline(inFile, line)) {
        if (line.find("PatientID:") == 0) {
            currentBill.patientId = stoi(line.substr(line.find(':') + 1));
        } else if (line.find("Doctor Visits:") == 0) {
            currentBill.visits = stoi(line.substr(line.find(':') + 1));
        } else if (line.find("Days Stayed:") == 0) {
            currentBill.stayDays = stoi(line.substr(line.find(':') + 1));
        } else if (line.find("Medicines:") == 0) {
            // Next lines contain medicines until separator
            currentBill.medicines.clear();
            while (getline(inFile, line) && line.find("  -") == 0) {
                // Parse medicine line: "  - medName x qty"
                size_t pos1 = line.find("-") + 1;
                size_t pos2 = line.find("x");
                string medName = line.substr(pos1, pos2 - pos1);
                medName.erase(medName.find_last_not_of(" \n\r\t")+1); // trim trailing spaces
                int qty = stoi(line.substr(pos2 + 1));
                
                Medicine* medPtr = searchMedicineByName(medName);
                if (medPtr) {
                    currentBill.medicines.emplace_back(medPtr, qty);
                }
            }
        } else if (line.find("Total Amount:") == 0) {
            currentBill.totalAmount = stod(line.substr(line.find(':') + 1));
        } else if (line.find("PaymentStatus:") == 0) {
            string status = line.substr(line.find(':') + 1);
            status.erase(0, status.find_first_not_of(" \t\n\r"));
            status.erase(status.find_last_not_of(" \t\n\r") + 1);
            currentBill.paymentStatus = (status == "PAID" ? PAID : UNPAID);
        } else if (line.find("PaymentDate:") == 0) {
            currentBill.paymentDate = line.substr(line.find(':') + 1);
        } else if (line.find("-----------------------------") == 0) {
            // End of one bill record
            bills.push_back(currentBill);
            // Prepare for next record
            currentBill.medicines.clear();
            currentBill.paymentStatus = UNPAID;
            currentBill.paymentDate = "";
        }
    }

    inFile.close();
    cout << "Billing records loaded successfully from bills.txt\n";
}
   // Function to calculate tax
double calculateTax(double amount) {
    const double TAX_RATE = 0.05; // 5%
    return amount * TAX_RATE;
}

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

    // Calculate subtotal
    double subtotal = consultCharge + roomCharge + medCharge;

    // Calculate tax
    double taxAmount = calculateTax(subtotal);

    // Total before discount
    double totalBeforeDiscount = subtotal + taxAmount;

    // Apply discount if total exceeds 1000 birr
    double discount = 0.0;
    if (totalBeforeDiscount > 1000) {
        discount = totalBeforeDiscount * 0.10; // 10% discount
    }

    double finalTotal = totalBeforeDiscount - discount;

    // Display summary
    cout << fixed << setprecision(2);
    cout << "\n--- Billing Summary ---\n";
    cout << "Consultation Fee (" << visits << " visits): " << consultCharge << " birr\n";
    cout << "Room Charge (" << stay << " days): " << roomCharge << " birr\n";
    cout << "Medicine Charges: " << medCharge << " birr\n";
    cout << "Subtotal: " << subtotal << " birr\n";
    cout << "Healthcare Tax (5%): " << taxAmount << " birr\n";

    if (discount > 0) {
        cout << "Discount Applied (10%): -" << discount << " birr\n";
    }
    cout << "Total payable: " << finalTotal << " birr\n";

    // Process payment
    double payment;
    while (true) {
        cout << "Enter payment amount: ";
        cin >> payment;
        cin.ignore();
        if (payment >= finalTotal) {
            cout << "Payment accepted. Change: " << payment - finalTotal << " birr\n";
            
            
            Bill newBill;
            newBill.patientId = id;
            newBill.visits = visits;
            newBill.stayDays = stay;
            newBill.totalAmount = finalTotal;
            newBill.paymentStatus = PAID;
            
            time_t now = time(0);
            char* dt = ctime(&now);
            newBill.paymentDate = string(dt);
            
            bills.push_back(newBill);
            
            cout << "Bill marked as PAID on " << newBill.paymentDate;
            break;
        } else {
            cout << "Insufficient payment. Please enter at least " << finalTotal << " birr.\n";
        }
    }
};
    void updatePaymentStatus() {
        int patientId = getValidInt("Enter patient ID to update payment status: ");
        Bill* bill = findBillByPatientId(patientId);
        if (!bill) {
            cout << "Bill not found for this patient.\n";
            return;
        }

        displayBill(bill);
        cout << "\nUpdate Payment Status:\n";
        cout << "1. Mark as PAID\n";
        cout << "2. Mark as UNPAID\n";
        int statusChoice = getValidInt("Enter choice: ");

        if (statusChoice == 1) {
            bill->paymentStatus = PAID;
            // Get current date for payment
            time_t now = time(0);
            char* dt = ctime(&now);
            bill->paymentDate = string(dt);
            cout << "Bill marked as PAID on " << bill->paymentDate;
        } else if (statusChoice == 2) {
            bill->paymentStatus = UNPAID;
            bill->paymentDate = "";
            cout << "Bill marked as UNPAID\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }

    int choice;
    do {
        cout << "\n--- Billing & Payment Menu ---\n";
        cout << "1. Process Billing\n";
        cout << "2. Edit billing system\n";
        cout << "3. Delete Bill\n";
        cout << "4. Search Bill\n";
        cout << "5. Update Payment Status\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: processBilling(); break;
            case 2: editchoice(); break;
            case 3: dleteBill(); break;
            case 4: searchBill(); break;
            case 5: updatePaymentStatus(); break;
            case 6: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 6);
}
