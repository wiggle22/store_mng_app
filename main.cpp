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



using namespace std;
ofstream file;
ofstream myfile;
const int MAX_SIZE = 100;



struct Customer {
	string name, age, phonenumber, item, price, storage, trade_date, warranty;
};

struct DanhMuc {
	string namepro, storage, price, warranty; 
};

DanhMuc arr[MAX_SIZE];

struct Node{
	Customer cus;
	Node *next;
};

typedef struct Node* node;

string chuanhoachu(string &a){
	if (a[0] >= 97 && a[0] <= 122) {
        a[0] = a[0] - 32;
    }

    for (int i = 1; i < a.length(); i++) {
        if (a[i - 1] == ' ' && (a[i] >= 97 && a[i] <= 122)) {
            a[i] = a[i] - 32;
        }
        else {
            a[i] = a[i];
        }
        if (a[i - 1] != ' ' && a[i] >= 65 && a[i] <= 90) {
            a[i] = a[i] + 32;
        }
    }

    // Thay th? ch? "Iphone" b?ng "iPhone"
    size_t pos = a.find("Iphone");
    if (pos != std::string::npos) {
        a.replace(pos, 7, "iPhone");
    }

    return a;
}

int countsdt=0;
string chuanhoasdt(string &a){
	string sdt;
	if(countsdt>=1){
		cin>>sdt;
		if(sdt.length()!=10 || sdt[0]!='0'||(sdt[1]!='9'
		&&sdt[1]!='5'&&sdt[1]!='3'&&sdt[1]!='2'&&sdt[1]!='7')){
			cout<<"Nhap lai: ";
			++countsdt;
			chuanhoasdt(a);		
		}else{
			a = sdt;
			return a;
		}
	}else{
		if(a.length()!=10 || a[0]!='0'||(a[1]!='9'
		&&a[1]!='5'&&a[1]!='3'&&a[1]!='2'&&a[1]!='7')){
			cout<<"Nhap lai: ";
			++countsdt;
			chuanhoasdt(a);		
		}	
		return a;
	}
	return a;
}

string chuanhoaday(string &a){
	int day,month,year;
	ostringstream ssday,ssmonth,ssyear;
	time_t now = time(NULL);
    tm *local_time = localtime(&now);
    day = local_time->tm_mday;
    month = local_time->tm_mon+1;
    year = local_time->tm_year+1900;
    ssday << day;
    ssmonth << month;
    ssyear << year;
    a = ssday.str() + "/" + ssmonth.str() + "/" + ssyear.str();
    return a;
}

int counttuoi=0;
string chuanhoatuoi(string &a){
	int b;
	b = stoi(a);
	if(counttuoi>=1){
		string tuoi;
		int c;
		ostringstream ss;
		cin>>tuoi;
		c = stoi(tuoi);
		if(c>=1&&c<=150){
			ss << c;
			a = ss.str() +" tuoi";		
		}else{
			cout<<"Tuoi khong hop le. Vui long nhap lai: ";
			++counttuoi;
			chuanhoatuoi(a);
		}
	}else{
		if(b>=1&&b<=150){
			a = a+" tuoi";
			
		}else{
			cout<<"Tuoi khong hop le. Vui long nhap lai: ";
			++counttuoi;
			chuanhoatuoi(a);
		}
	}
	return a ;
}

string chuanhoagia(string a) {
    while (a.back() == '.') {
        a.pop_back();
    }
    for (int i = 0; i < a.size(); i++) {
        if (!isdigit(a[i]) && a[i] != '.') {
            a.erase(i--, 1);
        }
    }
    double number;
    stringstream ss(a);
    ss >> number;
    ostringstream oss;
    oss << fixed << setprecision(2) << number;
    string formattedPrice = oss.str();
    for (int i = formattedPrice.find('.') - 3; i > 0; i -= 3) {
        formattedPrice.insert(i, ".");
    }
    return formattedPrice + " VND";
}

