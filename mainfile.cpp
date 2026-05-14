#include <iostream>
#include <fstream>
#include <string>
#include <regex> 
#include <limits> 
#include <set>
#include <map> 
#include <iomanip> 
using namespace std;
///////////////////// others function //////////////////////// 


void registerBookID(string id) {
    set<string> uniqueIDs;
    string line;
    
    // 1. Read existing IDs to prevent duplicates
    ifstream inFile("bookid.txt");
    while (getline(inFile, line)) {
        if (!line.empty()) uniqueIDs.insert(line);
    }
    inFile.close();

    // 2. Add the new ID
    uniqueIDs.insert(id);

    // 3. Save back to file
    ofstream outFile("bookid.txt");
    for (const string& uid : uniqueIDs) {
        outFile << uid << endl;
    }
    outFile.close();
}



void search(string pattern) {
    string filename;
    
    for (int l = 0; l < 4; ++l) {
        // Assign filename based on loop index - No braces style
        if (l == 0) filename = "shelf1.txt";
        else if (l == 1) filename = "shelf2.txt";
        else if (l == 2) filename = "shelf3.txt";
        else filename = "ebooks.txt";

        ifstream file(filename);

        // Check if the file exists
        if (!file.is_open()) {
            cerr << "Could not open: " << filename << endl;
            continue; // Move to the next file instead of returning
        }

        try {
            regex re(pattern);
            string line;
            int line_number = 0;
            bool foundInFile = false;

            // Print the file header
            cout << "\n>>> SEARCHING FILE: " << filename << " <<<" << endl;

            while (getline(file, line)) {
                line_number++;
                if (regex_search(line, re)) {
                    cout << "[Line " << line_number << "]: " << line << endl;
                    foundInFile = true;
                }
            }
            
            if (!foundInFile) 
                cout << "(No matches found in this file)" << endl;

        } catch (const regex_error& e) {
            cerr << "Invalid regex pattern: " << e.what() << endl;
            return; // Stop if the pattern itself is broken
        }

        file.close();
        cout << string(40, '-') << endl;
    }
}


// Helper function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Helper function to pause for 5 seconds
void pauseExecution() {
    system("pause"); 
}


