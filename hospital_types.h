#ifndef HOSPITAL_TYPES_H
#define HOSPITAL_TYPES_H

#include <string>
#include <iostream>
#include <vector>

enum AppointmentStatus { PENDING, ACTIVE, RETIRED };
enum DoctorStatus { ACTIVE_DOCTOR, ON_LEAVE, RETIRED_DOCTOR };
enum StockStatus { IN_STOCK, OUT_OF_STOCK };
enum Gender { MALE, FEMALE };
enum AdmissionStatus { ADMITTED, DISCHARGED };
enum WardType { GENERAL, PRIVATE, ICU, EMERGENCY };

struct Patient {
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string contact;
    std::string medicalHistory;
    AdmissionStatus admissionStatus;
    int assignedDoctorId;
    enum CaseSeverity { NORMAL, CRITICAL };
    CaseSeverity severity;
};

struct Doctor {
    int id;
    std::string name;
    std::string specialization;
    std::string contact;
    DoctorStatus status;
    string schedule; 
    bool available;
};

struct Bed {
    int bedNumber;
    bool isOccupied;
    Patient* occupiedBy;
};

struct Ward {
    int wardNumber;
    WardType type;
    std::vector<Bed> beds;
    int capacity;
};

struct Appointment {
    int appointmentID;
    Patient patient;
    Doctor doctor;
    std::string date;
    std::string time;
    std::string reason;
};

struct Medicine {
    int id;
    std::string name;
    double price;
    int stock;
    int initialStock;
    int timesDispensed;
    StockStatus status;
std::string expiryDate;
};

struct SaleRecord {
    int saleID;
    int medicineID;
    int quantity;
    std::string date; 
    double totalPrice;
};
struct Bill {
    int patientId;
    int visits;
    int stayDays;
    vector<pair<Medicine*, int>> medicines;  
    double totalAmount;
};


#endif// HOSPITAL_TYPES_H
