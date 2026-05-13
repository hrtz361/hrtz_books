#include <iostream>
#include <fstream>
#include <string>
#include <limits> // Required for numeric_limits

class Book {
public:
    std::string title;
    std::string author;
    int year;

    std::string toFileFormat() const {
        return title + " | " + author + " | " + std::to_string(year);
    }
};

int main() {
    int objectCount;
    std::string filename = "library.txt";

    std::cout << "How many new books would you like to add? ";
    if (!(std::cin >> objectCount)) {
        std::cerr << "Invalid input." << std::endl;
        return 1;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Opening with std::ios::app ensures new data is added to the end
    std::ofstream outFile(filename, std::ios::app); 
    
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    for (int i = 0; i < objectCount; ++i) {
        Book tempBook;
        std::cout << "\n--- Book #" << (i + 1) << " ---" << std::endl;

        std::cout << "Title: ";
        std::getline(std::cin, tempBook.title);

        std::cout << "Author: ";
        std::getline(std::cin, tempBook.author);

        std::cout << "Year: ";
        std::cin >> tempBook.year;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        outFile << tempBook.toFileFormat() << std::endl;
    }

    outFile.close();
    std::cout << "\nData successfully appended to " << filename << std::endl;

    return 0;
}
