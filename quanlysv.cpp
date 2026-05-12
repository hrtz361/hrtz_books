#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
using namespace std;

//Ð?nh nghia l?p Person
class Person {

private:
    string name, phone;
    string correctName(string &str);

public:
    Person(string, string);
    string getName();
    string getPhone();
    string getNameSort();
    void input();
};

Person::Person(string n="", string p=""){
    name = correctName(n); phone = p;
}

string Person::getName(){
    return name;
}

string Person::getPhone(){
    return phone;
}

void Person::input(){
    //Nh?p tên và ki?m tra d? dài không quá 22 ký t?
    do{
        cout <<"Nhap ho va ten:"; cin>>ws;
        getline(cin,name);
        name = correctName(name); //Chu?n hóa xâu nh?p vào
        if (name.length()>22){
            cout<<"Khong duoc qua 22 ky tu, hay nhap lai."
                <<endl;
        }
    }while(name.length()>22);

    //Nh?p di?n tho?i và ki?m tra d? dài không quá 11 ký t?
    do{
        cout <<"Nhap dien thoai:"; cin>>phone;
        if (phone.length()>11){
            cout<<"Khong duoc qua 11 ky tu, hay nhap lai."
                <<endl;
        }
    }while(phone.length()>11);
}

string Person::getNameSort(){
    int i = name.find_last_of(' '); //Có 1 d?u cách ? gi?a
    if(i<0) return name;
    string ten = name.substr(i+1);
    string hodem = name.substr(0,i);
    return ten + " " + hodem;
}

string Person::correctName(string &str){
    string result;
    stringstream ss(str);
    string word;
    while(ss>>word){
        word[0] = toupper(word[0]);
        for(size_t i=1;i<word.length();++i){
            word[i] = tolower(word[i]);
        }
        if (!result.empty()){
            result += " ";
        }
        result += word;
    }
    return result;
}

//Ð?nh nghia l?p Date
class Date {
private:
    int day, month, year;
    bool isValidDate(int,int,int);

public:
    Date(int,int,int);
    int getDay();
    int getMonth();
    int getYear();
    void input();
};

Date::Date(int d=1,int m=1,int y=2000){
    day=d; month=m; year=y;
}

int Date::getDay(){
    return day;
}

int Date::getMonth(){
    return month;
}

int Date::getYear(){
    return year;
}

void Date::input(){
    cout<<"Nhap ngay/thang/nam sinh:"<<endl;

    //Ki?m tra ngày h?p l?
    do{
        cout <<"Nhap ngay (1-31): "; cin>>day;
        if (day<1||day>31){
            cout<<"Khong hop le, hay nhap lai."<<endl;
        }
    }while(day<1||day>31);

    //Ki?m tra tháng h?p l?
    do{
        cout <<"Nhap thang (1-12):"; cin>>month;
        if (month<1||month>12) {
            cout<<"Khong hop le, hay nhap lai."<<endl;
        }
    }while(month<1||month>12);

    //Ki?m tra nam h?p l?
    do{
        cout <<"Nhap nam (1900-2100):"; cin>>year;
        if (year<1900||year>2100){
            cout<<"Khong hop le, hay nhap lai."<<endl;
        }
    }while(year<1900||year>2100);

    //Ki?m tra tính h?p l? c?a ngày/tháng/nam
    if(!isValidDate(day,month,year)){
        cout <<"Ngay thang nam khong hop le, hay nhap lai."
             <<endl;
        Date::input(); //Nh?p l?i n?u không h?p l?
    }
}

bool Date::isValidDate(int d,int m,int y){
    if(m==4||m==6||m==9||m==11){
        return d<=30;
    } else if(m==2){
        //Ki?m tra nam nhu?n
        if ((y%4==0&&y%100 !=0)||(y%400==0)){
            return d<=29;
        } else {
            return d<=28;
        }
    }
    return true; //Tháng 1,3,5,7,8,10,12 có 31 ngày
}

class Student:public Person, public Date{
private:
    string studentID;
    float GPA;
public:
    Student(string, string, int, int, int, string, float);
    string getStudentID();
    float getGPA();
    string letterGrade();
    void input();
};

Student::Student(string n="", string p="", int d=1,
int m=1, int y=2000, string id="",
float gpa=0.0): Person(n, p), Date(d, m, y){
    studentID = id; GPA = gpa;
}

string Student::getStudentID(){
    return studentID;
}

float Student::getGPA(){
    return GPA;
}

