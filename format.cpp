#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <set>

#include "mng_app.h"

int countsdt = 0;

/* Formating Name */
std::string formatName(std::string& a) {
    if (a.empty()) return a;

    // Step 1: Remove leading and trailing spaces
    size_t start = 0;
    while (start < a.length() && a[start] == ' ') start++;
    size_t end = a.length();
    while (end > start && a[end - 1] == ' ') end--;
    a = a.substr(start, end - start);

    if (a.empty()) return a;

    // Step 2: Remove extra spaces between words
    std::string temp;
    bool inSpace = false;
    for (char c : a) {
        if (c == ' ') {
            if (!inSpace) {
                temp += ' ';
                inSpace = true;
            }
        } else {
            temp += c;
            inSpace = false;
        }
    }
    a = temp;

    // Step 3: Capitalize first letter and adjust case
    if (islower(a[0])) {
        a[0] = toupper(a[0]);
    }

    for (size_t i = 1; i < a.length(); i++) {
        if (a[i - 1] == ' ' && islower(a[i])) {
            // Capitalize first letter of each word
            a[i] = toupper(a[i]);
        } else if (a[i - 1] != ' ' && isupper(a[i])) {
            // Lowercase subsequent letters in each word
            a[i] = tolower(a[i]);
        }
    }

    // Step 4: Replace "Iphone" with "iPhone"
    size_t pos = a.find("Iphone");
    if (pos != std::string::npos) {
        a.replace(pos, 6, "iPhone");
    }

    return a;
}

/* [Hien] Not understand clearly yet*/
/* Formating Phone Number */
string formatPhoneNumber(string &a) {
    string phoneNumber;
    if (countsdt >= 1) {
        cin >> phoneNumber;
        if (!isValidPhoneNumber(phoneNumber)) {
            cout << "Invalid phone number. Please enter again: ";
            ++countsdt;
            return formatPhoneNumber(a);
        } else {
            a = phoneNumber;
            return a;
        }
    } else {
        if (!isValidPhoneNumber(a)) {
            cout << "Invalid phone number. Please enter again: ";
            ++countsdt;
            return formatPhoneNumber(a);
        }
        return a;
    }
    return a;
}

bool isValidPhoneNumber(const string &a) {
    // Valid prefixes of Vietnamese telecom operators
    set<string> validPrefixes = {
        "09", "08", "07", "05", "03", "02" // Main valid prefixes
    };
    // Check length and the first character
    if (a.length() != 10 || a[0] != '0') return false;
    // Check if the prefix is in the valid list
    if (validPrefixes.find(a.substr(0, 2)) == validPrefixes.end()) return false;

    return true;
}

/* Formating Date */
string formatDate(string &a) {
    time_t now = time(nullptr);
    tm *local_time = localtime(&now);

    // Use stringstream to format day, month, and year
    ostringstream ss;
    ss << setw(2) << setfill('0') << local_time->tm_mday << "/"
       << setw(2) << setfill('0') << (local_time->tm_mon + 1) << "/"
       << (local_time->tm_year + 1900);

    a = ss.str();
    return a;
}

/* Formating Age */
string formatAge(string &a) {
    int age;
    try {
        age = stoi(a); // Convert string to integer
    } catch (exception &e) {
        cout << "Invalid age. Please enter again: ";
        cin >> a;
        return formatAge(a); // Recursively call the function if the input is not a number
    }

    // Check if the age is valid
    if (age < 1 || age > 150) {
        cout << "Invalid age. Please enter again: ";
        cin >> a;
        return formatAge(a); // Recursively call the function if the age is not valid
    }

    a = to_string(age) + " years old";
    return a;
}

/* Formating Price */
string formatPrice(string a) {
    // Remove non-digit and non-dot characters
    a.erase(remove_if(a.begin(), a.end(), [](char c) {
        return !isdigit(c) && c != '.';
    }), a.end());

    // Remove trailing dots (if any)
    while (!a.empty() && a.back() == '.') {
        a.pop_back();
    }

    // Convert the cleaned string to an integer
    long long number = stoll(a);
    
    // Format the number without decimal places
    ostringstream oss;
    oss << number;
    string formattedPrice = oss.str();

    // Insert dots as thousand separators
    int pos = formattedPrice.length();
    for (int i = pos - 3; i > 0; i -= 3) {
        formattedPrice.insert(i, ".");
    }

    return formattedPrice + " VND";
}