#include <stdio.h>
#include "vm/VirtualMachine.h"
#include "sys/assembler/Assembler.h"
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
	// Assemble
	try {
		Assembler as("../src/sys/assembler/opcodes.lst");
		as.build(argv[0]);
	} catch(const char* error) {
		printf ("[Assembler Error] %s \n", error);
		return 1;
	}

	// Read in assembly file
	short (&mem)[memSize];

	string assemblyFile;

	// LOAD LINE BY LINE INTO MEM, CHECK FOR OUT OF RANGE ERROR
	// STRIP filename and append .o
	// IN ASSEMBLY, LINE 72, do the same.
	// At that point this should run. I hope.

	// Run
	try {
		VirtualMachine vm(arg[0], mem);
		as.build(input);
	} catch(const char* error) {
		printf ("[VirtualMachine Error] %s \n", error);
		return 1;
	}

	printf("Program Complete!\n");

	return 0;
}
