#include<iostream>
#include<fstream>
#include<regex> 
#include<string> 
using namespace std; 
int main(){
	string buffer; 
	ifstream infile("FILE_stream.cpp"); 
	while(getline(infile,buffer)){
		cout << buffer << endl ; 
	}
	infile.close();
	ofstream File("save.txt"); 
	File << "thequickbrownfoxjumpsoverthelazydog";
	File.close(); 
}	