node readCus(){
    node head = NULL;
    ifstream file("data.txt");
    if(file.is_open()){
        while(!file.eof()){
            Customer cus;
            getline(file, cus.name, ',');
            getline(file, cus.age, ',');
            getline(file, cus.phonenumber, ',');
            getline(file, cus.item, ',');
			getline(file, cus.storage, ',');
            getline(file, cus.price, ',');
            getline(file, cus.trade_date, ',');
            getline(file, cus.warranty);
            if(cus.name != ""){ 
                node tmp = new Node();
                tmp->cus = cus;
                tmp->next = head;
                head = tmp;
            }
        }
        file.close();
    }
    return head;
}

void readCate(){
	ifstream myfile("danhmuc.txt");
    int i = 0;
    if(myfile.is_open()){
        while(!myfile.eof()){
            getline(myfile, arr[i].namepro, ',');
            getline(myfile, arr[i].storage, ',');
            getline(myfile, arr[i].price, ',');
            getline(myfile, arr[i].warranty);
            i++;
        }
        myfile.close();
    }

    	cout<<"----------------------------------------------------------------\n";
		cout<<setiosflags(ios::left)<<setw(20)<<"San pham";
	    cout<<setiosflags(ios::left)<<setw(15)<<"Dung luong";
	    cout<<setiosflags(ios::left)<<setw(20)<<"Gia";
	    cout<<setiosflags(ios::left)<<setw(20)<<"Bao hanh";
	    cout<<endl;
	    cout<<"----------------------------------------------------------------\n";

    for (int j = 0; j < i; j++){
        cout<<setiosflags(ios::left)<<setw(20)<<arr[j].namepro;
	    cout<<setiosflags(ios::left)<<setw(15)<<arr[j].storage;
	    cout<<setiosflags(ios::left)<<setw(20)<<chuanhoagia(arr[j].price);
	    cout<<setiosflags(ios::left)<<setw(10)<<arr[j].warranty;
	    cout<<endl;
    }


    return;
}

// Hàm loại bỏ ký tự không phải chữ cái và số
string removeSpecialChars(const string &input) {
    string result;
    for (char ch : input) {
        if (isalnum(ch) || ch == ' ' || ch == '.') {  // Giữ lại chữ cái, số và khoảng trắng, dấu chấm
            result += ch;
        }
    }
    return result;
}

node makeNode() {
    Customer cus;
    cout << "Nhap thong tin khach hang: " << endl;

    cout << "Nhap ten khach hang: ";
    getline(cin.ignore(), cus.name);
	cus.name = removeSpecialChars(cus.name);
    //cus.name = chuanhoachu(cus.name);
	
    cout << "Nhap tuoi khach hang: ";
    cin >> cus.age;
    cus.age = chuanhoatuoi(cus.age);
    cin.ignore();  // Xóa ký tự thừa sau khi nhập

    cout << "Nhap so dien thoai khach hang: ";
    cin >> cus.phonenumber;
    cus.phonenumber = chuanhoasdt(cus.phonenumber);
    cin.ignore();  // Xóa ký tự thừa sau khi nhập

    bool found = false;

    while (!found)
    {
        cout<<"Nhap mon hang: "; 
        getline(cin,cus.item);
		cus.item = removeSpecialChars(cus.item);
		//cus.item=chuanhoachu(cus.item);

        ifstream myfile("danhmuc.txt");
        string line;

        while (getline(myfile, line))
        {
            if (line.find(cus.item) != string::npos)
            {
            	found = true;
            	break;
            }
        }

        if (!found){
            cout << "San pham ban vua nhap khong co trong danh muc\n";
			cout << "Vui long nhap lai" << endl;
        }

        file.close();
    }

	found = false;
	while (!found)
    {
        cout<<"Nhap dung luong dien thoai: ";
        cin>>cus.storage;
		cus.storage = removeSpecialChars(cus.storage);
        ifstream myfile("danhmuc.txt");
        string line;

        while (getline(myfile, line))
        {
            if (line.find(cus.storage) != string::npos)
            {
                ifstream myfile("danhmuc.txt");
				int i = 0;
			    if(myfile.is_open()){
			        while(!myfile.eof()){
			            getline(myfile, arr[i].namepro, ',');
			            getline(myfile, arr[i].storage, ',');
			            getline(myfile, arr[i].price, ',');
			            getline(myfile, arr[i].warranty);
			            i++;
			        }
			        myfile.close();
			    }
			
				for (int j = 0; j < i; j++){
			        if(arr[j].namepro == cus.item && arr[j].storage == cus.storage){
						cus.price = arr[j].price;
						cus.warranty = arr[j].warranty;
						found = true;
                		break;
					}	
			    }
            }
        }

        if (!found){
            cout << "Dung luong ban vua nhap khong ton tai\n";
			cout << "Vui long nhap lai" << endl;
        }

        file.close();
    }

    // Chuẩn hóa giá cả và ngày giao dịch
    cus.price = chuanhoagia(cus.price);
    cus.trade_date = chuanhoaday(cus.trade_date);

    // Tạo node mới
    ofstream file("data.txt", ios::app);
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
        cout << "Khong the mo file data.txt" << endl;
    }

    return tmp;
}



