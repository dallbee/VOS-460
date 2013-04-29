#include <stdio.h>
#include "vm/VirtualMachine.h"
#include "sys/assembler/Assembler.h"
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
	printf ("%i \n",argc);
	printf ("%s \n",argv[1]);
	if(argc == 1){
		printf ("Please specify a Program");
	}

	// Assemble opcodes
	try {
		Assembler as("../src/sys/assembler/opcodes.lst");
		as.build(argv[1]);
	} catch(const char* error) {
		printf ("[Assembler Error] %s \n", error);
		return 1;
	}

	//Load assembly into memory
	short (mem)[VirtualMachine::memSize];
	string programAs = argv[1];
	programAs = (programAs.substr(0, programAs.find_first_of("."))).append(".o");
 	ifstream asFile(programAs.c_str());
 	if(!asFile){
 		printf ("OS: could not find assembly code file");
 	}


 	int asLine = 0, limit = 0;
 	asFile >> asLine;
 	while(!asFile.eof()){
 		printf ("asLine:\t %i \n", asLine);
 		mem[++limit] = asLine;
 		asFile >> asLine;
 		printf ("MEM[]: \t %u \n", mem[limit] );
 	}

 	//Execute the program
	try {
		VirtualMachine vm(argv[1], mem, limit);
	} catch(const char* error) {
		printf ("[VirtualMachine Error] %s \n", error);
		return 1;
	}

	// Read in assembly file


	//string assemblyFile;



	// LOAD LINE BY LINE INTO MEM, CHECK FOR OUT OF RANGE ERROR
	// STRIP filename and append .o
		//Done in assembler on as.build -Taylor
	// At that point this should run. I hope.

	//Run
	// try {
	// 	VirtualMachine vm(argv[0], mem);
	// 	as.build(input);
	// } catch(const char* error) {
	// 	printf ("[VirtualMachine Error] %s \n", error);
	// 	return 1;
	// }

	printf("Program Complete!\n");

	return 0;
}