void Student::input(){
    Person::input();
    Date::input();
    do{
        cout << "Nhap ma SV:"; cin >> studentID;
        if(studentID.length() > 15){
            cout << "Khong duoc qua 15 ky tu, hay nhap lai."
                 << endl;
        }
    }while(studentID.length() > 15);

    do{
        cout << "Nhap diem GPA:"; cin >> GPA;
        if(GPA < 0 || GPA > 10){
            cout << "GPA khong hop le, hay nhap lai." << endl;
        }
    }while(GPA < 0 || GPA > 10);
}

string Student::letterGrade(){
    if(GPA >= 8.5) return "A";
    if(GPA >= 7.0) return "B";
    if(GPA >= 5.5) return "C";
    if(GPA >= 4.0) return "D";
    return "F";
}

class StudentManagement: public Student{
private:
    string className;
public:
    StudentManagement(string, string, string, int, int,
    int, string, float);
    void input();
    void display();
    string getClassName();
    string toString();
    friend bool operator>(StudentManagement &s1,
    StudentManagement &s2);
};

StudentManagement::StudentManagement(string c="",
string n="", string p="", int d=1, int m=1,
int y=2000, string id="", float gpa=0.0):
Student(n, p, d, m, y, id, gpa){
    className = c;
}

void StudentManagement::input(){
    do{
        cout << "\nNhap lop:"; cin >> ws;
        getline(cin, className);
        if(className.length() > 8){
            cout << "Khong qua 8 ky tu, hay nhap lai." << endl;
        }
    }while(className.length() > 8);
    Student::input();
}

void StudentManagement::display(){
    cout << left << fixed << setprecision(2);
    cout << "|" << setw(8) << className;
    cout << "|" << setw(24) << getName();
    cout << "|" << setw(16) << getStudentID();
    cout << "|" << ((getDay() > 9) ? "" : "0") << getDay();
    cout << "/" << ((getMonth() > 9) ? "" : "0")
         << getMonth();
    cout << "/" << setw(5) << getYear();
    cout << "|" << setw(12) << getPhone();
    cout << "|" << right << setw(8) << getGPA();
    cout << " | " << left << setw(8) << letterGrade();
    cout << "|" << endl;
}

string StudentManagement::toString(){
    stringstream ss;
    ss << className << "|" << getName() << "|"
       << getStudentID() << "|" << getDay() << "|"
       << getMonth() << "|" << getYear() << "|"
       << getPhone() << "|" << getGPA();
    return ss.str();
}

string StudentManagement::getClassName(){
    return className;
}

bool operator > (StudentManagement &s1,
StudentManagement &s2){
    return s1.getGPA() > s2.getGPA();
}




void readStudentsFromFile(const char* filename,
StudentManagement students[], int &count) {
    ifstream file(filename);
    if (!file) {
        cout << "Khong the mo tep:" << filename << endl;
        cout << "Khoi tao du lieu mau..." << endl;
        students[0] = StudentManagement("62K1",
        "Nguyen Van Phong", "0912454749", 18, 11, 2004,
        "225748020110333", 7.8);
        count = 1;
        return;
    }
    count = 0;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string className, name, phone, studentID;
        int day, month, year;
        float GPA;
        getline(ss, className, '|');
        getline(ss, name, '|');
        getline(ss, studentID, '|');
        ss >> day; ss.ignore();
        ss >> month; ss.ignore();
        ss >> year; ss.ignore();
        getline(ss, phone, '|');
        ss >> GPA;
        students[count] = StudentManagement(className, name,
        phone, day, month, year, studentID, GPA);
        count++;
    }
    file.close();
}

void saveStudentsToFile(const char* filename,
StudentManagement students[], int count) {
    ofstream file(filename);
    if (!file) {
        cout << "Khong the mo tep" << filename << endl;
        return;
    }
    for (int i = 0; i < count; i++) {
        file << students[i].toString() << endl;
    }
    file.close();
}

void addStudent(StudentManagement students[], int &count,
int max_size) {
    if (count >= max_size) {
        cout << "Qua so luong SV cho phep!" << endl;
        return;
    }
    StudentManagement newStudent;
    newStudent.input();
    students[count] = newStudent;
    count++;
}

void updateStudent(StudentManagement students[], int &count) {
    string studentID;
    cout << "\nNhap ma sinh vien can sua:"; cin >> ws;
    getline(cin, studentID);
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (students[i].getStudentID() == studentID) {
            cout << "Thong tin SV hien tai:" << endl;
            students[i].display();
            cout << "Nhap thong tin SV moi:" << endl;
            students[i].input();
            found = true;
            cout << "Cap nhat thong tin thanh cong." << endl;
            break;
        }
    }
    if (!found) {
        cout << "Khong tim thay SV:" << studentID << endl;
    }
    system("pause");
}

