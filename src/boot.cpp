#include <iostream>
#include "os/os.h"
#include "kernel/virtualMachine.h"
#include "assembler/assembler.h"
using namespace std;

int main()
{
	try {
		Assembler("src/assembler/opscodes.lst");
	} catch(const char* error) {
		cout << "Assembler Error: " << error << endl;
	}

	return 0;
}

