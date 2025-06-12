#include <iostream>
#include <iomanip>
#include <algorithm>
#include "hospital_types.h"
#include "hospital_data.h"
#include "utils.h"

void initializeWards() {
    // Initialize different types of wards with their capacities
    Ward generalWard{1, GENERAL, {}, 20};
    Ward privateWard{2, PRIVATE, {}, 10};
    Ward icuWard{3, ICU, {}, 15};
    Ward emergencyWard{4, EMERGENCY, {}, 8};

    // Initialize beds for each ward
    auto initializeBeds = [](Ward& ward) {
        for (int i = 1; i <= ward.capacity; i++) {
            ward.beds.push_back({i, false, nullptr});
        }
    };

    initializeBeds(generalWard);
    initializeBeds(privateWard);
    initializeBeds(icuWard);
    initializeBeds(emergencyWard);

    // Add wards to the global wards vector
    wards = {generalWard, privateWard, icuWard, emergencyWard};
}

std::string getWardTypeName(WardType type) {
    switch (type) {
        case GENERAL: return "General";
        case PRIVATE: return "Private";
        case ICU: return "ICU";
        case EMERGENCY: return "Emergency";
        default: return "Unknown";
    }
}

void displayWardStatus() {
    std::cout << "\n=== Hospital Ward Status ===\n";
    std::cout << std::setfill('=') << std::setw(50) << "\n";
    std::cout << std::setfill(' ');

    for (const auto& ward : wards) {
        int occupiedBeds = std::count_if(ward.beds.begin(), ward.beds.end(),
            [](const Bed& bed) { return bed.isOccupied; });

        std::cout << "\nWard " << ward.wardNumber << " - " << getWardTypeName(ward.type);
        std::cout << "\n----------------------------";
        std::cout << "\nTotal Capacity: " << ward.capacity;
        std::cout << "\nOccupied Beds: " << occupiedBeds;
        std::cout << "\nAvailable Beds: " << (ward.capacity - occupiedBeds);
        
        if (occupiedBeds > 0) {
            std::cout << "\n\nOccupied Bed Details:";
            for (const auto& bed : ward.beds) {
                if (bed.isOccupied && bed.occupiedBy != nullptr) {
                    std::cout << "\nBed " << bed.bedNumber << ": "
                              << bed.occupiedBy->name << " (ID: " << bed.occupiedBy->id << ")";
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << std::setfill('=') << std::setw(50) << "\n";
}

bool isPatientAdmitted(int patientId) {
    for (const auto& ward : wards) {
        for (const auto& bed : ward.beds) {
            if (bed.isOccupied && bed.occupiedBy != nullptr && bed.occupiedBy->id == patientId) {
                return true;
            }
        }
    }
    return false;
}

void assignBed() {
    int patientId;
    std::cout << "\nEnter Patient ID: ";
    std::cin >> patientId;

    // Find patient
    auto patientIt = std::find_if(patients.begin(), patients.end(),
        [patientId](const Patient& p) { return p.id == patientId; });

    if (patientIt == patients.end()) {
        std::cout << "\nError: Patient not found!\n";
        return;
    }

    if (isPatientAdmitted(patientId)) {
        std::cout << "\nError: Patient is already admitted to a bed!\n";
        return;
    }

    // Display available wards with free beds
    std::cout << "\nAvailable Wards:\n";
    std::cout << std::setfill('-') << std::setw(40) << "\n";
    std::cout << std::setfill(' ');

    bool hasAvailableBeds = false;
    for (const auto& ward : wards) {
        int availableBeds = std::count_if(ward.beds.begin(), ward.beds.end(),
            [](const Bed& bed) { return !bed.isOccupied; });

        if (availableBeds > 0) {
            std::cout << ward.wardNumber << ". " << getWardTypeName(ward.type)
                      << " Ward (Available Beds: " << availableBeds << ")\n";
            hasAvailableBeds = true;
        }
    }

    if (!hasAvailableBeds) {
        std::cout << "\nError: No beds available in any ward!\n";
        return;
    }

    int wardChoice;
    std::cout << "\nSelect Ward Number: ";
    std::cin >> wardChoice;

    // Find selected ward
    auto wardIt = std::find_if(wards.begin(), wards.end(),
        [wardChoice](const Ward& w) { return w.wardNumber == wardChoice; });

    if (wardIt == wards.end()) {
        std::cout << "\nError: Invalid ward selection!\n";
        return;
    }

    // Display available beds in selected ward
    std::cout << "\nAvailable Beds in " << getWardTypeName(wardIt->type) << " Ward:\n";
    for (const auto& bed : wardIt->beds) {
        if (!bed.isOccupied) {
            std::cout << bed.bedNumber << " ";
        }
    }

    int bedChoice;
    std::cout << "\n\nSelect Bed Number: ";
    std::cin >> bedChoice;

    // Find and assign the selected bed
    auto bedIt = std::find_if(wardIt->beds.begin(), wardIt->beds.end(),
        [bedChoice](const Bed& b) { return b.bedNumber == bedChoice && !b.isOccupied; });

    if (bedIt == wardIt->beds.end()) {
        std::cout << "\nError: Invalid bed selection or bed is occupied!\n";
        return;
    }

    // Assign bed to patient
    bedIt->isOccupied = true;
    bedIt->occupiedBy = &(*patientIt);
    patientIt->admissionStatus = ADMITTED;

    std::cout << "\nSuccess: Patient " << patientIt->name << " assigned to "
              << getWardTypeName(wardIt->type) << " Ward, Bed " << bedIt->bedNumber << "\n";
}

void dischargeBed() {
    int patientId;
    std::cout << "\nEnter Patient ID: ";
    std::cin >> patientId;

    // Find patient
    auto patientIt = std::find_if(patients.begin(), patients.end(),
        [patientId](const Patient& p) { return p.id == patientId; });

    if (patientIt == patients.end()) {
        std::cout << "\nError: Patient not found!\n";
        return;
    }

    if (patientIt->admissionStatus != ADMITTED) {
        std::cout << "\nError: Patient is not currently admitted!\n";
        return;
    }

    // Find and free the bed
    bool bedFound = false;
    for (auto& ward : wards) {
        for (auto& bed : ward.beds) {
            if (bed.isOccupied && bed.occupiedBy == &(*patientIt)) {
                bed.isOccupied = false;
                bed.occupiedBy = nullptr;
                patientIt->admissionStatus = DISCHARGED;
                
                std::cout << "\nSuccess: Patient " << patientIt->name << " discharged from "
                          << getWardTypeName(ward.type) << " Ward, Bed " << bed.bedNumber << "\n";
                bedFound = true;
                break;
            }
        }
        if (bedFound) break;
    }

    if (!bedFound) {
        std::cout << "\nError: Patient's bed assignment not found!\n";
    }
}

void searchPatientBed() {
    int patientId;
    std::cout << "\nEnter Patient ID: ";
    std::cin >> patientId;

    bool found = false;
    for (const auto& ward : wards) {
        for (const auto& bed : ward.beds) {
            if (bed.isOccupied && bed.occupiedBy != nullptr && bed.occupiedBy->id == patientId) {
                std::cout << "\nPatient Location:";
                std::cout << "\n-----------------";
                std::cout << "\nWard: " << getWardTypeName(ward.type);
                std::cout << "\nBed Number: " << bed.bedNumber;
                std::cout << "\nPatient Name: " << bed.occupiedBy->name;
                std::cout << "\nPatient ID: " << bed.occupiedBy->id << "\n";
                found = true;
                break;
            }
        }
        if (found) break;
    }

    if (!found) {
        std::cout << "\nPatient not found in any ward!\n";
    }
}

void runWardModule() {
    if (wards.empty()) {
        initializeWards();
    }

    while (true) {
        std::cout << "\n=== Ward Management Module ===\n";
        std::cout << "1. Display Ward Status\n";
        std::cout << "2. Assign Patient to Bed\n";
        std::cout << "3. Discharge Patient\n";
        std::cout << "4. Search Patient Bed\n";
        std::cout << "5. Return to Main Menu\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                displayWardStatus();
                break;
            case 2:
                assignBed();
                break;
            case 3:
                dischargeBed();
                break;
            case 4:
                searchPatientBed();
                break;
            case 5:
                return;
            default:
                std::cout << "\nInvalid choice! Please try again.\n";
        }
    }
}
