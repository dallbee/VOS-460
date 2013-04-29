#include <stdio.h>
#include "vm/VirtualMachine.h"
#include "sys/assembler/Assembler.h"
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
	string fAndF = argv[1]; //filesAndFolders
	fAndF = "../io/"+(fAndF.substr(0, fAndF.find_last_of("."))).append("/")+fAndF;
	if(argc == 1){
		printf ("Please specify a Program");
	}

	// Assemble opcodes
	try {
		Assembler as("../src/sys/assembler/opcodes.lst");
		as.build(fAndF);
	} catch(const char* error) {
		printf ("[Assembler Error] %s \n", error);
		return 1;
	}

	//Load assembly code into memory
	short (mem)[VirtualMachine::memSize]={0};
	string programAs = fAndF;
	programAs = (programAs.substr(0, programAs.find_last_of("."))).append(".o");
 	ifstream asFile(programAs.c_str());
 	if(!asFile){
 		printf ("OS: could not find assembly code file");
 	}


 	int asLine = 0, limit = 0;
 	asFile >> asLine;
 	while(!asFile.eof()){
 		mem[limit++] = asLine;
 		asFile >> asLine;
 	}

 	//Execute the program
	try {
		VirtualMachine vm(fAndF, mem, limit);
	} catch(const char* error) {
		printf ("[VirtualMachine Error] %s \n", error);
		return 1;
	}

	printf("Program Complete!\n");

	return 0;
}
