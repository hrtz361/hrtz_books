#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<cstdlib>
using namespace std;


class Person {
    private:
        string fullName, gender, birthYear, phoneNumber;
    public:
        Person();
        void Input();
        void Display();
        
        virtual void readFromFile(ifstream& f) {
            getline(f, fullName,    ',');
            getline(f, gender,      ',');
            getline(f, birthYear,   ',');
            getline(f, phoneNumber, ',');
        }
        string getFullName()    const { return fullName;    }
        string getGender()      const { return gender;      }
        string getBirthYear()   const { return birthYear;   }
        string getPhoneNumber() const { return phoneNumber; }
};

Person::Person() {
    fullName    = "Nguyen Van A";
    gender      = "Nam";
    birthYear   = "01/01/2000";
    phoneNumber = "0123456789";
}
void Person::Input() {
    cout << "Nhap ho ten: ";            cin >> ws; getline(cin, fullName);
    cout << "Nhap gioi tinh: ";         cin >> ws; getline(cin, gender);
    cout << "Nhap ngay thang nam sinh: "; cin >> ws; getline(cin, birthYear);
    cout << "Nhap so dien thoai: ";     cin >> ws; getline(cin, phoneNumber);
}
void Person::Display() {
    cout << "Ho ten: "  << fullName
         << "\nGioi tinh: " << gender
         << "\nNam sinh: "  << birthYear
         << "\nSDT: "       << phoneNumber;
}


class Reader : public Person {
    private:
        string readerID, registrationDate, expirationDate;
        int    maxBooks;
        bool   Block;
    public:
        Reader();
        void Input();
        void Display();
        string getReaderID()         { return readerID;          }
        
        string getRegistrationDate() { return registrationDate;  }
        string getExpirationDate()   { return expirationDate;    }
        int    getMaxBooks()         { return maxBooks;           }
        bool   getStatus()           { return Block;             }
        void renewcard(string newExp){ expirationDate = newExp;  }
        void blockCard()             { Block = true;             }
        void unblockCard()           { Block = false;            }
        
        void readFromFile(ifstream& f) override {
            Person::readFromFile(f);
            getline(f, readerID,          ',');
            getline(f, registrationDate,  ',');
            getline(f, expirationDate,    ',');
            string maxStr, blockStr;
            getline(f, maxStr,  ',');
            getline(f, blockStr);
            if (!maxStr.empty())   maxBooks = stoi(maxStr);
            Block = (blockStr == "1");
        }
};

Reader::Reader() {
    readerID = registrationDate = expirationDate = "";
    maxBooks = 0;
    Block    = false;
}
void Reader::Input() {
    Person::Input();
    cout << "Nhap ma nguoi doc: ";               cin >> ws; getline(cin, readerID);
    cout << "Nhap ngay dang ki (dd/mm/yyyy): ";  cin >> ws; getline(cin, registrationDate);
    cout << "Nhap ngay het han (dd/mm/yyyy): ";  cin >> ws; getline(cin, expirationDate);
    cout << "Nhap so sach toi da duoc muon: ";   cin >> maxBooks;
    Block = false;
}
void Reader::Display() {
    Person::Display();
    cout << "\nMa nguoi doc: "    << readerID
         << "\nNgay dang ki: "    << registrationDate
         << "\nNgay het han: "    << expirationDate
         << "\nSo sach duoc muon: " << maxBooks
         
         << "\nTrang thai the: "  << (Block ? "Bi khoa" : "Hoat dong");
}


class Book {
    protected:
        string bookID, title, author, publisher;
        double price;
        int    borrowCount;
    public:
        Book();
        virtual void Input();
        virtual void Output();
        virtual double calculateRentalFee() = 0;
        string getBookID()  { return bookID;       }
        string getTitle()   { return title;        }
        string getAuthor()  { return author;       }
        double getPrice()   { return price;        }
        int getBorrowCount(){ return borrowCount;  }
        void incrementBorrowCount() { borrowCount++; }
        virtual void writeToFile(ofstream& f) {
            f << bookID << "," << title << "," << author << ","
              << publisher << "," << price << ",";
        }
        
