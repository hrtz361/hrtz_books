#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <regex> 
#include <limits> 
#include <set>
#include <map> 
#include <iomanip> 
#include <vector>

using namespace std;

///////////////////// CÁC HÀM TRỢ GIÚP //////////////////////// 

void registerBookID(string id) {
    set<string> uniqueIDs;
    string line;
    
    ifstream inFile("bookid.txt");
    while (getline(inFile, line)) {
        if (!line.empty()) uniqueIDs.insert(line);
    }
    inFile.close();

    uniqueIDs.insert(id);

    ofstream outFile("bookid.txt");
    for (const string& uid : uniqueIDs) {
        outFile << uid << endl;
    }
    outFile.close();
}

void search(string pattern) {
    string filename;
    for (int l = 0; l < 4; ++l) {
        if (l == 0) filename = "shelf1.txt";
        else if (l == 1) filename = "shelf2.txt";
        else if (l == 2) filename = "shelf3.txt";
        else filename = "ebooks.txt";

        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Không thể mở tệp: " << filename << endl;
            continue; 
        }

        try {
            regex re(pattern);
            string line;
            int line_number = 0;
            bool foundInFile = false;

            cout << "\n>>> ĐANG TÌM TRONG TỆP: " << filename << " <<<" << endl;

            while (getline(file, line)) {
                line_number++;
                if (regex_search(line, re)) {
                    cout << "[Dòng " << line_number << "]: " << line << endl;
                    foundInFile = true;
                }
            }
            
            if (!foundInFile) 
                cout << "(Không tìm thấy kết quả trong tệp này)" << endl;

        } catch (const regex_error& e) {
            cerr << "Lỗi định dạng tìm kiếm: " << e.what() << endl;
            return; 
        }
        file.close();
        cout << string(40, '-') << endl;
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseExecution() {
    cout << "\nNhấn phím bất kỳ để tiếp tục...";
    system("pause > nul"); 
}

// ==========================================
// 1. ĐỊNH NGHĨA LỚP (CLASS)
// ==========================================
class Person {
protected:
    string fullName, gender, phoneNumber;
    int birthYear;
public:
    virtual void input();
    virtual void display();
};

class Reader : public Person {
private:
    string readerID, registrationDate, dueDate;
    int readerCardRank;
public:
    void input() override;
    void display() override;
    string getFileData(); 
};

class Book {
protected:
    string id, title, author, publisher;
    float price;
    string currentFile;

public:
    virtual void input();
    virtual void display();   
    string belongBook();
    
    void setFileContext();    
    void displayFile();       
    void editORdelete(bool isDelete); 
    friend void search(string pattern); 
    virtual double calculateRentalFee(double price); 
};

class PrintedBook : public Book {
private:
    int pages, shelf, remain;
    float weight;
public:
    void input() override;
};

class Ebook : public Book {
private:
    float fileSize;
    string format, link;
public:
    void input() override;
};

class BorrowTicket: public Book , public Reader {
public:
    void inputData();
    void displayData();
    void updateData();
    void calculateOverdueFeature();
    void calculateLostFeature();
    void searchKeyword(string key);
    static void findMostBorrowedBook();
    double calculateRentalFee(double price) override; 
};

// ==========================================
// 2. CÀI ĐẶT HÀM THÀNH VIÊN
// ==========================================

double BorrowTicket::calculateRentalFee(double price){ return price * 0.04; };

void BorrowTicket::searchKeyword(string key) {
    ifstream inFile("borrow.txt");
    string line;
    bool found = false;
    regex pattern(key); 

    cout << "\n--- KẾT QUẢ TÌM KIẾM CHO: " << key << " ---\n";
    while (getline(inFile, line)) {
        if (regex_search(line, pattern)) {
            cout << line << endl;
            found = true;
        }
    }
    if (!found) cout << "Không tìm thấy dữ liệu phù hợp." << endl;
    inFile.close();
}

void BorrowTicket::updateData() {
    ifstream inFile("borrow.txt");
    vector<string> lines;
    string line;
    while (getline(inFile, line)) lines.push_back(line);
    inFile.close();

    displayData();
    int target;
    cout << "Nhập số dòng cần cập nhật: "; cin >> target;

    if (target > 0 && target <= (int)lines.size()) {
        lines.erase(lines.begin() + (target - 1));
        ofstream outFile("borrow.txt");
        for (const string& s : lines) outFile << s << endl;
        outFile.close();
        inputData();
    }
}

void BorrowTicket::inputData() {
    string tID, rID, bID, bDate, dDate, rDate;
    double price;
    int mB;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Mã Phiếu: "; getline(cin, tID);
    cout << "Mã Người Đọc: "; getline(cin, rID);
    cout << "Mã Sách: "; getline(cin, bID);

    registerBookID(bID);

    cout << "Giá Sách: "; cin >> price;
    double rentalFee = calculateRentalFee(price);
    cout << "Ngày Mượn (DD/MM/YYYY): "; cin >> bDate;
    cout << "Hạn Trả (DD/MM/YYYY): "; cin >> dDate;
    cout << "Đã Trả Sách Ngày (DD/MM/YYYY): "; cin >> rDate;
    cout << "Số Lượng Sách Tối Đa: "; cin >> mB;

    ofstream outFile("borrow.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "MAPHIEU: " << tID << " | MANGUOIDOC: " << rID << " | MASACH: " << bID 
                << " | GIA: " << price << " | PHIMUON: " << rentalFee 
                << " | NGAYMUON: " << bDate << " | HANTRA: " << dDate 
                << " | NGAYTRA: " << rDate << " | TOIDA: " << mB << endl;
        outFile.close();
        cout << "Đã thêm thẻ mượn thành công!\n";
    }
}

void BorrowTicket::calculateOverdueFeature() {
    ifstream inFile("borrow.txt");
    if (!inFile) return;
    vector<string> lines;
    string line;
    while (getline(inFile, line)) lines.push_back(line);
    inFile.close();

    displayData();
    int target; double price; int days;
    cout << "Chọn số dòng: "; cin >> target;
    cout << "Giá sách: "; cin >> price;
    cout << "Số ngày quá hạn: "; cin >> days;

    if (target > 0 && target <= (int)lines.size()) {
        double fee = days * price * 0.1 ; 
        lines[target - 1] = "PHIPHAT_QUAHAN: " + to_string(fee) + " | " + lines[target - 1];
        ofstream outFile("borrow.txt");
        for (const string& s : lines) outFile << s << endl;
        outFile.close();
        cout << "Đã cộng phí quá hạn!\n";
    }
}

void BorrowTicket::calculateLostFeature() {
    ifstream inFile("borrow.txt");
    if (!inFile) return;
    vector<string> lines;
    string line;
    while (getline(inFile, line)) lines.push_back(line);
    inFile.close();

    displayData();
    int target; double price;
    cout << "Chọn dòng báo MẤT SÁCH: "; cin >> target;
    cout << "Giá trị sách: "; cin >> price;

    if (target > 0 && target <= (int)lines.size()) {
        double fee = price * 2; 
        lines[target - 1] = "PHIPHAT_MATSACH: " + to_string(fee) + " | " + lines[target - 1];
        
        ofstream outFile("borrow.txt");
        for (const string& s : lines) outFile << s << endl;
        outFile.close();
        cout << "Đã thêm phí đền bù mất sách!\n";
    }
}

void BorrowTicket::findMostBorrowedBook() {
    ifstream idFile("bookid.txt");
    if (!idFile) { cout << "Không tìm thấy danh sách mã sách.\n"; return; }

    string id;
    string mostMatchedID = "Trống";
    int maxMatches = 0;

    cout << "\n--- TẦN SUẤT MƯỢN SÁCH ---\n";
    
    while (getline(idFile, id)) {
        if (id.empty()) continue;

        ifstream borrowFile("borrow.txt");
        string line;
        string target = "MASACH: " + id;
        int count = 0;

        while (getline(borrowFile, line)) {
            if (line.find(target) != string::npos) {
                count++;
            }
        }
        borrowFile.close();

        cout << "Mã Sách: " << id << " | Số lần mượn: " << count << endl;

        if (count > maxMatches) {
            maxMatches = count;
            mostMatchedID = id;
        }
    }
    idFile.close();

    cout << "-----------------------------\n";
    cout << "KẾT QUẢ: Sách có mã " << mostMatchedID << " được mượn nhiều nhất (" << maxMatches << " lần).\n";
}

void BorrowTicket::displayData() {
    ifstream inFile("borrow.txt");
    string line;
    int count = 1;
    while (getline(inFile, line)) cout << count++ << ". " << line << endl;
    inFile.close();
}

void Person::input() {
    cout << "Nhập Họ Tên: "; 
    getline(cin >> ws, fullName);
    cout << "Giới tính: "; 
    getline(cin >> ws, gender);
    cout << "Năm sinh: "; 
    cin >> birthYear; 
    cout << "Số điện thoại: "; 
    getline(cin >> ws, phoneNumber); 
}

void Person::display() {
    cout << "TÊN: " << fullName << " | GIỚI TÍNH: " << gender << " | NĂM SINH: " << birthYear;
}

void Reader::input() {
    Person::input();
    cout << "Mã Người Đọc: "; 
    getline(cin >> ws, readerID);
    cout << "Ngày đăng ký: "; 
    getline(cin >> ws, registrationDate);
    cout << "Ngày hết hạn: "; 
    getline(cin >> ws, dueDate);
    cout << "Hạng thẻ (1-3): "; 
    cin >> readerCardRank;
}

void Reader::display() {
    Person::display();
    int canRent = readerCardRank + 4;
    cout << " | ID: " << readerID << " | MƯỢN TỐI ĐA: " << canRent << " cuốn" << endl;
}

string Reader::getFileData() {
    int canRent = (readerCardRank == 1) ? 5 : (readerCardRank == 2) ? 6 : 7;
    return "TEN: " + fullName + " | GTINH: " + gender + " | NSINH: " + to_string(birthYear) + 
           " | ĐT: " + phoneNumber + " | ID: " + readerID + " | NGAY_DK:" + registrationDate + 
           " | NGAY_HH:" + dueDate + " | HANG:" + to_string(readerCardRank) +
           " | TOI_DA:" + to_string(canRent) ;
}

void Book::input() {
    cout << "Nhập Mã Sách: "; cin >> id;
    cin.ignore();
    cout << "Nhập Tên Sách: "; getline(cin, title);
    cout << "Nhập Tác Giả: "; getline(cin, author);
    cout << "Nhập Nhà Xuất Bản: "; getline(cin, publisher);
    cout << "Nhập Giá: "; cin >> price;
}

void Book::display() {
    cout << "ID: " << id << " | Tên: " << title << " | Tác giả: " << author;
}

string Book::belongBook() {
    return "ID: " + id + " | TEN: " + title + " | TACGIA: " + author + 
           " | NXB: " + publisher + " | GIA: " + to_string(price);
}

void Book::setFileContext() {
    int type;
    cout << "\nChọn Loại Sách:\n1. Sách In (Kệ 1-3)\n2. E-Books (Sách điện tử)\nLựa chọn: ";
    cin >> type;
    if (type == 1) {
        int s; 
        cout << "Nhập số kệ (1, 2, hoặc 3): "; cin >> s;
        currentFile = "shelf" + to_string(s) + ".txt";
    } else {
        currentFile = "ebooks.txt";
    }
}

void Book::displayFile() {
    ifstream inFile(currentFile);
    if (!inFile) { 
        cout << "\n[!] Lỗi: Tệp " << currentFile << " không tồn tại.\n"; 
        return; 
    }
    
    string line;
    int count = 1;
    cout << "\n--- DỮ LIỆU TRONG TỆP " << currentFile << " ---\n";
    while (getline(inFile, line)) {
        cout << count++ << ". " << line << endl;
    }
    inFile.close();
}

void Book::editORdelete(bool isDelete) {
    displayFile();
    int lineNum;
    cout << "\nNhập số dòng để " << (isDelete ? "XÓA" : "SỬA") << ": ";
    cin >> lineNum;

    ifstream inFile(currentFile);
    vector<string> lines;
    string line;
    int currentLine = 1;
    bool found = false;

    while (getline(inFile, line)) {
        if (currentLine == lineNum) {
            found = true;
            if (!isDelete) {
                cout << "\n[ ĐANG CHỈNH SỬA ]\n";
                if (currentFile == "ebooks.txt") { Ebook temp; temp.input(); }
                else { PrintedBook temp; temp.input(); }
            }
        } else {
            lines.push_back(line);
        }
        currentLine++;
    }
    inFile.close();

    if (found) {
        ofstream outFile(currentFile, ios::trunc);
        for (const string& l : lines) outFile << l << endl;
        cout << "\n[+] Cập nhật tệp thành công.\n";
    } else {
        cout << "\n[!] Lỗi: Không tìm thấy dòng yêu cầu.\n";
    }
}

double Book::calculateRentalFee(double value){
    return value * 0.04;
}

void PrintedBook::input() {
    Book::input();
    cout << "Nhập số trang: "; cin >> pages;
    cout << "Nhập trọng lượng: "; cin >> weight;
    cout << "Nhập số kệ (1-3): "; cin >> shelf;
    cout << "Nhập số lượng tồn kho: "; cin >> remain;

    string filename = "shelf" + to_string(shelf) + ".txt";
    ofstream outFile(filename, ios::app);
    outFile << belongBook() << " | TRANG: " << pages << " | CAN_NANG: " << weight << " | CON_LAI: " << remain << endl;
    cout << "\n[+] Đã lưu thông tin sách in.\n";
}

void Ebook::input() {
    Book::input();
    cout << "Nhập dung lượng (MB): "; cin >> fileSize;
    cout << "Nhập định dạng (pdf/epub): "; cin >> format;
    cin.ignore();
    cout << "Nhập link tải: "; getline(cin, link);

    ofstream outFile("ebooks.txt", ios::app);
    outFile << belongBook() << " | DL_SIZE: " << fileSize << "MB | DINH_DANG: " << format << " | LINK: " << link << endl;
    cout << "\n[+] Đã lưu thông tin E-Book.\n";
}

void saveToFile(Reader& r) {
    ofstream outFile("readers.txt", ios::app);
    if (outFile.is_open()) {
        outFile << r.getFileData() << endl; 
        outFile.close();
        cout << "Đã lưu hồ sơ người đọc.\n";
    }
}

void displayFromFile() {
    ifstream inFile("readers.txt");
    string line;
    cout << "\n--- DANH SÁCH NGƯỜI ĐỌC TRONG HỆ THỐNG ---\n";
    if (inFile.is_open()) {
        int lineNum = 1;
        while (getline(inFile, line)) {
            cout << lineNum << ". " << line << endl;
            lineNum++;
        }
        inFile.close();
    } else {
        cout << "Không thể mở danh sách người đọc.\n";
    }
}

void extendDueDate() {
    vector<string> lines;
    string line;
    int lineNum;

    ifstream inFile("readers.txt");
    while (getline(inFile, line)) lines.push_back(line);
    inFile.close();

    cout << "Nhập số dòng người đọc cần gia hạn: ";
    cin >> lineNum;

    if (lineNum > 0 && lineNum <= (int)lines.size()) {
        lines.erase(lines.begin() + (lineNum - 1));
        ofstream outFile("readers.txt");
        for (const auto& l : lines) outFile << l << endl;
        outFile.close();

        cout << "\n--- TIẾN HÀNH GIA HẠN (NHẬP LẠI THÔNG TIN MỚI) ---\n";
        Reader r;
        r.input();     
        saveToFile(r); 
    } else {
        cout << "Số dòng không hợp lệ!\n";
    }
}

void blockReader(int lineNum) {
    vector<string> lines;
    string line;
    ifstream inFile("readers.txt");
    while (getline(inFile, line)) lines.push_back(line);
    inFile.close();

    if (lineNum > 0 && lineNum <= (int)lines.size()) {
        lines[lineNum - 1] = "[DA_CHAN] | " + lines[lineNum - 1];
        ofstream outFile("readers.txt");
        for (const auto& l : lines) outFile << l << endl;
        outFile.close();
        cout << "Đã chặn người đọc tại dòng " << lineNum << ".\n";
    }
}

// ==========================================
// 3. CHƯƠNG TRÌNH CHÍNH
// ==========================================

void displayMenu() {
    clearScreen(); 
    cout << "==================================================================================" << endl;
    cout << "                         HE THONG QUAN LY THU VIEN" << endl;
    cout << "==================================================================================" << endl;

    int w = 32;
    cout << left 
         << setw(w) << "1. Them Sach In"         << setw(w) << "7. Them Nguoi Doc"       << setw(w) << "13. Tinh Phi Qua Han" << endl
         << setw(w) << "2. Them E-Book"          << setw(w) << "8. In DS Nguoi Doc"      << setw(w) << "14. Tinh Phi Mat Sach" << endl
         << setw(w) << "3. Sua Thong Tin Sach"   << setw(w) << "9. Gia Han The Doc"      << setw(w) << "15. Tim DS Qua Han" << endl
         << setw(w) << "4. Xoa Sach"             << setw(w) << "10. Chan Nguoi Doc"     << setw(w) << "16. Sach Muon Nhieu Nhat" << endl
         << setw(w) << "5. In Tat Ca Sach"       << setw(w) << "11. Them The Muon"       << endl
         << setw(w) << "6. Tim Kiem Sach"        << setw(w) << "12. Cap Nhat The Muon"   << setw(w) << "0. Thoat Chuong Trinh" << endl;

    cout << "==================================================================================" << endl;
    cout << "Chon chuc nang: ";
}

int main() {
    SetConsoleOutputCP(65001); // Hỗ trợ hiển thị tiếng Việt Unicode
    Book manager;
    Reader r; 
    BorrowTicket BT; 
    int choice;
    string pat; 

    do {
        displayMenu(); 
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        switch (choice) {
            case 1: { PrintedBook pb; pb.input(); pauseExecution(); break; }
            case 2: { Ebook eb; eb.input(); pauseExecution(); break; }
            case 3: manager.setFileContext(); manager.editORdelete(false); pauseExecution(); break;
            case 4: manager.setFileContext(); manager.editORdelete(true); pauseExecution(); break;
            case 5: manager.setFileContext(); manager.displayFile(); pauseExecution(); break;
            case 6: cout << "Nhap tu khoa tim kiem: "; cin >> pat; search(pat); pauseExecution(); break;  
            case 7: r.input(); saveToFile(r); pauseExecution(); break;
            case 8: displayFromFile(); pauseExecution(); break; 
            case 9: displayFromFile(); extendDueDate(); pauseExecution(); break;
            case 10: displayFromFile();{ int l; cout << "Nhap so dong nguoi doc can chan: "; cin >> l; blockReader(l); pauseExecution(); break; }
            case 11: BT.inputData(); pauseExecution(); break; 
            case 12: BT.updateData(); pauseExecution(); break;
            case 13: BT.calculateOverdueFeature(); pauseExecution(); break;
            case 14: BT.calculateLostFeature(); pauseExecution(); break;
            case 15: BT.searchKeyword("PHIPHAT_QUAHAN:"); pauseExecution(); break; 
            case 16: BorrowTicket::findMostBorrowedBook(); pauseExecution(); break; 
            case 0: cout << "\nDang thoat... Tam biet!\n"; break;
            default: cout << "\n[!] Lua chon khong hop le.\n"; pauseExecution();
        }
    } while (choice != 0);

    return 0;
}
