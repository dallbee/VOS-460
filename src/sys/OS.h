#ifndef OS_H
#define OS_H

#include <string>
#include "assembler/Assembler.h"
#include "../vm/VirtualMachine.h"
using namespace std;

class OS {
public:
	OS();
	string program;

	void launchProgram(string programFile);

	//debugging
	void vmDump();

private:


	Assembler ass;
	VirtualMachine vm;

	void loadFile();
	void assemble();
	void execute();
};

#endif