        virtual void readFromFile(ifstream& f) {
            getline(f, bookID,     ',');
            getline(f, title,      ',');
            getline(f, author,     ',');
            getline(f, publisher,  ',');
            string priceStr;
            getline(f, priceStr,   ',');
            if (!priceStr.empty()) price = stod(priceStr);
        }
        virtual ~Book() {}
};

Book::Book() {
    bookID = title = author = publisher = "";
    price = 0;
    borrowCount = 0;
}
void Book::Input() {
    cout << "Nhap ma sach: ";         cin >> ws; getline(cin, bookID);
    cout << "Nhap tieu de: ";         cin >> ws; getline(cin, title);
    cout << "Nhap ten tac gia: ";     cin >> ws; getline(cin, author);
    cout << "Nhap ten nha xuat ban: ";cin >> ws; getline(cin, publisher);
    cout << "Nhap gia sach: ";        cin >> price;
}
void Book::Output() {
    cout << "\n Ma sach: "      << bookID
         << "\n Tieu de: "      << title
         << "\n Tac gia: "      << author
         << "\n Nha xuat ban: " << publisher
         << "\n Gia sach: "     << price;
}


class PrintedBook : public Book {
    private:
        int    pageCount;
        string weight, status, shelfLocation;
    public:
        PrintedBook();
        void Input();
        void Output();
        double calculateRentalFee() { return price * 0.03; }
       
        void writeToFile(ofstream& f) override {
            f << 1 << ",";
            Book::writeToFile(f);
            f << pageCount << "," << weight << "," << shelfLocation << "," << status << "\n";
        }
        
        void readFromFile(ifstream& f) override {
            Book::readFromFile(f);
            string pageStr;
            getline(f, pageStr, ',');
            if (!pageStr.empty()) pageCount = stoi(pageStr);
            getline(f, weight,       ',');
            getline(f, shelfLocation,',');
            getline(f, status);
        }
};

PrintedBook::PrintedBook() {
    pageCount = 0;
    weight = shelfLocation = status = "";
}
void PrintedBook::Input() {
    Book::Input();
    cout << "Nhap so trang sach: ";          cin >> pageCount;
    cout << "Nhap trong luong sach (kg): ";  cin >> ws; getline(cin, weight);
    cout << "Nhap vi tri ke: ";              cin >> ws; getline(cin, shelfLocation);
    cout << "Nhap trang thai (con/het): ";   cin >> ws; getline(cin, status);
}
void PrintedBook::Output() {
    Book::Output();
    cout << "\n So trang: "      << pageCount
         << "\n Trong luong: "   << weight    << " kg"
         << "\n Vi tri ke: "     << shelfLocation
         << "\n Trang thai: "    << status
         << "\n Phi muon sach: " << calculateRentalFee();
}


class Ebook : public Book {
    private:
        string fileSize, fileFormat, downloadLink;
    public:
        Ebook();
        void Input();
        void Output();
        double calculateRentalFee() { return price * 0.02; }
        void writeToFile(ofstream& f) override {
            f << 2 << ",";
            Book::writeToFile(f);
            f << fileSize << "," << fileFormat << "," << downloadLink << "\n";
        }
       
        void readFromFile(ifstream& f) override {
            Book::readFromFile(f);
            getline(f, fileSize,     ',');
            getline(f, fileFormat,   ',');
            getline(f, downloadLink);
        }
};

Ebook::Ebook() {
    fileSize = fileFormat = downloadLink = "";
}
void Ebook::Input() {
    Book::Input();
    cout << "Nhap dung luong file (MB): ";    cin >> ws; getline(cin, fileSize);
    cout << "Nhap dinh dang file (PDF/EPUB): "; cin >> ws; getline(cin, fileFormat);
    cout << "Nhap Link tai ve: ";             cin >> ws; getline(cin, downloadLink);
}
void Ebook::Output() {
    Book::Output();
    cout << "\n Dung luong file: " << fileSize    << " MB"
         << "\n Dinh dang file: "  << fileFormat
         << "\n Link tai ve: "     << downloadLink
         << "\n Phi muon sach: "   << calculateRentalFee();
}


class BorrowTicket {
private:
    string ticketID, readerID, bookID, borrowDateStr;
    int dueDate;    // Luu du?i d?ng t?ng s? ngày
    int returnDate; // 0 = chua tr?

