#include <map>
#include <iomanip>
#include <hospital_data.h>
#include <hospital_types.h>
#include <iostream>
#include <ctime>
#include <utils.h>

void runReportingModule() {
    // Assuming these are globally accessible or passed as parameters
    extern std::vector<Patient> patients;
    extern std::vector<Doctor> doctors;
    extern std::vector<Appointment> appointments;
    extern std::vector<SaleRecord> sales;
    int choice;

    while (true) {
        cout << "\n--- Reporting Management Menu ---\n";
        cout << "1. Generate daily Patient report\n";
        cout << "2. Generate doctor performance Module\n";
        cout << "3. Report critical cases\n";
        cout << "4. Sales Report\n";
        cout << "5. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: generateDailyPatientReport(patients); break;
            case 2: generateDoctorPerformanceReport(doctors, appointments); break;
            case 3: reportCriticalCases(patients); break;
            case 4: generateRevenueReport(sales); break;
            case 5: return;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}

string getCurrentDateString() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buf[11];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
    return string(buf);
}
void generateDailyPatientReport(const vector<Patient>& patients) {
    string today = getCurrentDateString();
    int admittedCount = 0;
    int dischargedCount = 0;

    for (const auto& patient : patients) {
        if (patient.admissionStatus == ADMITTED && patient.admissionDate == today) {
            admittedCount++;
        }
        if (patient.admissionStatus == DISCHARGED && patient.dischargeDate == today) {
            dischargedCount++;
        }
    }

    cout << "\n=== Daily Patient Report (" << today << ") ===\n";
    cout << "Patients Admitted Today: " << admittedCount << "\n";
    cout << "Patients Discharged Today: " << dischargedCount << "\n";
}

void generateDoctorPerformanceReport(const std::vector<Doctor>& doctors, const std::vector<Appointment>& appointments) {
    // Map DoctorID to number of patients treated
    std::map<int, int> doctorPatientCount;

    // Initialize counts for all doctors to 0
    for (const auto& doc : doctors) {
        doctorPatientCount[doc.id] = 0;
    }

    // Count appointments per doctor
    for (const auto& appt : appointments) {
        doctorPatientCount[appt.doctor.id]++;
    }

    // Display the report
    std::cout << "\n=== Doctor Performance Report ===\n";
    std::cout << std::left << std::setw(10) << "Doctor ID"
              << std::left << std::setw(25) << "Doctor Name"
              << "Patients Treated\n";
    std::cout << std::string(60, '=') << "\n";

    for (const auto& doc : doctors) {
        int count = doctorPatientCount[doc.id];
        std::cout << std::left << std::setw(10) << doc.id
                  << std::left << std::setw(25) << doc.name
                  << count << "\n";
    }
}
void reportCriticalCases(const std::vector<Patient>& patients) {
    int totalCritical = 0;
    int admittedCritical = 0; 
    int dischargedCritical = 0;

    for (const auto& patient : patients) {
        if (patient.severity == CRITICAL) {
            totalCritical++;

            if (patient.admissionStatus == ADMITTED) {
                admittedCritical++;
            } 
           
        }
    }

  
    cout << "\n=== Emergency/Critical Case Statistics ===\n";
    cout << "Total Critical Cases: " << totalCritical << "\n";
    cout << "Currently Admitted Critical Cases: " << admittedCritical << "\n";

    
}

void generateRevenueReport(const std::vector<SaleRecord>& sales, int year = 0, int month = 0) {
    double totalEarnings = 0.0;
    map<string, double> periodEarnings; 

    for (const auto& sale : sales) {
        // Parse date
        string date = sale.date; 
        string yearStr = date.substr(0,4);
        string monthStr = date.substr(5,2);
        string periodKey;

        if (month == 0) {
            
            periodKey = yearStr;
        } else if (year == std::stoi(yearStr) && month == std::stoi(monthStr)) {
            
            periodKey = yearStr + "-" + monthStr;
        } else if (year == 0 && month == 0) {
            
            periodKey = "All Time";
        } else {
            continue; // skip if not matching the filter
        }

        periodEarnings[periodKey] += sale.totalPrice;
        totalEarnings += sale.totalPrice;
    }

    
    cout << "\n=== Revenue Report ===\n";
    if (periodEarnings.empty()) {
        cout << "No sales data for the specified period.\n";
        return;
    }

    for (const auto& entry : periodEarnings) {
        cout << "Period: " << entry.first << " | Earnings: $" << entry.second << "\n";
    }

    cout << "Total Earnings: $" << totalEarnings << "\n";
}

