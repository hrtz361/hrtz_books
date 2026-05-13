#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std; 
class book{
	public:
		int search( string pattern); 

};


int book::search( string pattern){
     string filename = "library.txt";
    
     ifstream file(filename);
    
    // Check if the file opened successfully
    if (!file.is_open()) {
         cerr << "Could not open the file - '" << filename << "'" <<  endl;
        return 1;
    }

    try {
         regex re(pattern);
         string line;
        int line_number = 0;

         cout << "Searching for pattern: " << pattern << "\n" <<  string(30, '-') <<  endl;

        while ( getline(file, line)) {
            line_number++;
            // Check if the line matches the regex pattern
            if ( regex_search(line, re)) {
                 cout << "Line " << line_number << ": " << line <<  endl;
            }
        }
    } catch (const  regex_error& e) {
         cerr << "Invalid regex pattern: " << e.what() <<  endl;
        return 1;
    }

    file.close();
    return 0;
}

int main(){ 
book A; 
string pattern; 
cout << "search name: "; 
cin >> pattern; 
A.search(pattern); 
    return 0;
}
