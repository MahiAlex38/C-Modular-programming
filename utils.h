#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <regex>
#include <iostream>
#include <limits>

// Checks if the string contains only digits
inline bool isNumeric(const std::string& str) {
    return std::regex_match(str, std::regex("^[0-9]+$"));
}

// Checks if the string contains only alphabetic characters and spaces
inline bool isAlpha(const std::string& str) {
    return std::regex_match(str, std::regex("^[A-Za-z ]+$"));
}

// Checks if a contact number is valid (exactly 10 digits)
inline bool isValidContact(const std::string& str) {
    std::string cleaned;
    for (char c : str) {
        if (std::isdigit(c)) cleaned += c;
    }
    return cleaned.length() == 10;
}

// Checks if the price is a positive number
inline bool isValidPrice(double price) {
    return price > 0;
}

// Prompts the user until a valid positive integer is entered
inline int getValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 0) {
            std::cin.ignore();
            return value;
        } else {
            std::cout << "Invalid input. Please enter a valid positive number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

#endif // UTILS_H