#include <stdio.h>
#include "os/os.h"
#include "kernel/virtualMachine.h"
#include "assembler/assembler.h"
using namespace std;

int main()
{
	try {
		Assembler("../src/assembler/opcodes.lst");
	} catch(const char* error) {
		printf ("Assembler Error: %s \n", error);
	}

	virtualMachine();

	return 0;
}

