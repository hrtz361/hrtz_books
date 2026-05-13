#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread> // For sleep
#include <chrono> // For time durations

using namespace std;

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
    cout << "\nPausing for 5 seconds to let you review..." << endl;
    this_thread::sleep_for(chrono::seconds(5));
}

// ==========================================
// 1. CLASS PROTOTYPES
// ==========================================

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

// ==========================================
// 2. FUNCTION IMPLEMENTATIONS
// ==========================================

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
        int s; cout << "Enter Shelf # (1, 2, or 3): "; cin >> s;
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

// ==========================================
// 3. MAIN PROGRAM
// ==========================================

int main() {
    Book manager;
    int choice;

    do {
        clearScreen(); // Refresh the menu view
        cout << "===============================" << endl;
        cout << "   LIBRARY MANAGEMENT SYSTEM" << endl;
        cout << "===============================" << endl;
        cout << "1. Add Printed Book" << endl;
        cout << "2. Add E-Book" << endl;
        cout << "3. Edit Book" << endl;
        cout << "4. Delete Book" << endl;
        cout << "5. View File Content" << endl;
        cout << "0. Exit Program" << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: { PrintedBook pb; pb.input(); pauseExecution(); break; }
            case 2: { Ebook eb; eb.input(); pauseExecution(); break; }
            case 3: manager.setFileContext(); manager.editORdelete(false); pauseExecution(); break;
            case 4: manager.setFileContext(); manager.editORdelete(true); pauseExecution(); break;
            case 5: manager.setFileContext(); manager.displayFile(); pauseExecution(); break;
            case 0: cout << "\nExiting... Goodbye!\n"; break;
            default: cout << "\n[!] Invalid choice.\n"; pauseExecution();
        }
    } while (choice != 0);

    return 0;
}
