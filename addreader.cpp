#include <iostream>
#include <fstream>
#include <string>
#include <limits>

struct Reader {
    std::string fullName;
    std::string gender;
    int birthYear;
    std::string phoneNumber;
    std::string readerID;
    std::string registrationDate;
    int maxBooks;
};

void addReadersToFile(const std::string& filename) {
    int numReaders;
    
    // Open file in Append mode (ios::app) to keep existing content
    std::ofstream outFile(filename, std::ios::app);
    
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    std::cout << "How many readers would you like to add? ";
    while (!(std::cin >> numReaders)) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    std::cin.ignore(); // Clear the newline character from the buffer

    for (int i = 0; i < numReaders; ++i) {
        Reader r;
        std::cout << "\n--- Entering details for Reader #" << (i + 1) << " ---\n";

        std::cout << "Full Name: ";
        std::getline(std::cin, r.fullName);

        std::cout << "Gender: ";
        std::getline(std::cin, r.gender);

        std::cout << "Birth Year: ";
        std::cin >> r.birthYear;
        std::cin.ignore();

        std::cout << "Phone Number: ";
        std::getline(std::cin, r.phoneNumber);

        std::cout << "Reader ID: ";
        std::getline(std::cin, r.readerID);

        std::cout << "Registration Date (DD/MM/YYYY): ";
        std::getline(std::cin, r.registrationDate);

        std::cout << "Max Books Allowed: ";
        std::cin >> r.maxBooks;
        std::cin.ignore();

        // Write to file: Each reader on a single line, attributes separated by commas
        outFile << r.fullName << "|" 
                << r.gender << "|" 
                << r.birthYear << "|" 
                << r.phoneNumber << "|" 
                << r.readerID << "|" 
                << r.registrationDate << "|" 
                << r.maxBooks << "\n";
    }

    outFile.close();
    std::cout << "\nData successfully appended to " << filename << std::endl;
}

int main() {
    const std::string filename = "readers.txt";
    addReadersToFile(filename);
    return 0;
}