bool empty(node a){
	return a == NULL;
}

int size(node a){
	int count;
	while(a!=NULL){
		count++;
		a = a->next;
	}
	return count;
}

void insertCustomer(node &a){
	node tmp = makeNode();
	if(a == NULL){
		a = tmp;
	}else{
		tmp->next = a;
		a = tmp;
	}
}

void deleteAllCustomer(node &a){
	a = NULL;
	ofstream file("data.txt",ios::trunc);
	if (file.is_open()) {
    	file << "";
    	file.close();
	}
	cout<<"Da xoa toan bo thong tin khach hang" << endl;
}

void deleteCustomer(node &a){
	if(empty(a)){
		cout<<"Khong co khach hang."<<endl;
	}else{
		Customer check;
		string sdt,choice;
		cout<<"Nhap so dien thoai khach hang: ";cin>>sdt;
		node top = NULL;
		int count=0;
		for(int i=1;i<=size(a);i++){
			++count;
			check = a->cus;
			if(sdt == check.phonenumber){
				if(count == size(a)){
					top->next = NULL;
				}else if(count == 1){
					a= a->next;
				}else{
					top->next = a->next;
					a->next = NULL;
				}
				ofstream file("data.txt", ios::out);
			    if(file.is_open()){
			    	while(a){
			      	   	file << a->cus.name << "," << a->cus.age << "," 
						<< a->cus.phonenumber << "," << a->cus.item << "," 
						<< a->cus.storage << "," << a->cus.price << "," 
						<< a->cus.trade_date << "," << a->cus.warranty << endl;
			            a = a->next;
			        }
			    	file.close();
			    }
			}else{
				top = a;
				a = a->next;
				if(count == size(a) && sdt!=check.phonenumber){
					cout<<"So dien thoai khong hop le."<<endl;
					cout<<"Ban co muon thu lai khong: ";cin>>choice;
					if(choice =="co"||choice =="yes"||choice =="ok"){
						deleteCustomer(a);
					}else
						return;
				}
			}
		}
	}
}


string firstName(string fullName) {
    // tim vi tri khoang trang cuoi cung trong ho va ten
    int lastSpacePos = fullName.rfind(" ");
    
    // neu khong tim thay khoang trang thi ho se la chuoi rong (khong co ho)
    string lastName = lastSpacePos == string::npos ? "" : fullName.substr(0, lastSpacePos);
    
    // ten la phan con lai
    string firstName = lastSpacePos == string::npos ? fullName : fullName.substr(lastSpacePos + 1);
    
    return firstName;
}

void sapxep(node head){
	ofstream file("data.txt", ios::out);
	for(node i = head; i != NULL; i = i->next){
		node minNode = i;
		for(node j = i->next; j != NULL; j = j->next){
			if(firstName(minNode->cus.name) > firstName(j->cus.name)){
				minNode = j;
			}
		}
		Customer tmp = minNode->cus;
		minNode->cus = i->cus;
		i->cus = tmp;
		
		
		if(file.is_open()){
			file << tmp.name << "," << tmp.age << "," << tmp.phonenumber << "," << tmp.item << "," << tmp.storage << "," << tmp.price << "," << tmp.trade_date << "," << tmp.warranty << endl;
		}
	}
}