// ==========================================
// 1. CLASS PROTOTYPES
// ==========================================
class Person {
protected:
    string fullName, gender, phoneNumber ;
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
// 2. FUNCTION IMPLEMENTATIONS
// ==========================================
double BorrowTicket::calculateRentalFee(double price){ return price * 0.04; };

void BorrowTicket::searchKeyword(string key) {
    ifstream inFile("borrow.txt");
    string line;
    bool found = false;
    regex pattern(key); 

    cout << "\n--- SEARCH RESULTS FOR: " << key << " ---\n";
    while (getline(inFile, line)) {
        if (regex_search(line, pattern)) {
            cout << line << endl;
            found = true;
        }
    }
    if (!found) cout << "No matches found." << endl;
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
    cout << "Line to update: "; cin >> target;

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
    cout << "Ticket ID: "; getline(cin, tID);
    cout << "Reader ID: "; getline(cin, rID);
    cout << "Book ID: "; getline(cin, bID);
    //add unique ID to bookid.txt

    registerBookID(bID);

    cout << "Price: "; cin >> price;
    double rentalFee = calculateRentalFee(price);
    cout << "Borrow Date: "; cin >> bDate;
    cout << "Due Date: "; cin >> dDate;
    cout << "Return Date: "; cin >> rDate;
    cout << "Max Book: "; cin >> mB;

    ofstream outFile("borrow.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "TICKETID: " << tID << " | READERID: " << rID << " | BOOKID: " << bID 
                << " | COST: " << price << " | RENTALFEE: " << rentalFee 
                << " | BORROW: " << bDate << " | DUE: " << dDate 
                << " | RETURN: " << rDate << " | MAX: " << mB << endl;
        outFile.close();
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
    cout << "Select line: "; cin >> target;
    cout << "Price: "; cin >> price;
    cout << "Days: "; cin >> days;

    if (target > 0 && target <= (int)lines.size()) {
        double fee = days * price * 0.1 ; 
        lines[target - 1] = "OVERDUEFEE: " + to_string(fee) + " | " + lines[target - 1];
        ofstream outFile("borrow.txt");
        for (const string& s : lines) outFile << s << endl;
        outFile.close();
        cout << "Overdue Fee Added!\n";
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
    cout << "Select line for LOST book: "; cin >> target;
    cout << "Book Price: "; cin >> price;

    if (target > 0 && target <= (int)lines.size()) {
        double fee = price * 2; 
        lines[target - 1] = "LOSTFEE: " + to_string(fee) + " | " + lines[target - 1];
        
        ofstream outFile("borrow.txt");
        for (const string& s : lines) outFile << s << endl;
        outFile.close();
        cout << "Lost Fee Added successfully!\n";
    }
}

void BorrowTicket::findMostBorrowedBook() {
    ifstream idFile("bookid.txt");
    if (!idFile) { cout << "No book registry found.\n"; return; }

    string id;
    string mostMatchedID = "None";
    int maxMatches = 0;

    cout << "\n--- BOOK BORROW FREQUENCY ---\n";
    
    // Iterate through every unique ID registered
    while (getline(idFile, id)) {
        if (id.empty()) continue;

        ifstream borrowFile("borrow.txt");
        string line;
        string target = "BOOKID: " + id;
        int count = 0;

        // Search the main borrow file for matches
        while (getline(borrowFile, line)) {
            if (line.find(target) != string::npos) {
                count++;
            }
        }
        borrowFile.close();

        cout << "ID: " << id << " | Matches: " << count << endl;

        if (count > maxMatches) {
            maxMatches = count;
            mostMatchedID = id;
        }
    }
    idFile.close();

    cout << "-----------------------------\n";
    cout << "RESULT: Book " << mostMatchedID << " is the most borrowed (" << maxMatches << " times).\n";
}


void BorrowTicket::displayData() {
    ifstream inFile("borrow.txt");
    string line;
    int count = 1;
    while (getline(inFile, line)) cout << count++ << ". " << line << endl;
    inFile.close();
}
////////////////////////////////////////////////////
void Person::input() {
    cout << "Enter Full Name: "; 
    getline(cin >> ws, fullName);
    
    cout << "Gender: "; 
    getline(cin >> ws, gender);
    
    cout << "Birth Year: "; 
    cin >> birthYear; 
    cout << "Phone: "; 
    getline(cin >> ws, phoneNumber); 
}

void Person::display() {
    cout << "NAME: " << fullName << " | GENDER: " << gender << " | BIRTH: " << birthYear;
}

void Reader::input() {
    Person::input();
    
    cout << "Reader ID: "; 
    getline(cin >> ws, readerID);
    
    cout << "Reg Date: "; 
    getline(cin >> ws, registrationDate);
    
    cout << "Due Date: "; 
    getline(cin >> ws, dueDate);
    
    cout << "Rank (1-3): "; 
    cin >> readerCardRank;
}

void Reader::display() {
    Person::display();
    int canRent = readerCardRank + 4;
    cout << " | ID: " << readerID << " | MAX BOOKS: " << canRent << endl;
}

string Reader::getFileData() {
    int canRent = (readerCardRank == 1) ? 5 : (readerCardRank == 2) ? 6 : 7;
    return "NAME: " + fullName + " | GENDER: " + gender + " | BIRTH: " + to_string(birthYear) + 
           " | PHONE: " + phoneNumber + " | ID: " + readerID + " | REGDATE:" + registrationDate + 
           " | DUEDATE" + dueDate + "| RANK" + to_string(readerCardRank) +
           " = MAXBOOKS:" + to_string(canRent) ;
}

////////////////////


void Book::input() {
    cout << "Enter Book ID: "; cin >> id;
    cin.ignore();
    cout << "Enter Title: "; getline(cin, title);
    cout << "Enter Author: "; getline(cin, author);
    cout << "Enter Publisher: "; getline(cin, publisher);
    cout << "Enter Price: "; cin >> price;
}

void Book::display() {
    cout << "ID: " << id << " | Title: " << title << " | Author: " << author;
}

string Book::belongBook() {
    return "ID: " + id + " | TITLE: " + title + " | AUTHOR: " + author + 
           " | PUB: " + publisher + " | PRICE: " + to_string(price);
}

void Book::setFileContext() {
    int type;
    cout << "\nTarget File Category:\n1. Printed (Shelf 1-3)\n2. E-Books\nChoice: ";
    cin >> type;
    if (type == 1) {
        int s; 
	cout << "Enter Shelf # (1, 2, or 3): "; cin >> s;
        currentFile = "shelf" + to_string(s) + ".txt";
    } else {
        currentFile = "ebooks.txt";
    }
}

void Book::displayFile() {
    ifstream inFile(currentFile);
    if (!inFile) { 
        cout << "\n[!] Error: " << currentFile << " is missing.\n"; 
        return; 
    }
    
    string line;
    int count = 1;
    cout << "\n--- Records in " << currentFile << " ---\n";
    while (getline(inFile, line)) {
        cout << count++ << ". " << line << endl;
    }
    inFile.close();
}

void Book::editORdelete(bool isDelete) {
    displayFile();
    int lineNum;
    cout << "\nEnter Line Number to " << (isDelete ? "DELETE" : "EDIT") << ": ";
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
                cout << "\n[ Editing Mode ]\n";
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
        cout << "\n[+] File successfully updated.\n";
    } else {
        cout << "\n[!] Error: Line not found.\n";
    }
}
double Book::calculateRentalFee(double value){
	return value * 0.04;
}

///////////////////////////////////////////////
void PrintedBook::input() {
    Book::input();
    cout << "Enter Page Count: "; cin >> pages;
    cout << "Enter Weight: "; cin >> weight;
    cout << "Enter Shelf (1-3): "; cin >> shelf;
    cout << "Enter Status (Remain): "; cin >> remain;

    string filename = "shelf" + to_string(shelf) + ".txt";
    ofstream outFile(filename, ios::app);
    outFile << belongBook() << " | PAGES: " << pages << " | WEIGHT: " << weight << " | REMAIN: " << remain << endl;
    cout << "\n[+] Printed book recorded.\n";
}
//////////////////////////////////////////////// 
void Ebook::input() {
    Book::input();
    cout << "Enter File Size (MB): "; cin >> fileSize;
    cout << "Enter Format: "; cin >> format;
    cin.ignore();
    cout << "Enter Download Link: "; getline(cin, link);

    ofstream outFile("ebooks.txt", ios::app);
    outFile << belongBook() << " | SIZE: " << fileSize << "MB | FORMAT: " << format << " | LINK: " << link << endl;
    cout << "\n[+] E-Book recorded.\n";
}
/////////////////////////

void saveToFile(Reader& r) {
    ofstream outFile("readers.txt", ios::app);
    if (outFile.is_open()) {
        outFile << r.getFileData() << endl; 
        outFile.close();
        cout << "Record saved successfully.\n";
    }
}

void displayFromFile() {
    ifstream inFile("readers.txt");
    string line;
    cout << "\n--- CURRENT DATABASE RECORDS ---\n";
    if (inFile.is_open()) {
        int lineNum = 1;
        while (getline(inFile, line)) {
            cout << lineNum << ". " << line << endl << endl;
            lineNum++;
        }
        inFile.close();
    }
}

void extendDueDate() {
    vector<string> lines;
    string line;
    int lineNum;

    ifstream inFile("readers.txt");
    while (getline(inFile, line)) lines.push_back(line);
    inFile.close();

    cout << "Enter line number to extend due date: ";
    cin >> lineNum;

    if (lineNum > 0 && lineNum <= (int)lines.size()) {
        lines.erase(lines.begin() + (lineNum - 1));

        ofstream outFile("readers.txt");
        for (const auto& l : lines) outFile << l << endl;
        outFile.close();

        cout << "\n--- Extend Due Date ---\n";
        cout << "Please enter the updated details for the new line:\n";
        
        Reader r;
        r.input();     
        saveToFile(r); 
    } else {
        cout << "Invalid line number!\n";
    }
}

void blockReader(int lineNum) {
    vector<string> lines;
    string line;
    ifstream inFile("readers.txt");
    while (getline(inFile, line)) lines.push_back(line);
    inFile.close();

    if (lineNum > 0 && lineNum <= (int)lines.size()) {
        lines[lineNum - 1] = "#block# | " + lines[lineNum - 1];
        ofstream outFile("readers.txt");
        for (const auto& l : lines) outFile << l << endl;
        outFile.close();
        cout << "Line " << lineNum << " blocked.\n";
    }
}

// ==========================================
// 3. MAIN PROGRAM
// ==========================================

void displayMenu() {
    clearScreen(); 
    cout << "================================================================" << endl;
    cout << "                   LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << "================================================================" << endl;

    // Define column width
    int w = 22;

    cout << left 
         << setw(w) << "1. Add Printed Book"  << setw(w) << "7. Add New Reader"    << setw(w) << "13. Charge Overdue" << endl
         << setw(w) << "2. Add E-Book"        << setw(w) << "8. View All Readers"  << setw(w) << "14. Charge Lost Book" << endl
         << setw(w) << "3. Edit Book"          << setw(w) << "9. Extend Due Date"   << setw(w) << "15. Overdue Readers" << endl
         << setw(w) << "4. Delete Book"        << setw(w) << "10. Block A Reader"   << setw(w) << "16. Most Borrowed" << endl
         << setw(w) << "5. View All Books"     << setw(w) << "11. Add Borrow Tik."  << endl
         << setw(w) << "6. Search Book"        << setw(w) << "12. Update Borrow Tik." << setw(w) << "0. Exit Program" << endl;

    cout << "================================================================" << endl;
    cout << "Choice: ";
}
int main() {
    Book manager;
    Reader r; 
    BorrowTicket BT; 
    int choice;
    string pat; 

    do {
        displayMenu(); 
        cin >> choice;
	switch (choice) {
            case 1: { PrintedBook pb; pb.input(); pauseExecution(); break; }
            case 2: { Ebook eb; eb.input(); pauseExecution(); break; }
            case 3: manager.setFileContext(); manager.editORdelete(false); pauseExecution(); break;
            case 4: manager.setFileContext(); manager.editORdelete(true); pauseExecution(); break;
            case 5: manager.setFileContext(); manager.displayFile(); pauseExecution(); break;
	    case 6: cout << "pattern: "  ; cin >> pat ; search(pat); pauseExecution();break;  
	    case 7: r.input();saveToFile(r); pauseExecution();break;
	    case 8: displayFromFile();pauseExecution();break; 
	    case 9: displayFromFile();extendDueDate();pauseExecution ;break;
	    case 10: int l;cout << "Enter line number to block: ";cin >> l;blockReader(l);
	    case 11: BT.inputData(); pauseExecution();break; 
	    case 12: BT.updateData(); pauseExecution();break;
	    case 13: BT.calculateOverdueFeature(); pauseExecution();break;
	    case 14: BT.calculateLostFeature(); pauseExecution();break;
	    case 15: BT.searchKeyword("OVERDUEFEE:"); pauseExecution();break; 
	    case 16: BorrowTicket::findMostBorrowedBook(); pauseExecution();break; 
                break;


            case 0: cout << "\nExiting... Goodbye!\n"; break;
            default: cout << "\n[!] Invalid choice.\n"; pauseExecution();
        }
    } while (choice != 0);

    return 0;
}
