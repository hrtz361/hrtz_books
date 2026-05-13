et ninclude <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to display the file with line numbers
void displayFile(const std::vector<std::string>& lines) {
    if (lines.empty()) {
        std::cout << "\n[File is empty or does not exist]\n";
        return;
    }
    std::cout << "\n--- File Content ---\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << i + 1 << ": " << lines[i] << std::endl;
    }
    std::cout << "--------------------\n";
}

// Function to save the vector back to the text file
void saveToFile(const std::string& filename, const std::vector<std::string>& lines) {
    std::ofstream outFile(filename);
    for (const auto& line : lines) {
        outFile << line << "\n";
    }
    outFile.close();
}

int main() {
    std::string filename = "readers.txt";
    std::string line;
    std::vector<std::string> lines;

    // 1. Read the file into a vector
    std::ifstream inFile(filename);
    if (inFile) {
        while (std::getline(inFile, line)) {
            lines.push_back(line);
        }
        inFile.close();
    }

    displayFile(lines);

    if (lines.empty()) return 0;

    // 2. Ask user for action
    int choice, lineNum;
    std::cout << "\nEnter line number to modify: ";
    std::cin >> lineNum;
    
    if (lineNum < 1 || lineNum > lines.size()) {
        std::cout << "Invalid line number.\n";
        return 1;
    }

    std::cout << "1. Edit Line\n2. Delete Line\nChoice: ";
    std::cin >> choice;
    std::cin.ignore(); // Clear buffer

    // 3. Process Choice
    if (choice == 1) {
        // Edit
        std::string newData;
        std::cout << "Enter new data for line " << lineNum << ": ";
        std::getline(std::cin, newData);
        lines[lineNum - 1] = newData;
        std::cout << "Line updated.\n";
    } 
    else if (choice == 2) {
        // Delete
        lines.erase(lines.begin() + (lineNum - 1));
        std::cout << "Line deleted.\n";
    } 
    else {
        std::cout << "Invalid choice.\n";
        return 1;
    }

    // 4. Write changes back to file
    saveToFile(filename, lines);
    std::cout << "File updated successfully.\n";

    return 0;
}