void bar(){
	cout<<"--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	cout << left << setw(30) <<"Ten";
    cout << left << setw(10) <<"Tuoi";
    cout << left << setw(20) <<"So dien thoai";
    cout << left << setw(20) <<"Mon hang";
	cout << left << setw(20) <<"Dung luong";
    cout << left << setw(20) <<"Gia mon hang";
    cout << left << setw(20) <<"Ngay mua hang";
    cout << left << setw(20) <<"Thoi han bao hanh";
    cout<<endl;
	cout<<"--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
}

void printCustomer(Customer cus){
    cout << left << setw(30) << cus.name;
    cout << left << setw(10) << cus.age;
    cout << left << setw(20) << cus.phonenumber;
    cout << left << setw(20) << cus.item;
    cout << left << setw(20) << cus.storage;
    cout << left << setw(20) << cus.price;
    cout << left << setw(20) << cus.trade_date;
    cout << cus.warranty;
    cout<<endl;
}

void printInfo(node &a) {
    if (empty(a)) {
        cout << "Khong co khach hang." << endl;
    } else {
        bar();
        node tmp = a;
        sapxep(a);
        while (tmp != NULL) {
            printCustomer(tmp->cus);
            tmp = tmp->next;
        }
    }
}

void printInfoByItem(node &a){
	if(empty(a)){
		cout<<"Khong co khach hang."<<endl;
	}else{
		string choice;
		cout<<"1. iPhone X/Xs/Xs Max\n";
		cout<<"2. iPhone 11 Series\n";
		cout<<"3. iPhone 12 Series\n";
		cout<<"4. iPhone 13 Series\n";
		cout<<"5. iPhone 14 Series\n";
		cout<<"Chon san pham: "; cin>>choice;
		bar();
		for(node k = a; k != NULL; k = k->next){
			if(choice == "1"){
				if(k->cus.item == "iPhone X" || k->cus.item == "iPhone Xs" || k->cus.item == "iPhone Xs Max"){
					printCustomer(k->cus);
				}
			} else if(choice == "2"){
				if(k->cus.item == "iPhone 11" || k->cus.item == "iPhone 11 Pro" || k->cus.item == "iPhone 11 Pro Max"){
					printCustomer(k->cus);
				}
			} else if(choice == "3"){
				if(k->cus.item == "iPhone 12" || k->cus.item == "iPhone 12 Pro" || k->cus.item == "iPhone 12 Pro Max"){
					printCustomer(k->cus);
				}
			} else if(choice == "4"){
				if(k->cus.item == "iPhone 13" || k->cus.item == "iPhone 13 Pro" || k->cus.item == "iPhone 13 Pro Max"){
					printCustomer(k->cus);
				}
			} else if(choice == "5"){
				if(k->cus.item == "iPhone 14" || k->cus.item == "iPhone 14 Pro" || k->cus.item == "iPhone 14 Pro Max"){
					printCustomer(k->cus);
				}
				//return;
			}
		}
	}
}

void printOneInfo(node &a){
	if(empty(a)){
		cout<<"Khong co khach hang."<<endl;
	}else{
		Customer check;
		string sdt,choice;
		bool found = false;
		cout<<"Nhap so dien thoai khach hang: ";cin>>sdt;
		while(!found){
			for(node i = a; i != NULL; i = i->next){
				check = i->cus;
				if(sdt == check.phonenumber){
					bar();
					printCustomer(check);
					found = true;
					break;
				}
			}
			if(!found){
				cout<<"So dien thoai khong hop le."<<endl;
				cout<<"Ban co muon thu lai khong: ";cin>>choice;
				if(choice =="co"||choice =="yes"||choice =="ok"){
					cout<<"Nhap so dien thoai khach hang: ";cin>>sdt;
				}else{
					found = true;
					break;
				}
			}
		}
	}
}

