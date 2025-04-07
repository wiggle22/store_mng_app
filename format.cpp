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
#include <unordered_set>
#include <regex>

#include "mng_app.h"

int countsdt = 0;

// Check if the iPhone model name is valid
bool isValidIphoneModel(const string& model) {
    regex pattern(R"(iPhone (\d{2})( (Pro|Pro Max|Plus))?$)");
    return regex_match(model, pattern);
}

// Retrieve the list of existing iPhones from the file
unordered_set<string> getExistingIphonesWithStorage(const string& filename) {
    unordered_set<string> existingIphones;
    ifstream inFile(filename);
    string line;

    while (getline(inFile, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos) {
            existingIphones.insert(line.substr(0, line.find(',', pos + 1))); // Extract "Name,Storage"
        }
    }

    inFile.close();
    return existingIphones;
}

/* Function to check if string is all number */
bool isValidNumber(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}


/* Function to remove specical characters */
string removeSpecialChars(string a) {
    if (a.length() >= 2 && a[0] == '[') {  // 
        if (a[1] == 'A' || a[1] == 'B' || a[1] == 'C' || a[1] == 'D') {
            return a.substr(2);  // Remove "[ X"
        }
    }
    if (a[0] == '\n') return a.substr(1);
    return a;
}

/* Formating Name */
string formatName(string& a) {
    if (a.empty()) {
        cout << "[WARNING] This field cannot be empty. Please enter again: ";
        getline(cin, a);
        return formatName(a);
    }

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

/* Formating Phone Number */
string formatPhoneNumber(string &a) {
    // Check if the whole string is numeric or contains space or is empty or is valid prefixes of Vietnamese telecom operators
    if (a.find(' ') != string::npos || !isValidNumber(a) || a.empty() || !isValidPhoneNumber(a)) {
        cout << "[WARNING] Invalid phone number. Please enter again: ";
        getline(cin, a);
        return formatPhoneNumber(a);
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
    // Check if the whole string is numeric or contains space or is empty
    if (a.find(' ') != string::npos || !isValidNumber(a) || a.empty()) {
        cout << "[WARNING] Invalid age. Please enter again: ";
        getline(cin, a);
        return formatAge(a);
    }

    int age;
    age = stoi(a); // Convert string to integer
    
    // Check if the age is valid
    if (age < 1 || age > 150) {
        cout << "[WARNING] Invalid age. Please enter again: ";
        getline(cin, a);
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