    bool isLeap(int y) const {
        return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
    }
    int daymonth(int m, int y) const {
        int d[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        if (m == 2 && isLeap(y)) return 29;
        return d[m - 1];
    }
    int totalday(int d, int m, int y) const {
        int total = 0;
        for (int year = 1; year < y; ++year)
            total += isLeap(year) ? 366 : 365;
        for (int month = 1; month < m; ++month)
            total += daymonth(month, y);
        total += d;
        return total;
    }

public:
    BorrowTicket() {
        ticketID = readerID = bookID = borrowDateStr = "";
        dueDate = returnDate = 0;
    }

    void Input() {
        int d, m, y; char c;
        cout << "Nhap ma ve: ";          cin >> ws; getline(cin, ticketID);
        cout << "Nhap ma nguoi doc: ";   cin >> ws; getline(cin, readerID);
        cout << "Nhap ma sach: ";        cin >> ws; getline(cin, bookID);
        cout << "Nhap ngay muon (dd/mm/yyyy): ";
        cin >> ws; getline(cin, borrowDateStr);
        cout << "Nhap ngay den han tra (dd/mm/yyyy): ";
        cin >> d >> c >> m >> c >> y;
        dueDate = totalday(d, m, y);
        returnDate = 0; // M?i t?o phi?u => chua tr?
    }

    void Output() const {
        cout << "Ma ve: "       << ticketID
             << "\nMa nguoi doc: " << readerID
             << "\nMa sach: "      << bookID
             << "\nNgay muon: "    << borrowDateStr;
        if (returnDate == 0) cout << "\nTrang thai: CHUA TRA\n";
        else {
            int late = returnDate - dueDate;
            if (late <= 0) cout << "\nTrang thai: DA TRA - DUNG HAN/SOM\n";
            else cout << "\nTrang thai: DA TRA - TRE " << late << " NGAY"
                      << "\nTien phat: " << calculateFine() << " VND\n";
        }
    }

    long calculateFine(int fine = 2000) const {
        if (returnDate == 0) return 0;
        int lateDay = returnDate - dueDate;
        if (lateDay <= 0) return 0;
        return 1LL * lateDay * fine;
    }

    string getTicketID()  { return ticketID;    }
    string getReaderID()  { return readerID;    }
    string getBookID()    { return bookID;       }
    int    getReturnDate(){ return returnDate;  }
    
    int    getDueDate()   { return dueDate;     }

    void updateReturnDate(int d, int m, int y) {
        returnDate = (d == 0) ? 0 : totalday(d, m, y);
    }
    void markAsResolved() {
        if (returnDate == 0) returnDate = dueDate;
    }

    void writeToFile(ofstream& f) {
        f << ticketID << "," << readerID << "," << bookID << ","
          << borrowDateStr << "," << dueDate << "," << returnDate << "\n";
    }
    void readFromFile(ifstream& f) {
        getline(f, ticketID,       ',');
        getline(f, readerID,       ',');
        getline(f, bookID,         ',');
        getline(f, borrowDateStr,  ',');
        string tempDue, tempReturn;
        getline(f, tempDue,   ',');
        getline(f, tempReturn);
        if (!tempDue.empty())    dueDate    = stoi(tempDue);
        if (!tempReturn.empty()) returnDate = stoi(tempReturn);
    }
};


class QuanLySach {
    private:
        vector<Book*> danhSachSach;
    public:
        ~QuanLySach() {
            for (int i = 0; i < (int)danhSachSach.size(); i++)
                delete danhSachSach[i];
        }

        void HienThiDanhSachTheoLuotMuon() {
            bool found = false;
            for (int i = 0; i < (int)danhSachSach.size(); i++) {
                if (danhSachSach[i]->getBorrowCount() > 0) {
                    danhSachSach[i]->Output();
                    cout << "\nLuot muon: " << danhSachSach[i]->getBorrowCount() << "\n";
                    found = true;
                }
            }
            if (!found) cout << "Chua co sach nao duoc muon!\n";
        }

