#ifndef MNG_APP_H
#define MNG_APP_H

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
#include <limits>
#include <unordered_set>
#include <regex>

using namespace std;

const int MAX_SIZE = 100;

struct Customer {
	string name, age, phonenumber, item, price, storage, trade_date, warranty;
};

struct Category {
	string namepro, storage, price, warranty; 
};

struct Node{
	Customer cus;
	Node *next;
};

// Define enum for menu options
enum MenuOption {
    ENTER_INFORMATION = 1,
    DELETE_INFORMATION,
    DELETE_ALL,
    SEARCH_INFORMATION,
    DISPLAY_ALL_INFORMATION,
    PRODUCT_CATALOG,
    UPDATE_INFORMATION,
    UPDATE_CATALOG,
    ADD_NEW_PRODUCT,
    BACKUP_DATA,
    EXIT
};

typedef struct Node* node;

/* Format Function */
string formatName(string &a);
string formatPhoneNumber(string &a);
bool isValidPhoneNumber(const string &a);
string formatDate(string &a);
string formatAge(string &a);
string formatPrice(string a);
bool isValidNumber(const string& str);
bool isValidIphoneModel(const string& model);
unordered_set<string> getExistingIphonesWithStorage(const string& filename);

#endif