void tableLine() {
    cout << "+---------+-------------------------+"
         << "-----------------+------------+"
         << "-------------+----------+----------+" << endl;
}

void tableHeader(){
    tableLine();
    cout<<"| LOP     | HOVATEN                 |"
        <<" MASINHVIEN     | NGAYSINH   |"
        <<" DIENTHOAI   |DIEMGPA |DIEMCHU|"
        <<endl;
    tableLine();
}





void displayStudents(StudentManagement students[], int count) {
    tableHeader();
    for (int i = 0; i < count; i++) {
        students[i].display();
    }
    tableLine();
    system("pause");
}

void displayStudentsInClass(StudentManagement students[], int count) {
    string className;
    cout << "\nNhap lop:"; cin >> ws;
    getline(cin, className);
    int numStudent = 0;
    tableHeader();
    for (int i = 0; i < count; i++) {
        if (students[i].getClassName() == className) {
            students[i].display();
            numStudent++;
        }
    }
    tableLine();
    if (numStudent == 0) {
        cout << "Khong co sinh vien nao trong lop " << className << endl;
    }
    system("pause");
}

void sortStudentsByGPA(StudentManagement students[], int count) {
    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (students[j] > students[i]) {
                StudentManagement st = students[i];
                students[i] = students[j];
                students[j] = st;
            }
}

void sortStudentsByName(StudentManagement students[], int count) {
    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (students[i].getNameSort() > students[j].getNameSort()) {
                StudentManagement st = students[i];
                students[i] = students[j];
                students[j] = st;
            }
}

// Hàm xóa sinh viên theo mã sinh viên
void deleteStudentByID(StudentManagement students[], int &count) {
    string studentID;
    cout << "\nNhap ma sinh vien can xoa:"; cin >> ws;
    getline(cin, studentID);
    int index = -1;
    // Tìm ch? s? c?a sinh viên c?n xóa
    for (int i = 0; i < count; i++) {
        if (students[i].getStudentID() == studentID) {
            index = i;
            break;
        }
    }
    // N?u không tìm th?y sinh viên, thoát hàm
    if (index == -1) {
        cout << "Khong tim thay sinh vien:" << studentID << endl;
        system("pause");
        return;
    }
    // D?ch chuy?n các ph?n t? phía sau lên tru?c m?t v? trí
    for (int i = index; i < count - 1; i++) {
        students[i] = students[i + 1];
    }
    count--;
    cout << "\nDa xoa sinh vien:" << studentID << endl;
    system("pause");
}

// Hàm hi?n th? các ch?c nang trong h? th?ng
void showMenu() {
    cout << "\033[2J\033[H"; // Xóa màn hình
    cout << "+------------HETHONG------------+\n";
    cout << "| QUAN LY THONG TIN SINH VIEN |\n";
    cout << "+----------------------------------+\n";
    cout << "|1. Them sinh vien moi             |\n";
    cout << "|2. Hien thi tat ca sinh vien       |\n";
    cout << "|3. Hien thi danh sach theo lop     |\n";
    cout << "|4. Sap xep danh sach theo GPA      |\n";
    cout << "|5. Sap xep danh sach theo ten      |\n";
    cout << "|6. Sua thong tin sinh vien         |\n";
    cout << "|7. Xoa sinh vien theo ma           |\n";
    cout << "|8. Thoat chuong trinh              |\n";
    cout << "+----------------------------------+\n";
    cout << "Chon chuc nang:";
}

// Chuong trình chính
int main() {
    const int MAX_SIZE = 100;
    StudentManagement students[MAX_SIZE];
    int count = 0;
    readStudentsFromFile("students.txt", students, count);
    int choice;
    // Hi?n th? danh sách các ch?c nang cho ngu?i dùng ch?n
    do {
        showMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                addStudent(students, count, MAX_SIZE);
                break;
            case 2:
                displayStudents(students, count);
                break;
            case 3:
                displayStudentsInClass(students, count);
                break;
            case 4:
                sortStudentsByGPA(students, count);
                displayStudents(students, count);
                break;
            case 5:
                sortStudentsByName(students, count);
                displayStudents(students, count);
                break;
            case 6:
                updateStudent(students, count);
                break;
            case 7:
                deleteStudentByID(students, count);
                break;
            case 8:
                saveStudentsToFile("students.txt", students, count);
                break;
            default:
                cout << "Chon sai chuc nang!" << endl;
                system("pause");
                break;
        }
    } while (choice != 8);
    // Ch?n 8 d? luu d? li?u vào t?p và thoát kh?i chuong trình
    return 0;
}

