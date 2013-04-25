#include <stdio.h>
#include "vm/VirtualMachine.h"
#include "sys/os.h"
#include "sys/assembler/Assembler.h"
using namespace std;

int main()
{
	try {
		Assembler test ("../src/sys/assembler/opcodes.lst");
		test.build("../src/sys/assembler/test.asm");
	} catch(const char* error) {
		printf ("[Assembler Error] %s \n", error);
	}

	//virtualMachine();

	return 0;
}

