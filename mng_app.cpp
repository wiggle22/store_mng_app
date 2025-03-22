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

ofstream file;
ofstream myfile;
Category arr[MAX_SIZE];

node readCustomers() {
    node head = NULL;
    ifstream file("customerdata.txt");
    if (!file.is_open()) {
        cout << "[ERROR] Unable to open file customerdata.txt" << endl;
        return head;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

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

// Function to remove non-alphanumeric characters
string removeSpecialChars(const string &input) {
    string result;
    for (char ch : input) {
        if (isalnum(ch) || ch == ' ' || ch == '.') {  // Keep letters, digits, spaces, and dots
            result += ch;
        }
    }
    return result;
}

node makeNode() {
    Customer cus;
    cout << "* Enter customer information *" << endl;

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

        while (getline(myfile, line)) {
            if (line.find(cus.item) != string::npos) {
                found = true;
                break;
            }
        }
        myfile.close();

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

bool isEmpty(node a) {
    return a == NULL;
}

string getFirstName(const string& fullName) {
    int lastSpacePos = fullName.rfind(" ");
    return lastSpacePos == string::npos ? fullName : fullName.substr(lastSpacePos + 1);
}

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

void insertCustomer(node &a) {
    node tmp = makeNode();
    tmp->next = a;
    a = tmp;
    sortList(a);  // Sort the list after insertion
}

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

void deleteCustomer(node &a) {
    if (isEmpty(a)) {
        cout << "[WARNING] No customer found." << endl;
        return;
    }

    Customer check;
    string phoneNumber, choice;
    cout << "=> Customer phone number: ";
    cin >> phoneNumber;

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
        if (choice == "yes" || choice == "no") {
            deleteCustomer(a);
        }
    }
}

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
    return false;
}

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
    cout << "Select product: ";
    cin >> choice;

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
            if (choice == "yes" || choice == "no") {
                cout << "=> Customer phone number: ";
                cin >> phoneNumber;
            } else {
                break;
            }
        }
    }
}

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

bool askYesNo(const string &message) {
    string selection;
    cout << message << " (yes/no)? ";
    cin >> selection;
    transform(selection.begin(), selection.end(), selection.begin(), ::tolower);
    return (selection == "yes" || selection == "no");
}

void changeCustomer(node &a) {
    if (isEmpty(a)) {
        cout << "[WARNING] No customer found." << endl;
        return;
    }

    Customer check;
    string phoneNumber;
    bool found = false;

    cout << "=> Customer phone number: ";
    cin >> phoneNumber;

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

                cin.ignore(); // Clear newline character after input
                if (askYesNo("Do you want to change the product")) {
                    cout << "New product: ";
                    getline(cin, check.item);
                    check.item = formatName(check.item);
                }

                if (askYesNo("Do you want to change the storage")) {
                    cout << "New storage: ";
                    cin >> check.storage;
                    check.storage = formatPrice(check.storage);
                }

                if (askYesNo("Do you want to change the price")) {
                    cout << "New price: ";
                    cin >> check.price;
                    check.price = formatPrice(check.price);
                }

                if (askYesNo("Do you want to change the warranty period")) {
                    cout << "New warranty period: ";
                    cin >> check.warranty;
                    check.warranty += " years";
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

// MENU
void gotoxy(int column, int line) {
    printf("\033[%d;%dH", line, column);
    fflush(stdout);
}

// kbhit() in Linux
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

// getch() in Linux
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
        static const int _numberOfItems = 8;
        string items[_numberOfItems];
};
    
Menu::Menu() {
    items[0] = "1. Enter customer information";
    items[1] = "2. Delete customer information";
    items[2] = "3. Delete all customer information";
    items[3] = "4. Search customer information";
    items[4] = "5. Display all customer information";
    items[5] = "6. Product catalog";
    items[6] = "7. Update customer information";
    items[7] = "8. Exit";
}
    
void Menu::printMenu() {
    for (int i = 0; i < _numberOfItems; i++) {
        gotoxy(3, 7 + i);
        cout << items[i];
        usleep(50000); // Reduce delay for faster display
    }
    cout << "\n=============================================================";
}
    
void Menu::clearMenu() {
    for (int i = 0; i < _numberOfItems; i++) {
        gotoxy(0, 7 + i);
        cout << string(items[i].size() + 3, ' '); // Clear by printing spaces
        usleep(50000); // Reduce delay
    }
}
    
// Function to display the banner title
void printBanner() {
    cout << "\n=============================================================";
    cout << "\n \t\t-----------------------------------------------";
    cout << "\n\t\t| Customer Management System for iPhone Store |";
    cout << "\n \t\t-----------------------------------------------";
}

// Function to exit the program
void exitProgram() {
    system("clear");
    cout << "******* Have a great day *******" << endl;
    cout << "************ See you again *************" << endl;
    file.close();
    exit(0);
}

// Function to handle menu options
void handleMenuOption(MenuOption option, node &head) {
    gotoxy(0, 16);
    switch (option) {
        case ENTER_INFORMATION:
            insertCustomer(head);
            break;
        case DELETE_INFORMATION:
            deleteCustomer(head);
            break;
        case DELETE_ALL:
            deleteAllCustomers(head);
            break;
        case SEARCH_INFORMATION: {
            string choice;
            cout << "1. Search by customer phone number\n";
            cout << "2. Search by product\n";
            cout << "Please choose: ";
            cin >> choice;
            if (choice == "1")
                printOneInfo(head);
            else if (choice == "2")
                printInfoByItem(head);
            break;
        }
        case DISPLAY_ALL_INFORMATION:
            printInfo(head);
            break;
        case PRODUCT_CATALOG:
            readCategory();
            break;
        case UPDATE_INFORMATION:
            changeCustomer(head);
            break;
        case EXIT:
            exitProgram();
            break;
    }
}

// Main function
int main() {
    node head = NULL;
    file.open("customerdata.txt", ios::out | ios::app);
    head = readCustomers();
    Menu menu;
    int x;
    int line = 7;   // Starting position of the cursor
    bool exitProgram = false;

    // Display banner and menu
    printBanner();
    menu.printMenu();
    gotoxy(0, line);
    cout << (char)1;  // Draw the cursor pointing to the menu

    while (!exitProgram) {
        if (kbhit()) {
            x = move();
            gotoxy(0, line);
            cout << " ";  // Clear the cursor at the old position

            switch (x) {
                case 1:
                case 3:  // Move down
                    line++;
                    if (line >= menu.getNumberOfItems() + 7) line = 7;
                    break;
                case 2:
                case 4:  // Move up
                    line--;
                    if (line < 7) line = menu.getNumberOfItems() + 7 - 1;
                    break;
                case 5:  // Confirm menu selection
                    handleMenuOption(static_cast<MenuOption>(line - 6), head);
                    system("read -p 'Press any key to continue...' var");
                    system("clear");
                    printBanner();
                    menu.printMenu();
                    break;
                case 8:  // Exit the program
                    exitProgram = true;
                    break;
            }
            gotoxy(0, line);
            cout << (char)1;  // Draw the cursor at the new position
        }
    }

    return 0;
}