void changeCustomer(node &a){
	if(empty(a)){
		cout<<"Khong co khach hang."<<endl;
	}else{
		Customer check;
		string sdt,choice;
		string selection;
		string day,month,year;
		bool found = false;
		cout<<"Nhap so dien thoai khach hang: ";cin>>sdt;
		while(!found){
			for(node i = a; i != NULL; i = i->next){ 
				check = i->cus;
				if(sdt == check.phonenumber){
					bar();
					printCustomer(check);
					cout<<"Ban co muon thay doi ten khong: ";cin>>selection;
					if(selection == "co"){
						
						cout<<"Ten moi: ";getline(cin,check.name);
						check.name=chuanhoachu(check.name);
					}
					cout<<"Ban co muon thay doi tuoi khong: ";cin>>selection;
					if(selection == "co"){
						cout<<"Tuoi moi: ";cin>>check.age;
						check.age=check.age+" tuoi";
					}
					cout<<"Ban co muon thay doi so dien thoai khong: ";cin>>selection;
					if(selection == "co"){
						cout<<"So dien thoai moi: ";cin>>check.phonenumber;
						check.phonenumber=chuanhoasdt(check.phonenumber);
					}
					cout<<"Ban co muon thay doi mon hang khong: ";cin>>selection;
					if(selection == "co"){
						
						cout<<"Mon hang moi: ";getline(cin,check.item);
						check.item=chuanhoachu(check.item);
					}
					cout<<"Ban co muon thay doi dung luong dien thoai khong: ";cin>>selection;
					if(selection == "co"){
						
						cout<<"Dung luong moi: ";getline(cin,check.storage);
					}
					cout<<"Ban co muon thay doi gia mon hang khong: ";cin>>selection;
						if(selection == "co"){
						cout<<"Gia moi: ";cin>>check.price;
						check.price=chuanhoagia(check.price);
					}
					cout<<"Ban co muon thay doi thoi gian bao hanh khong: ";cin>>selection;
					if(selection == "co"){
						cout<<"Thoi gian bao hanh moi: ";cin>>check.warranty;
						check.warranty=check.warranty+" nam";
					}
					i->cus = check;
					ofstream file("data.txt", ios::out);
					if(file.is_open()){
						i = a;
						while(i){
							file << i->cus.name << "," << i->cus.age << "," 
							<< i->cus.phonenumber << "," << i->cus.item << ","
							<< i->cus.price << "," << i->cus.trade_date << ","
							<< i->cus.warranty << endl;
							i = i->next;
						}
						file.close();
					}
					found = true;
					break;
			    }
			}

			if(!found){
				cout<<"So dien thoai khong hop le."<<endl;
				cout<<"Ban co muon thu lai khong: ";cin>>choice;
				if(choice =="co"||choice =="yes"||choice =="ok"){
					cout<<"Nhap so dien thoai khach hang: ";cin>>sdt;
				}else{
					found = true;
					break;
				}
			}	
		}
	}
}


// MENU
void gotoxy(int column, int line) {
    printf("\033[%d;%dH", line, column);
    fflush(stdout);
}

// Hàm thay thế Windows' kbhit() trên Linux
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

// Hàm thay thế getch() trên Linux
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
            getch(); // Bỏ ký tự '['
            switch (getch()) {
                case 'A': return 2; // Lên
                case 'B': return 1; // Xuống
                case 'C': return 3; // Phải
                case 'D': return 4; // Trái
            }
        }
    } else if (c == 10) {
        return 5; // Enter
    } else if (c == 27) {
        return 8; // ESC
    }
    return 0;
}

// Lớp Menu
class Menu {
public:
    Menu();
    ~Menu() {}

    void printMenu();
    void deleteMenu();
    int numberOfItem() { return _numberOfItem; }
    string* getItem() { return item; }

private:
    string* item;
    int _numberOfItem;
};

Menu::Menu() {
    item = new string[8];
    _numberOfItem = 8;
    item[0] = "1. Nhap thong tin khach hang";
    item[1] = "2. Xoa thong tin khach hang";
    item[2] = "3. Xoa tat ca thong tin khach hang";
    item[3] = "4. Tim kiem thong tin khach hang";
    item[4] = "5. Thong tin tat ca khach hang";
    item[5] = "6. Danh muc san pham";
    item[6] = "7. Thay doi thong tin khach hang";
    item[7] = "8. Het gio lam";
}

