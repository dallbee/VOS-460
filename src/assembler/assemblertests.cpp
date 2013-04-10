#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;

int main () {
	unsigned limit = 0;
	string filePath = "example.txt";
	string line;
	vector<string> output;

	//simply read in the file
	cout << filePath;
	ifstream file;
  	file.open ( filePath.c_str() );
  	while ( getline( file, line) ){
  		++limit;
  		output.push_back(line);
  		std::istringstream iss(line);
  	}

	//parsing ignore   !  exclamation  treat as comments
  	for( int i = limit; i > 0; i--){

  	}



return 0;
}
