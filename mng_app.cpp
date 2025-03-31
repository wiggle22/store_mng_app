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
#include <limits>
#include <pthread.h>

#include "mng_app.h"

ofstream file;
ofstream myfile;
Category arr[MAX_SIZE];

// Menu
class Menu {
    public:
        Menu();
        ~Menu() {}
    
        void printMenu();
        void clearMenu();
        int getNumberOfItems() const { return _numberOfItems; }
        const string* getItems() const { return items; }
    
    private:
        static const int _numberOfItems = 10;
        string items[_numberOfItems];
};
 
/* Class menuu */
Menu::Menu() {
    items[0] = "1. Enter customer information";
    items[1] = "2. Delete customer information";
    items[2] = "3. Delete all customer information";
    items[3] = "4. Search customer information";
    items[4] = "5. Display all customer information";
    items[5] = "6. Product catalog";
    items[6] = "7. Update customer information";
    items[7] = "8. Update product catalog";
    items[8] = "9. Add new catalog";
    items[9] = "10. Exit";
}

/* Funtion to show customer list */
node readCustomers() {
    node head = NULL;
    ifstream file("customerdata.txt");
    if (!file.is_open()) {
        cout << "[ERROR] Unable to open file customerdata.txt" << endl;
        return head;
    }

    string line;
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue; 

        Customer cus;
        stringstream ss(line);
        getline(ss, cus.name, ',');
        getline(ss, cus.age, ',');
        getline(ss, cus.phonenumber, ',');
        getline(ss, cus.item, ',');
        getline(ss, cus.storage, ',');
        getline(ss, cus.price, ',');
        getline(ss, cus.trade_date, ',');
        getline(ss, cus.warranty);

        node tmp = new Node();
        tmp->cus = cus;
        tmp->next = head;
        head = tmp;
    }
    file.close();
    return head;
}

/* Function to show phone category */
void readCategory() {
    ifstream myfile("category.txt");
    int i = 0;

    if (!myfile.is_open()) {
        cout << "[ERROR] Unable to open file category.txt" << endl;
        return;
    }

    string line;
    while (getline(myfile, line)) {
        if (line.empty()) continue;  // Skip empty lines

        stringstream ss(line);
        getline(ss, arr[i].namepro, ',');
        getline(ss, arr[i].storage, ',');
        getline(ss, arr[i].price, ',');
        getline(ss, arr[i].warranty);
        i++;
    }
    myfile.close();

    cout << "----------------------------------------------------------------\n";
    cout << setiosflags(ios::left) << setw(20) << "Product";
    cout << setiosflags(ios::left) << setw(15) << "Storage";
    cout << setiosflags(ios::left) << setw(20) << "Price";
    cout << setiosflags(ios::left) << setw(20) << "Warranty";
    cout << endl;
    cout << "----------------------------------------------------------------\n";

    for (int j = 0; j < i; j++) {
        cout << setiosflags(ios::left) << setw(20) << arr[j].namepro;
        cout << setiosflags(ios::left) << setw(15) << arr[j].storage;
        cout << setiosflags(ios::left) << setw(20) << formatPrice(arr[j].price);
        cout << arr[j].warranty << endl;
    }
}


