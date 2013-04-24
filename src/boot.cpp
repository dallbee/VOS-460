#include <stdio.h>
#include "os/os.h"
#include "kernel/VirtualMachine.h"
#include "assembler/Assembler.h"
using namespace std;

int main()
{
	try {
		Assembler test ("../src/assembler/opcodes.lst");
		test.build("../src/assembler/test.asm");
	} catch(const char* error) {
		printf ("[Assembler Error] %s \n", error);
	}

	//virtualMachine();

	return 0;
}