        void ThemSach() {
            int loai;
            cout << "--- CHON LOAI SACH ---\n1. Sach in\n2. Sach dien tu\nChon: ";
            cin >> loai;
            Book* sachMoi = NULL;
            if      (loai == 1) sachMoi = new PrintedBook();
            else if (loai == 2) sachMoi = new Ebook();
            else { cout << "Lua chon khong hop le!\n"; return; }
            sachMoi->Input();
            danhSachSach.push_back(sachMoi);
            cout << "-> Them sach thanh cong!\n";
        }

        void HienThiDanhSach() {
            if (danhSachSach.empty()) { cout << "Danh sach sach dang trong!\n"; return; }
            for (int i = 0; i < (int)danhSachSach.size(); i++) {
                cout << "\n--- Sach thu " << i+1 << " ---";
                danhSachSach[i]->Output();
                cout << "\n";
            }
        }

        void TimKiemSach() {
            if (danhSachSach.empty()) { cout << "Danh sach sach trong!\n"; return; }
            int tieuChi;
            cout << "Tim kiem theo: 1. Ma sach | 2. Ten sach | 3. Tac gia: ";
            cin >> tieuChi;
            string tuKhoa;
            cout << "Nhap tu khoa: "; cin >> ws; getline(cin, tuKhoa);
            bool found = false;
            for (int i = 0; i < (int)danhSachSach.size(); i++) {
                if ((tieuChi == 1 && danhSachSach[i]->getBookID() == tuKhoa) ||
                    (tieuChi == 2 && danhSachSach[i]->getTitle()  == tuKhoa) ||
                    (tieuChi == 3 && danhSachSach[i]->getAuthor() == tuKhoa)) {
                    cout << "\n--- Tim thay ---";
                    danhSachSach[i]->Output();
                    cout << "\n";
                    found = true;
                }
            }
            if (!found) cout << "Khong tim thay sach phu hop!\n";
        }

        void XoaSach() {
            string maSach;
            cout << "Nhap ma sach can xoa: "; cin >> ws; getline(cin, maSach);
            for (int i = 0; i < (int)danhSachSach.size(); i++) {
                if (danhSachSach[i]->getBookID() == maSach) {
                    delete danhSachSach[i];
                    danhSachSach.erase(danhSachSach.begin() + i);
                    cout << "-> Da xoa sach thanh cong!\n";
                    return;
                }
            }
            cout << "Khong tim thay ma sach can xoa!\n";
        }

        void SuaSach() {
            string maSach;
            cout << "Nhap ma sach can sua: "; cin >> ws; getline(cin, maSach);
            for (int i = 0; i < (int)danhSachSach.size(); i++) {
                if (danhSachSach[i]->getBookID() == maSach) {
                    cout << "Nhap lai thong tin moi:\n";
                    danhSachSach[i]->Input();
                    cout << "-> Cap nhat thanh cong!\n";
                    return;
                }
            }
            cout << "Khong tim thay ma sach can sua!\n";
        }

        Book* TimSachTheoMa(string maSach) {
            for (int i = 0; i < (int)danhSachSach.size(); i++)
                if (danhSachSach[i]->getBookID() == maSach) return danhSachSach[i];
            return NULL;
        }

        void saveToFile() {
            ofstream f("sach.txt");
            for (auto s : danhSachSach) s->writeToFile(f);
            f.close();
        }

        
        void loadFromFile() {
            ifstream f("sach.txt");
            if (!f.is_open()) return;
            danhSachSach.clear();
            int loai;
            while (f >> loai) {
                f.ignore(); // b? d?u ','
                Book* s = (loai == 1) ? (Book*)new PrintedBook() : (Book*)new Ebook();
                s->readFromFile(f);
                danhSachSach.push_back(s);
            }
            f.close();
        }

        
        void MenuQuanLySach() {
            int sub;
            do {
                cout << "\n--- QUAN LY SACH ---\n"
                     << "1. Them sach\n2. Hien thi danh sach\n"
                     << "3. Tim kiem\n4. Xoa sach\n5. Sua sach\n0. Quay lai\nChon: ";
                cin >> sub;
                if      (sub == 1) ThemSach();
                else if (sub == 2) HienThiDanhSach();
                else if (sub == 3) TimKiemSach();
                else if (sub == 4) XoaSach();
                else if (sub == 5) SuaSach();
            } while (sub != 0);
        }
};


class QuanLyDocGia {
    private:
        vector<Reader> danhSachDocGia;
    public:
        void CapTheMoi() {
            Reader dgMoi;
            cout << "\n--- NHAP THONG TIN DOC GIA MOI ---\n";
            dgMoi.Input();
            danhSachDocGia.push_back(dgMoi);
            cout << "-> Cap the moi thanh cong!\n";
        }

