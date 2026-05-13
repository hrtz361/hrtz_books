#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std; 
// Function to display the file with line numbers
void displayFile(const  vector< string>& lines) {
    if (lines.empty()) {
         cout << "\n[File is empty or does not exist]\n";
        return;
    }
     cout << "\n--- File Content ---\n";
    for (size_t i = 0; i < lines.size(); ++i) {
         cout << i + 1 << ": " << lines[i] <<  endl;
    }
     cout << "--------------------\n";
}

// Function to save the vector back to the text file
void saveToFile(const  string& filename, const  vector< string>& lines) {
     ofstream outFile(filename);
    for (const auto& line : lines) {
        outFile << line << "\n";
    }
    outFile.close();
}

int main() {
     string filename = "library.txt";
     string line;
     vector< string> lines;

    // 1. Read the file into a vector
     ifstream inFile(filename);
    if (inFile) {
        while ( getline(inFile, line)) {
            lines.push_back(line);
        }
        inFile.close();
    }

    displayFile(lines);

    if (lines.empty()) return 0;

    // 2. Ask user for action
    int choice, lineNum;
     cout << "\nEnter line number to modify: ";
     cin >> lineNum;
    
    if (lineNum < 1 || lineNum > lines.size()) {
         cout << "Invalid line number.\n";
        return 1;
    }

     cout << "1. Edit Line\n2. Delete Line\nChoice: ";
     cin >> choice;
     cin.ignore(); // Clear buffer

    // 3. Process Choice
    if (choice == 1) {
        // Edit
         string newData;
         cout << "Enter new data for line " << lineNum << ": ";
         getline( cin, newData);
        lines[lineNum - 1] = newData;
         cout << "Line updated.\n";
    } 
    else if (choice == 2) {
        // Delete
        lines.erase(lines.begin() + (lineNum - 1));
         cout << "Line deleted.\n";
    } 
    else {
         cout << "Invalid choice.\n";
        return 1;
    }

    // 4. Write changes back to file
    saveToFile(filename, lines);
     cout << "File updated successfully.\n";

    return 0;
}
