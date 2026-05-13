#include <iostream>
#include <fstream>
#include <string>
#include <limits> // Required for numeric_limits
using namespace std; 
class Book {
public:
     string title;
     string author;
    int year;

     string toFileFormat() const {
        return title + " | " + author + " | " +  to_string(year);
    }
};

int main() {
    int objectCount;
     string filename = "library.txt";

     cout << "How many new books would you like to add? ";
    if (!( cin >> objectCount)) {
         cerr << "Invalid input." <<  endl;
        return 1;
    }

     cin.ignore( numeric_limits< streamsize>::max(), '\n');

    // Opening with  ios::app ensures new data is added to the end
     ofstream outFile(filename,  ios::app); 
    
    if (!outFile.is_open()) {
         cerr << "Error: Could not open file." <<  endl;
        return 1;
    }

    for (int i = 0; i < objectCount; ++i) {
        Book tempBook;
         cout << "\n--- Book #" << (i + 1) << " ---" <<  endl;

         cout << "Title: ";
         getline( cin, tempBook.title);

         cout << "Author: ";
         getline( cin, tempBook.author);

         cout << "Year: ";
         cin >> tempBook.year;
         cin.ignore( numeric_limits< streamsize>::max(), '\n');

        outFile << tempBook.toFileFormat() <<  endl;
    }

    outFile.close();
     cout << "\nData successfully appended to " << filename <<  endl;

    return 0;
}