/* Function to add info new customer */
node makeNode() {
    Customer cus;
    // Input and format customer name
    cout << "=> Customer name: ";
    getline(cin.ignore(), cus.name);
    cus.name = removeSpecialChars(cus.name);
    cus.name = formatName(cus.name);

    // Input and format customer age
    cout << "=> Customer age: ";
    cin >> cus.age;
    cus.age = formatAge(cus.age);
    cin.ignore();  // Clear extra character after input

    // Input and format customer phone number
    cout << "=> Customer phone number: ";
    cin >> cus.phonenumber;
    cus.phonenumber = formatPhoneNumber(cus.phonenumber);
    cin.ignore();  // Clear extra character after input

    // Input and validate product item
    bool found = false;
    while (!found) {
        cout << "=> Product item: ";
        getline(cin, cus.item);
        cus.item = removeSpecialChars(cus.item);
        cus.item = formatName(cus.item);

        ifstream myfile("category.txt");
        string line;
        int i = 0;

        if (myfile.is_open()) {
            while (!myfile.eof()) {
                getline(myfile, arr[i].namepro, ',');
                getline(myfile, arr[i].storage, ',');
                getline(myfile, arr[i].price, ',');
                getline(myfile, arr[i].warranty);
                i++;
            }
            myfile.close();
        }

        for (int j = 0; j < i; j++) {
            if (arr[j].namepro == cus.item) {
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "[WARNING] The product you entered is not in the catalog.\n";
            cout << "Please try again." << endl;
        }
    }

    // Input and validate phone storage capacity
    found = false;
    while (!found) {
        cout << "=> Phone storage capacity: ";
        cin >> cus.storage;
        cus.storage = removeSpecialChars(cus.storage);

        ifstream myfile("category.txt");
        string line;
        int i = 0;

        if (myfile.is_open()) {
            while (!myfile.eof()) {
                getline(myfile, arr[i].namepro, ',');
                getline(myfile, arr[i].storage, ',');
                getline(myfile, arr[i].price, ',');
                getline(myfile, arr[i].warranty);
                i++;
            }
            myfile.close();
        }

        for (int j = 0; j < i; j++) {
            if (arr[j].namepro == cus.item && arr[j].storage == cus.storage) {
                cus.price = arr[j].price;
                cus.warranty = arr[j].warranty;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "[WARNING] The storage capacity you entered does not exist.\n";
            cout << "Please try again." << endl;
        }
    }

    // Format price and trade date
    cus.price = formatPrice(cus.price);
    cus.trade_date = formatDate(cus.trade_date);

    // Create a new node and write to file
    ofstream file("customerdata.txt", ios::app);
    node tmp = new Node();
    tmp->cus = cus;
    tmp->next = NULL;

    if (file.is_open()) {
        file << cus.name << "," << cus.age << ","
             << cus.phonenumber << "," << cus.item << ","
             << cus.storage << "," << cus.price << ","
             << cus.trade_date << "," << cus.warranty << endl;
        file.close();
    } else {
        cout << "[ERROR] Unable to open file customerdata.txt" << endl;
    }

    return tmp;
}

/* Check emty */
bool isEmpty(node a) {
    return a == NULL;
}

/* Function to get first name */
string getFirstName(const string& fullName) {
    int lastSpacePos = fullName.rfind(" ");
    return lastSpacePos == string::npos ? fullName : fullName.substr(lastSpacePos + 1);
}

/* Function to sort list */
void sortList(node head) {
    if (head == NULL) {
        cout << "[WARNING] The list is empty!" << endl;
        return;
    }
    // Sort the list by first name
    for (node i = head; i != NULL; i = i->next) {
        node minNode = i;
        for (node j = i->next; j != NULL; j = j->next) {
            if (getFirstName(minNode->cus.name) > getFirstName(j->cus.name)) {
                minNode = j;
            }
        }
        // Swap customer information
        if (minNode != i) {
            swap(minNode->cus, i->cus);
        }
    }
    // Write the sorted list to the file
    ofstream file("customerdata.txt", ios::trunc);  // Open file with truncate mode
    if (file.is_open()) {
        for (node temp = head; temp != NULL; temp = temp->next) {
            file << temp->cus.name << "," << temp->cus.age << ","
                 << temp->cus.phonenumber << "," << temp->cus.item << ","
                 << temp->cus.storage << "," << temp->cus.price << ","
                 << temp->cus.trade_date << "," << temp->cus.warranty << endl;
        }
        file.close();
    } else {
        cout << "[ERROR] Unable to open file for writing!" << endl;
    }
}

/* Function to add customer */
void insertCustomer(node &a) {
    node tmp = makeNode();
    tmp->next = a;
    a = tmp;
    sortList(a);  // Sort the list after insertion
}

/* Function to delete info of all customer */
void deleteAllCustomers(node &a) {
    // Free the entire linked list
    while (a != NULL) {
        node tmp = a;
        a = a->next;
        delete tmp;
    }
    // Clear the contents of the file
    ofstream file("customerdata.txt", ios::trunc);
    if (file.is_open()) {
        file.close();
    }
    cout << "All customer information has been deleted" << endl;
}

/* Function to delete specified customer */
void deleteCustomer(node &a) {
    if (isEmpty(a)) {
        cout << "[WARNING] No customer found." << endl;
        return;
    }

    Customer check;
    string phoneNumber, choice;
    cout << "=> Customer phone number: ";
    getline(cin.ignore(), phoneNumber);
    phoneNumber = removeSpecialChars(phoneNumber);

    node current = a;
    node previous = NULL;
    bool found = false;

    while (current != NULL) {
        check = current->cus;
        if (phoneNumber == check.phonenumber) {
            found = true;

            // If it is the first node
            if (previous == NULL) {
                a = current->next;
            } else {
                previous->next = current->next;
            }

            delete current;  // Free memory
            cout << "Customer deleted successfully." << endl;

            // Update the file after deletion
            ofstream file("customerdata.txt", ios::trunc);
            if (file.is_open()) {
                node temp = a;
                while (temp != NULL) {
                    file << temp->cus.name << "," << temp->cus.age << ","
                         << temp->cus.phonenumber << "," << temp->cus.item << ","
                         << temp->cus.storage << "," << temp->cus.price << ","
                         << temp->cus.trade_date << "," << temp->cus.warranty << endl;
                    temp = temp->next;
                }
                file.close();
            } else {
                cout << "Error opening customerdata.txt for update." << endl;
            }
            return;
        }
        // Update previous and current pointers
        previous = current;
        current = current->next;
    }

    if (!found) {
        cout << "[WARNING] Phone number not found." << endl;
        cout << "Would you like to try again (yes/no)? ";
        cin >> choice;

        // Convert to lowercase for comparison
        transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
        if (choice == "yes") {
            deleteCustomer(a);
        }
    }
}

/* Function to print header */
void printHeader() {
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    cout << left << setw(30) << "Name";
    cout << left << setw(20) << "Age";
    cout << left << setw(20) << "Phone Number";
    cout << left << setw(20) << "Product";
    cout << left << setw(10) << "Storage";
    cout << left << setw(20) << "Price";
    cout << left << setw(20) << "Purchase Date";
    cout << left << setw(20) << "Warranty";
    cout << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
}

/* Function to print customer */
void printCustomer(Customer cus) {
    cout << left << setw(30) << cus.name;
    cout << left << setw(20) << cus.age;
    cout << left << setw(20) << cus.phonenumber;
    cout << left << setw(20) << cus.item;
    cout << left << setw(10) << cus.storage;
    cout << left << setw(20) << cus.price;
    cout << left << setw(20) << cus.trade_date;
    cout << cus.warranty;
    cout << endl;
}

/* Function to print info of customer */
void printInfo(node &a) {
    if (isEmpty(a)) {
        cout << "[WARNING] No customer found." << endl;
        return;
    }

    printHeader();
    sortList(a);
    node tmp = a;
    while (tmp != NULL) {
        printCustomer(tmp->cus);
        tmp = tmp->next;
    }
}

/* Function to check item */
bool checkItem(const string& choice, const string& item) {
    if (choice == "1") 
        return (item == "iPhone X" || item == "iPhone Xs" || item == "iPhone Xs Max");
    if (choice == "2") 
        return (item == "iPhone 11" || item == "iPhone 11 Pro" || item == "iPhone 11 Pro Max");
    if (choice == "3") 
        return (item == "iPhone 12" || item == "iPhone 12 Pro" || item == "iPhone 12 Pro Max");
    if (choice == "4") 
        return (item == "iPhone 13" || item == "iPhone 13 Pro" || item == "iPhone 13 Pro Max");
    if (choice == "5") 
        return (item == "iPhone 14" || item == "iPhone 14 Pro" || item == "iPhone 14 Pro Max");
    if (choice == "6") 
        return (item == "iPhone 15" || item == "iPhone 15 Pro" || item == "iPhone 15 Pro Max");
    return false;
}

/* Function to print info of item */
void printInfoByItem(node &a) {
    if (isEmpty(a)) {
        cout << "[WARNING] No customer found." << endl;
        return;
    }

    string choice;
    cout << "1. iPhone X/Xs/Xs Max\n";
    cout << "2. iPhone 11 Series\n";
    cout << "3. iPhone 12 Series\n";
    cout << "4. iPhone 13 Series\n";
    cout << "5. iPhone 14 Series\n";
    cout << "6. iPhone 15 Series\n";
    cout << "Select product: ";

    while(true) {
        cin >> choice;
        if (choice == "1" || choice == "2" || choice == "3" || choice == "4" || choice == "5" || choice == "6" ) break;
        else cout << "Please try again: ";
    }

    bool found = false;
    printHeader();
    for (node k = a; k != NULL; k = k->next) {
        if (checkItem(choice, k->cus.item)) {
            printCustomer(k->cus);
            found = true;
        }
    }

    if (!found) {
        cout << "[WARNING] No customer found with the corresponding product." << endl;
    }
}

/* Function to print info of specified customer */
void printOneInfo(node &a) {
    if (isEmpty(a)) {
        cout << "[WARNING] No customer found." << endl;
        return;
    }

    string phoneNumber, choice;
    bool found = false;

    cout << "=> Customer phone number: ";
    cin >> phoneNumber;

    while (!found) {
        for (node i = a; i != NULL; i = i->next) {
            if (phoneNumber == i->cus.phonenumber) {
                printHeader();
                printCustomer(i->cus);
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "[WARNING] Invalid phone number." << endl;
            cout << "Would you like to try again (yes/no)? ";
            cin >> choice;
            if (choice == "yes") {
                cout << "=> Customer phone number: ";
                cin >> phoneNumber;
            } else {
                break;
            }
        }
    }
}

/* Function to update file customer */
void updateFile(node &a) {
    ofstream file("customerdata.txt", ios::out);
    if (file.is_open()) {
        node temp = a;
        while (temp) {
            file << temp->cus.name << "," << temp->cus.age << ","
                 << temp->cus.phonenumber << "," << temp->cus.item << ","
                 << temp->cus.storage << "," << temp->cus.price << ","
                 << temp->cus.trade_date << "," << temp->cus.warranty << endl;
            temp = temp->next;
        }
        file.close();
        cout << "Customer information updated successfully." << endl;
    } else {
        cout << "[ERROR] Unable to open customerdata.txt" << endl;
    }
}

/* Function to check option */
bool askYesNo(const string &message) {
    string selection;
    cout << message << " (yes/no)? ";
    cin >> selection;
    transform(selection.begin(), selection.end(), selection.begin(), ::tolower);
    return (selection == "yes");
}

/* Function to change info of customer */
void changeCustomer(node &a) {
    if (isEmpty(a)) {
        cout << "[WARNING] No customer found." << endl;
        return;
    }

    Customer check;
    string phoneNumber;
    bool found = false;

    cout << "=> Customer phone number: ";
    getline(cin.ignore(), phoneNumber);
    phoneNumber = removeSpecialChars(phoneNumber);

    while (!found) {
        for (node i = a; i != NULL; i = i->next) {
            check = i->cus;
            if (phoneNumber == check.phonenumber) {
                printHeader();
                printCustomer(check);

                // Update customer information
                if (askYesNo("Do you want to change the name")) {
                    cout << "New name: ";
                    getline(cin.ignore(), check.name);
                    check.name = formatName(check.name);
                }

                if (askYesNo("Do you want to change the age")) {
                    cout << "New age: ";
                    cin >> check.age;
                    check.age = formatAge(check.age);
                }

                if (askYesNo("Do you want to change the phone number")) {
                    cout << "New phone number: ";
                    cin >> check.phonenumber;
                    check.phonenumber = formatPhoneNumber(check.phonenumber);
                }

                // Update customer information
                i->cus = check;

                // Write the updated list to the file
                updateFile(a);
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "[WARNING] Invalid phone number." << endl;
            if (askYesNo("Would you like to try again")) {
                cout << "=> Customer phone number: ";
                cin >> phoneNumber;
            } else {
                break;
            }
        }
    }
}

/* Function to update items in category.txt */
void updateCategory() {
    ifstream myfile("category.txt");
    Category tempArr[MAX_SIZE];
    int count = 0;
    bool found = false;

    // Read existing categories into temporary array
    if (!myfile.is_open()) {
        cout << "[ERROR] Unable to open file category.txt" << endl;
        return;
    }

    string line;
    while (getline(myfile, line) && count < MAX_SIZE) {
        if (line.empty()) continue;

        stringstream ss(line);
        getline(ss, tempArr[count].namepro, ',');
        getline(ss, tempArr[count].storage, ',');
        getline(ss, tempArr[count].price, ',');
        getline(ss, tempArr[count].warranty);
        count++;
    }
    myfile.close();

    // Display current catalog
    cout << "\nCurrent Product Catalog:\n";
    readCategory();

    // Get product to update
    string productName, storage;
    cout << "\nEnter product name to update (exact match): ";
    getline(cin.ignore(), productName);
    productName = removeSpecialChars(productName);
    productName = formatName(productName);

    cout << "Enter storage capacity to update (exact match): ";
    getline(cin, storage);
   
    storage = removeSpecialChars(storage);

    // Find and update the item
    for (int i = 0; i < count; i++) {
        if (tempArr[i].namepro == productName && tempArr[i].storage == storage) {
            found = true;
            cout << "\nUpdating: " << tempArr[i].namepro << " " << tempArr[i].storage << endl;
            
            // Ask which fields to update
            bool changePrice = askYesNo("Do you want to change the price");
            cout << "[DEBUG] Change price selected: " << (changePrice ? "yes" : "no") << endl; // Debug output
            if (changePrice) {
                string rawPrice;
                cout << "New price (e.g., 36,000,000): ";
                cin >> rawPrice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer after price input
                
                // Remove thousand separators and format for storage
                rawPrice.erase(remove_if(rawPrice.begin(), rawPrice.end(), [](char c) {
                    return c == ',' || c == '.'; // Remove commas or dots used as thousand separators
                }), rawPrice.end());
                
                tempArr[i].price = rawPrice; // Only update price if "yes"
            } else {
                cout << "Price will remain unchanged: " << tempArr[i].price << endl;
            }

            bool changeWarranty = askYesNo("Do you want to change the warranty");
            cout << "[DEBUG] Change warranty selected: " << (changeWarranty ? "yes" : "no") << endl; // Debug output
            if (changeWarranty) {
                string warrantyInput;
                cout << "New warranty (enter number of years, e.g., 2): ";
                cin >> warrantyInput;
                cin.ignore(); // Clear buffer after warranty
                if (warrantyInput.empty()) {
                    cout << "[WARNING] Invalid warranty input. Defaulting to 1 year.\n";
                    tempArr[i].warranty = "1 year";
                } else {
                    int years = stoi(warrantyInput); // Convert string to integer
                    if (years >= 2) {
                        tempArr[i].warranty = warrantyInput + " years";
                    } else {
                         tempArr[i].warranty = warrantyInput + " year";
                    }
                }
            } else {
                cout << "Warranty will remain unchanged: " << tempArr[i].warranty << endl;
            }

            cout << "Product updated successfully.\n";
            break;
        }
    }

    if (!found) {
        cout << "[WARNING] Product with specified name and storage not found.\n";
        return;
    }

    // Write updated data back to file
    ofstream outFile("category.txt", ios::trunc);
    if (!outFile.is_open()) {
        cout << "[ERROR] Unable to open file category.txt for writing" << endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        outFile << tempArr[i].namepro << ","
                << tempArr[i].storage << ","
                << tempArr[i].price << ","
                << tempArr[i].warranty << endl;
    }
    outFile.close();

    cout << "Category file updated successfully.\n";
    cout << "\nUpdated Product Catalog:\n";
    readCategory();
}

void addNewProduct() {
    Category newProduct;
    unordered_set<string> existingProducts = getExistingIphonesWithStorage("category.txt");

    cin.ignore();
    // Input product name
    while (true) {
        cout << "=> Product name: ";
        getline(cin, newProduct.namepro);

        newProduct.namepro = removeSpecialChars(newProduct.namepro);
        newProduct.namepro = formatName(newProduct.namepro);

        if (newProduct.namepro.empty()) {
            cout << "[WARNING] Product name cannot be empty.\n";
            continue;
        }

        if (!isValidIphoneModel(newProduct.namepro)) {
            cout << "[WARNING] Invalid product name. Please enter a valid iPhone model.\n";
            continue;
        }

        break;
    }

    // Input storage capacity 
    int attempts = 0;
    while (attempts < 5) {
        cout << "=> Storage capacity (e.g., 64/128/256/512 or 1TB): ";
        cin >> newProduct.storage;
        newProduct.storage = removeSpecialChars(newProduct.storage);

        if (newProduct.storage == "1TB") {
            break;
        }

        // Check valid
        if (!isValidNumber(newProduct.storage) || newProduct.storage.empty()) {
            cout << "[WARNING] Invalid storage capacity. Please enter a numeric value.\n";
            attempts++;
            continue;
        }
        
        newProduct.storage = newProduct.storage + "GB";
        break;
    }

    if (attempts == 5) {
        cout << "[ERROR] Too many invalid attempts. Returning to menu.\n";
        return;
    }

    // Check if the iPhone model with this storage already exists
    string productKey = newProduct.namepro + "," + newProduct.storage;
    if (existingProducts.find(productKey) != existingProducts.end()) {
        cout << "[WARNING] This iPhone model with the specified storage already exists. Please enter a different product.\n";
        return;
    }

    // Input price
    attempts = 0;
    while(attempts < 5){
        cout << "=> New price (e.g., 36000000): ";
        cin >> newProduct.price;

        if (!isValidNumber(newProduct.price) || (newProduct.price).empty()) {
            cout << "[WARNING] Invalid price. Please enter a numeric value.\n";
            attempts++;
            continue;
        }

        newProduct.price += " VND";  // Thêm đơn vị tiền tệ
        break;
    }

    if (attempts == 5) {
        cout << "[ERROR] Too many invalid attempts. Returning to menu.\n";
        return;
    }

    // Input warranty
    attempts = 0;
    while(attempts < 5){
        cout << "=> Warranty period (in years, e.g., 2): ";
        cin >> newProduct.warranty;
        // Validate warranty input
        if (!isValidNumber(newProduct.warranty) || (newProduct.warranty.empty())) {
            cout << "[WARNING] Invalid warranty period. Please enter a numeric value.\n";
            attempts++;
            continue;
        }
        try {
            long long years = stoll(newProduct.warranty); // Chuyển sang số nguyên lớn hơn
    
            if (years >= 2) {
                newProduct.warranty += " years";
            } else {
                newProduct.warranty += " year";
            }
            break;
    
        } catch (const out_of_range&) {
            cout << "[WARNING] Warranty period is too large. Please enter a reasonable number.\n";
            attempts++;
        }
    }

    if (attempts == 5) {
        cout << "[ERROR] Too many invalid attempts. Returning to menu.\n";
        return;
    }
    

    // Open file in append mode
    ofstream outFile("category.txt", ios::app);
    if (!outFile.is_open()) {
        cout << "[ERROR] Unable to open file category.txt for writing" << endl;
        return;
    }

    // Write new product to file in CSV format
    outFile << newProduct.namepro << ","
            << newProduct.storage << ","
            << newProduct.price << ","
            << newProduct.warranty << endl;

    outFile.close();

    cout << "New product added successfully.\n";
    cout << "\nUpdated Product Catalog:\n";
    readCategory(); // Display updated catalog
}

/* MENU */
void gotoxy(int column, int line) {
    printf("\033[%d;%dH", line, column);
    fflush(stdout);
}

/* kbhit() in Linux */
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

/* getch() in Linux */
char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
    return buf;
}

/* Function to move pointer */
int move() {
    char c = getch();
    if (c == 27) { // ESC key
        if (kbhit()) {
            getch(); // Discard the '[' character
            switch (getch()) {
                case 'A': return 2; // Up
                case 'B': return 1; // Down
                case 'C': return 3; // Right
                case 'D': return 4; // Left
            }
        }
        return 8; // ESC
    } else if (c == 10) {
        return 5; // Enter
    }
    return 0;
}

/* Print Menu */
void Menu::printMenu() {
    for (int i = 0; i < _numberOfItems; i++) {
        gotoxy(3, 10 + i);
        cout << items[i];
        /* Reduce delay for faster display */
        usleep(50000);
    }
    cout << "\n=============================================================";
}
 
/* Clear Menu */
void Menu::clearMenu() {
    for (int i = 0; i < _numberOfItems; i++) {
        gotoxy(0, 8 + i);
        /* Clear by printing spaces */
        cout << string(items[i].size() + 3, ' ');
        /* Reduce delay */
        usleep(50000);
    }
}
    
/* Function to display the banner title */
void printBanner() {
    /* Draw border for clock */
    cout << "\t \t \t.-------------.\n";
    cout << "\t \t \t|    CLOCK    |\n";
    cout << "\t \t \t'-------------'";
    /* Draw app banner */
    cout << "\n\n=============================================================";
    cout << "\n \t-----------------------------------------------";
    cout << "\n\t| Customer Management System for iPhone Store |";
    cout << "\n \t-----------------------------------------------";
}

/* Function to exit the program */
void exitProgram() {
    system("clear");
    cout << "\n******* Have a great day *******\n" << endl;
    cout << "************ See you again *************\n" << endl;
    file.close();
    exit(0);
}

/* Function to handle menu options */
void handleMenuOption(MenuOption option, node &head) {
    gotoxy(0, 20);
    switch (option) {
        case ENTER_INFORMATION:
            cout << "\n1. Enter custormer information\n";
            insertCustomer(head);
            break;
        case DELETE_INFORMATION:
            cout << "\n2. Delete customer information\n";
            deleteCustomer(head);
            break;
        case DELETE_ALL:
            cout << "\n3. Delete all customer information\n";
            deleteAllCustomers(head);
            break;
        case SEARCH_INFORMATION: {
            cout << "\n4. Search customer information\n";
            string choice;
            cout << "a. Search by customer phone number\n";
            cout << "b. Search by product\n";
            cout << "=> Please choose: ";
            getline(cin.ignore(), choice);
            choice = removeSpecialChars(choice);
            if (choice == "a")
                printOneInfo(head);
            else if (choice == "b")
                printInfoByItem(head);
            break;
        }
        case DISPLAY_ALL_INFORMATION:
            cout << "\n5. Display all customer information\n";
            printInfo(head);
            break;
        case PRODUCT_CATALOG:
            cout << "\n6. Product catalog\n";
            readCategory();
            break;
        case UPDATE_INFORMATION:
            cout << "\n7. Update customer information\n"; 
            changeCustomer(head);
            break;
        case UPDATE_CATALOG:
            cout << "\n8. Update catalog\n";
            updateCategory();
            break;
        case ADD_NEW_PRODUCT:
            cout << "\n9. Add new catalog\n";
            addNewProduct();
            break;
        case EXIT:
            exitProgram();
            break;
    }
}

/* Thread of main app */
void* runApp(void* arg){
    node head = NULL;
    file.open("customerdata.txt", ios::out | ios::app);
    head = readCustomers();
    Menu menu;
    int x;
    /* Starting position of the cursor */
    int line = 10;  
    bool exitProgram = false;

    /* Display banner and menu */
    printBanner();
    menu.printMenu();
    gotoxy(0, line);
    /* Draw the cursor pointing to the menu */
    cout << (char)1;

    while (!exitProgram) {
        if (kbhit()) {
            x = move();
            gotoxy(0, line);

            switch (x) {
                case 1:
                /* Move down */
                case 3: 
                    line++;
                    if (line >= menu.getNumberOfItems() + 10) line = 10;
                    break;
                case 2:
                /* Move up */
                case 4: 
                    line--;
                    if (line < 10) line = menu.getNumberOfItems() + 10 - 1;
                    break;
                /* Confirm menu selection */
                case 5:
                    handleMenuOption(static_cast<MenuOption>(line - 9), head);
                    system("read -p 'Press any key to continue...' var");
                    system("clear");
                    printBanner();
                    menu.printMenu();
                    break;
                /* Exit the program */
                case 8: 
                    exitProgram = true;
                    break;
            }
            gotoxy(0, line);
            /* Draw the cursor at the new position */
            cout << (char)1;
        }
    }
    return 0;
}

/* Thread of clock */
void* clockThread(void* arg) {
    while (true) {
        /* current position of pointer */
        printf("\033[s"); 
        /* move pointer to line 2, colum 25 */
        printf("\033[2;25H");
        /* get time */
        time_t now = time(0);
        tm* ltm = localtime(&now);
        printf("| 🕒 %02d:%02d:%02d |", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
        /* back to previous position of pointer */
        printf("\033[u");  
        fflush(stdout);
        sleep(1);
    }
    return NULL;
}


/* Main function */
int main() {
    pthread_t clockT, appT;

    /* Create thread of show clock  */
    if (pthread_create(&clockT, NULL, clockThread, NULL) != 0) {
        cerr << "[ERROR]: Unable create clock thread!\n" << endl;
        return 1;
    }

    /* Create thread of main app */
    if (pthread_create(&appT, NULL, runApp, NULL) != 0) { 
        cerr << "[ERROR]: Unable create main app thread!\n" << endl;
        return 1;
    }

    /* Wait and finish 2 threads */
    pthread_join(clockT, NULL);
    pthread_join(appT, NULL);
    return 0;
}
