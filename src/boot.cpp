#include <stdio.h>
#include "vm/VirtualMachine.h"
#include "sys/OS.h"
#include "sys/assembler/Assembler.h"
#include <stdio.h>
using namespace std;

int main()
{
	/*

	try {
		Assembler test ("../src/sys/assembler/opcodes.lst");
		test.build("../src/sys/assembler/test.asm");
	} catch(const char* error) {
		printf ("[Assembler Error] %s \n", error);
	}
	*/
	//virtualMachine();
	int i = 0;
	signed char j = -5;
	i = j;
	printf("Number: %i\n", i);

	return 0;
}