void Menu::printMenu() {
    for (int i = 0; i < _numberOfItem; i++) {
        gotoxy(3, 7 + i);
        cout << item[i];
        usleep(100000); // Dừng 100ms
    }
}

void Menu::deleteMenu() {
    for (int i = 0; i < _numberOfItem; i++) {
        usleep(100000);
        gotoxy(0, i);
        cout << string(item[i].size() + 3, ' '); // Xóa bằng cách in khoảng trắng
    }
}

int main(){
	node head = NULL;
	file.open("data.txt", ios::out | ios::app);
	head = readCus();
	string mode;
	Menu menu;
	int x;
	int line = 7; //Vi tri bat dau cua pointer
	bool thoat = false;


	cout << "\n=============================================================";
	cout << "\n \t\t--------------------------------------------------------------------------";
	cout << "\n\t\t| Danh sach quan ly khach hang tai cua hang dien thoai chuyen ban iPhone |";
	cout << "\n \t\t--------------------------------------------------------------------------\n";
	menu.printMenu();
	cout << "\n=============================================================";


	gotoxy(0, line);
	cout << (char) 1; //V? con tr? tr? t?i menu

	while (!thoat) {
		if (kbhit()) {
			x = move();
			gotoxy(0, line);
			cout << " "; //X?a con tr? ? v? tr? cu
			switch (x) {
				case 1:

				case 3: {
						line++;
						if (line >= menu.numberOfItem()+7) line = 7;
						break;
					}

				case 2:

				case 4: {
						line--;
						if (line < 7) line = menu.numberOfItem() + 7 - 1;
						break;
					}

				case 5:{
						gotoxy(0, 16);
						if(menu.getItem()[line-7] == "1. Nhap thong tin khach hang")
							insertCustomer(head);
						else if (menu.getItem()[line-7] == "2. Xoa thong tin khach hang")
							deleteCustomer(head);
						else if (menu.getItem()[line-7] == "3. Xoa tat ca thong tin khach hang")
							deleteAllCustomer(head);
						else if (menu.getItem()[line-7] == "4. Tim kiem thong tin khach hang"){
							string choice;
							cout<<"1. Tim kiem theo so dien thoai khach hang\n";
							cout<<"2. Tim kiem theo san pham\n"; 
							cout<<"Moi ban chon: "; cin>>choice;
							if(choice == "1")
								printOneInfo(head);
							else if(choice == "2")
								printInfoByItem(head);
						}
						else if (menu.getItem()[line-7] == "5. Thong tin tat ca khach hang")
							printInfo(head);
						else if (menu.getItem()[line-7] == "6. Danh muc san pham")
							readCate();
						else if (menu.getItem()[line-7] == "7. Thay doi thong tin khach hang")
							changeCustomer(head);
						else if (menu.getItem()[line-7] == "8. Het gio lam"){
							system("clear");
							cout<<"******* Chuc mot ngay tot lanh *******"<<endl;
							cout<<"************ Hen gap lai *************"<<endl;
							file.close();
							thoat = true;
							break;
						}
						system("read -p 'Nhan phim bat ky de tiep tuc...' var");
						system("clear");
						cout << "\n=============================================================";
						cout << "\n \t\t--------------------------------------------------------------------------";
						cout << "\n\t\t| Danh sach quan ly khach hang tai cua hang dien thoai chuyen ban iPhone |";
						cout << "\n \t\t--------------------------------------------------------------------------\n";
						menu.printMenu();
						cout << "\n=============================================================";
						break;
					}

				case 8:{
					system("clear");
					cout<<"******* Chuc mot ngay tot lanh *******"<<endl;
					cout<<"************ Hen gap lai *************"<<endl;
					file.close();
					thoat = true;
					break;
				}
					
			}
			gotoxy(0, line);
			cout << (char) 1;

		}
		

	}

	//menu.deleteMenu();

	return 0;
}