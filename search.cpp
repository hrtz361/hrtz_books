#include <iostream>
#include <fstream>
#include <string>
#include <regex>

class book{
	public:
		int search(std::string pattern); 

};


int book::search(std::string pattern){
    std::string filename = "input.txt";
    
    std::ifstream file(filename);
    
    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Could not open the file - '" << filename << "'" << std::endl;
        return 1;
    }

    try {
        std::regex re(pattern);
        std::string line;
        int line_number = 0;

        std::cout << "Searching for pattern: " << pattern << "\n" << std::string(30, '-') << std::endl;

        while (std::getline(file, line)) {
            line_number++;
            // Check if the line matches the regex pattern
            if (std::regex_search(line, re)) {
                std::cout << "Line " << line_number << ": " << line << std::endl;
            }
        }
    } catch (const std::regex_error& e) {
        std::cerr << "Invalid regex pattern: " << e.what() << std::endl;
        return 1;
    }

    file.close();
    return 0;
}

int main(){ 
book A; 
A.search("ha"); 
    return 0;
}
