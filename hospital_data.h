#ifndef HOSPITAL_DATA_H
#define HOSPITAL_DATA_H

#include <vector>
#include "hospital_types.h"

extern std::vector<Patient> patients;
extern std::vector<Doctor> doctors;
extern std::vector<Appointment> appointments;
extern std::vector<Medicine> medicines;
extern std::vector<Ward> wards;
extern std::vector<SaleRecord> sales;
extern std::vector<Bill> bills;

#endif // HOSPITAL_DATA_H