        void HienThiDanhSach() {
            if (danhSachDocGia.empty()) { cout << "Danh sach doc gia dang trong!\n"; return; }
            for (int i = 0; i < (int)danhSachDocGia.size(); i++) {
                cout << "\n--- Doc gia thu " << i+1 << " ---\n";
                danhSachDocGia[i].Display();
                cout << "\n";
            }
        }

        void GiaHanThe() {
            string maDG, ngayMoi;
            cout << "Nhap ma doc gia can gia han: "; cin >> ws; getline(cin, maDG);
            for (int i = 0; i < (int)danhSachDocGia.size(); i++) {
                if (danhSachDocGia[i].getReaderID() == maDG) {
                    cout << "Nhap ngay het han moi (dd/mm/yyyy): "; cin >> ws; getline(cin, ngayMoi);
                    danhSachDocGia[i].renewcard(ngayMoi);
                    cout << "-> Gia han the thanh cong!\n";
                    return;
                }
            }
            cout << "Khong tim thay ma doc gia!\n";
        }

        void ChanThe() {
            string maDG;
            cout << "Nhap ma doc gia can chan the: "; cin >> ws; getline(cin, maDG);
            for (int i = 0; i < (int)danhSachDocGia.size(); i++) {
                if (danhSachDocGia[i].getReaderID() == maDG) {
                    if (danhSachDocGia[i].getStatus()) {
                        cout << "The da bi chan tu truoc!\n";
                    } else {
                        danhSachDocGia[i].blockCard();
                        cout << "-> Da chan the " << maDG << " thanh cong!\n";
                    }
                    return;
                }
            }
            cout << "Khong tim thay ma doc gia!\n";
        }

        Reader* TimDocGiaTheoMa(string maDG) {
            for (int i = 0; i < (int)danhSachDocGia.size(); i++)
                if (danhSachDocGia[i].getReaderID() == maDG) return &danhSachDocGia[i];
            return NULL;
        }

       
        void saveToFile() {
            ofstream f("docgia.txt");
            for (auto& dg : danhSachDocGia) {
                f << dg.getFullName()         << ","
                  << dg.getGender()           << ","
                  << dg.getBirthYear()        << ","
                  << dg.getPhoneNumber()      << ","
                  << dg.getReaderID()         << ","
                  << dg.getRegistrationDate() << ","
                  << dg.getExpirationDate()   << ","
                  << dg.getMaxBooks()         << ","
                  << dg.getStatus()           << "\n";
            }
            f.close();
        }

       
        void loadFromFile() {
            ifstream f("docgia.txt");
            if (!f.is_open()) return;
            danhSachDocGia.clear();
            while (f.peek() != EOF && f.peek() != -1) {
                Reader dg;
                dg.readFromFile(f);
                if (!dg.getReaderID().empty())
                    danhSachDocGia.push_back(dg);
            }
            f.close();
        }

        
        void MenuQuanLyDocGia() {
            int sub;
            do {
                cout << "\n--- QUAN LY DOC GIA ---\n"
                     << "1. Cap the moi\n2. Hien thi danh sach\n"
                     << "3. Gia han the\n4. Chan the\n0. Quay lai\nChon: ";
                cin >> sub;
                if      (sub == 1) CapTheMoi();
                else if (sub == 2) HienThiDanhSach();
                else if (sub == 3) GiaHanThe();
                else if (sub == 4) ChanThe();
            } while (sub != 0);
        }
};


     class QuanLyMuonTra {
   private:
    vector<BorrowTicket> danhSachPhieu;
   public:
    void saveToFile() {
        ofstream f("phieumuon.txt");
        for (int i = 0; i < (int)danhSachPhieu.size(); i++)
            danhSachPhieu[i].writeToFile(f);
        f.close();
    }

