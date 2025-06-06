#include <iostream>
using namespace std;

// Declare the run functions for each module
void runAppointmentModule();
void runBillingModule();
void runDoctorModule();
void runPatientModule();
void runPharmacyModule();

int main() {
    int choice;
    do {
        cout << "\n===== Hospital Management System =====\n";
        cout << "1. Appointment Module\n";
        cout << "2. Billing Module\n";
        cout << "3. Doctor Management Module\n";
        cout << "4. Patient Management Module\n";
        cout << "5. Pharmacy Inventory Module\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // Clear input buffer

        switch (choice) {
            case 1:
                runAppointmentModule();
                break;
            case 2:
                runBillingModule();
                break;
            case 3:
                runDoctorModule();
                break;
            case 4:
                runPatientModule();
                break;
            case 5:
                runPharmacyModule();
                break;
            case 6:
                cout << "Exiting system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 6);

    return 0;
}