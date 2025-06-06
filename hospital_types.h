#ifndef HOSPITAL_TYPES_H
#define HOSPITAL_TYPES_H

#include <string>

enum AppointmentStatus { PENDING, ACTIVE, RETIRED };
enum DoctorStatus { ACTIVE_DOCTOR, ON_LEAVE, RETIRED_DOCTOR };
enum StockStatus { IN_STOCK, OUT_OF_STOCK };
enum Gender { MALE, FEMALE };

struct Patient {
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string contact;
    std::string medicalHistory;
};

struct Doctor {
    int id;
    std::string name;
    std::string specialization;
    std::string contact;
    DoctorStatus status;
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
    StockStatus status;
};

#endif // HOSPITAL_TYPES_H