    void loadFromFile() {
        ifstream f("phieumuon.txt");
        if (!f.is_open()) return;
        danhSachPhieu.clear();
        while (f.peek() != EOF && f.peek() != -1) {
            BorrowTicket phieu;
            phieu.readFromFile(f);
            if (!phieu.getTicketID().empty())
                danhSachPhieu.push_back(phieu);
        }
        f.close();
    }

    vector<BorrowTicket>& getDanhSachPhieu() { return danhSachPhieu; }

    void LapPhieuMuon(QuanLyDocGia& qlDG, QuanLySach& qlSach) {
        BorrowTicket phieuMoi;
        cout << "\n--- LAP PHIEU MUON SACH ---\n";
        phieuMoi.Input();

        Reader* dg = qlDG.TimDocGiaTheoMa(phieuMoi.getReaderID());
        if (!dg) { cout << "-> Loi: Khong tim thay ma doc gia!\n"; return; }
        if (dg->getStatus()) { cout << "-> Loi: The doc gia dang bi khoa!\n"; return; }

        int soSachDangMuon = 0;
        for (int i = 0; i < (int)danhSachPhieu.size(); i++)
            if (danhSachPhieu[i].getReaderID() == phieuMoi.getReaderID()
                && danhSachPhieu[i].getReturnDate() == 0)
                soSachDangMuon++;

        if (soSachDangMuon >= dg->getMaxBooks()) {
            cout << "-> Loi: Da dat gioi han " << dg->getMaxBooks() << " sach!\n"; return;
        }

        Book* sach = qlSach.TimSachTheoMa(phieuMoi.getBookID());
        if (!sach) { cout << "-> Loi: Khong tim thay ma sach!\n"; return; }

       
        sach->incrementBorrowCount();
        danhSachPhieu.push_back(phieuMoi);
        cout << "-> Lap phieu muon thanh cong!\n";
    }

    void CapNhatNgayTra() {
        string maPhieu;
        cout << "Nhap ma phieu muon can tra sach: "; cin >> ws; getline(cin, maPhieu);
        for (int i = 0; i < (int)danhSachPhieu.size(); i++) {
            if (danhSachPhieu[i].getTicketID() == maPhieu) {
                if (danhSachPhieu[i].getReturnDate() != 0) {
                    cout << "-> Phieu nay da duoc tra sach truoc do!\n"; return;
                }
                int d, m, y; char c;
                cout << "Nhap ngay tra thuc te (dd/mm/yyyy): ";
                cin >> d >> c >> m >> c >> y;
                danhSachPhieu[i].updateReturnDate(d, m, y);
                cout << "-> Cap nhat ngay tra thanh cong!\n";
                danhSachPhieu[i].Output();
                return;
            }
        }
        cout << "-> Khong tim thay ma phieu!\n";
    }

    void XuLyMatSach(QuanLySach& qlSach) {
        string maPhieu;
        cout << "Nhap ma phieu cua sach bi mat: "; cin >> ws; getline(cin, maPhieu);
        for (int i = 0; i < (int)danhSachPhieu.size(); i++) {
            if (danhSachPhieu[i].getTicketID() == maPhieu) {
                Book* sach = qlSach.TimSachTheoMa(danhSachPhieu[i].getBookID());
                if (sach) {
                    double tienPhat = sach->getPrice() * 2;
                    cout << "\n--- XU LY MAT SACH ---"
                         << "\nTen sach: "       << sach->getTitle()
                         << "\nGia tri sach: "   << sach->getPrice()  << " VND"
                         << "\nTien den bu (x2): " << tienPhat << " VND\n";
                    danhSachPhieu[i].markAsResolved();
                    cout << "-> Da xu ly xong!\n";
                }
                return;
            }
        }
        cout << "-> Khong tim thay ma phieu!\n";
    }

