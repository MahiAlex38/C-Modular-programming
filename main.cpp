#include <iostream>
using namespace std;

// Declarations for module functions (defined in their respective .cpp files)
void runAppointmentModule();
void runBillingModule();
void runDoctorModule();
void runPatientModule();
void runPharmacyModule();
void runWardModule();
void runReportingModule();

int main() {
    int choice;
    do {
        cout << "\n===== Hospital Management System =====\n";
        cout << "1. Appointment Module\n";
        cout << "2. Billing & Payment Module\n";
        cout << "3. Doctor Management Module\n";
        cout << "4. Patient Management Module\n";
        cout << "5. Pharmacy Inventory Module\n";
        cout << "6. Ward Management Module\n";
        cout<< "7. Report Management Module\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline from input buffer

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
                runWardModule();
                break;
            case 7:
                runReportingModule();
                break;
            case 8:
                cout << "Exiting system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 7);

    return 0;
}