    void HienThiDanhSach() {
        if (danhSachPhieu.empty()) { cout << "Danh sach phieu muon trong!\n"; return; }
        for (int i = 0; i < (int)danhSachPhieu.size(); i++) {
            cout << "\n--- Phieu thu " << i+1 << " ---\n";
            danhSachPhieu[i].Output();
        }
    }

   
    void MenuQuanLyMuonTra(QuanLyDocGia& qlDG, QuanLySach& qlSach) {
        int sub;
        do {
            cout << "\n--- QUAN LY MUON TRA ---\n"
                 << "1. Lap phieu muon\n2. Cap nhat ngay tra\n"
                 << "3. Xu ly mat sach\n4. Hien thi danh sach phieu\n0. Quay lai\nChon: ";
            cin >> sub;
            if      (sub == 1) LapPhieuMuon(qlDG, qlSach);
            else if (sub == 2) CapNhatNgayTra();
            else if (sub == 3) XuLyMatSach(qlSach);
            else if (sub == 4) HienThiDanhSach();
        } while (sub != 0);
    }
};


class ThongKeBaoCao {
public:
    void ThongKeSachMuonNhieu(QuanLySach& qlSach) {
        cout << "\n--- TOP SACH DUOC MUON NHIEU NHAT ---\n";
        qlSach.HienThiDanhSachTheoLuotMuon();
    }

    
    void DanhSachQuaHan(QuanLyMuonTra& qlMT, QuanLyDocGia& qlDG) {
        cout << "\n--- DOC GIA NO SACH QUA HAN ---\n";
        
        
        int ngayHienTai = 739354;
        bool coQuaHan = false;
        vector<BorrowTicket>& ds = qlMT.getDanhSachPhieu();
        for (int i = 0; i < (int)ds.size(); i++) {
            if (ds[i].getReturnDate() == 0 && ds[i].getDueDate() < ngayHienTai) {
                cout << "Ma phieu: "    << ds[i].getTicketID()
                     << " | Ma DG: "    << ds[i].getReaderID()
                     << " | Ma sach: "  << ds[i].getBookID()
                     << " | Qua han: "  << (ngayHienTai - ds[i].getDueDate()) << " ngay\n";
                coQuaHan = true;
            }
        }
        if (!coQuaHan) cout << "Khong co doc gia nao no sach qua han!\n";
    }

    
    void BaoCaoDoanhThu(QuanLyMuonTra& qlMT) {
        long tongTien = 0;
        vector<BorrowTicket>& ds = qlMT.getDanhSachPhieu();
        for (int i = 0; i < (int)ds.size(); i++)
            tongTien += ds[i].calculateFine();
        cout << "\n--- BAO CAO DOANH THU PHI PHAT ---"
             << "\nTong tien phat thu duoc: " << tongTien << " VND\n";
    }
};


int main() {
    QuanLySach   qlSach;
    QuanLyDocGia qlDG;
    QuanLyMuonTra qlMT;
    ThongKeBaoCao qlTK;

    qlSach.loadFromFile();
    qlDG.loadFromFile();
    qlMT.loadFromFile();

    int choice;
    do {
        cout<<"+------------HE THONG------------+\n";
        cout<<"|        QUAN LY THU VIEN       |\n";
        cout<<"+-------------------------------+\n";
        cout<<"|1.Quan ly sach                 |\n";
        cout<<"|2.Quan ly doc gia              |\n";
        cout<<"|3.Quan ly muon - tra           |\n";
        cout<<"|4.Thong ke & bao cao           |\n";
        cout<<"|0. Thoat & Luu du lieu         |\n";      
        cout<<"+-------------------------------+\n"; 
		cout<<"Nhap lua chon: ";
        cin >> choice;

        switch (choice) {
            // [FIX 8] G?i sub-menu thay vì d? tr?ng (tru?c ch? có comment)
            case 1: qlSach.MenuQuanLySach();               break;
            case 2: qlDG.MenuQuanLyDocGia();               break;
            case 3: qlMT.MenuQuanLyMuonTra(qlDG, qlSach); break;
            case 4: {
                int sub;
                cout << "\n1. Sach muon nhieu nhat\n2. Doc gia qua han\n3. Doanh thu phi phat\nChon: ";
                cin >> sub;
                if      (sub == 1) qlTK.ThongKeSachMuonNhieu(qlSach);
                else if (sub == 2) qlTK.DanhSachQuaHan(qlMT, qlDG);
                else if (sub == 3) qlTK.BaoCaoDoanhThu(qlMT);
                break;
            }
        }
    } while (choice != 0);

    qlSach.saveToFile();
    qlDG.saveToFile();
    qlMT.saveToFile();
    cout << "Da luu du lieu. Tam biet!\n";
    return 0;
}
