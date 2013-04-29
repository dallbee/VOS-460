#ifndef OS_H
#define OS_H

#include <string>
#include "assembler/Assembler.h"
#include "../vm/VirtualMachine.h"
using namespace std;

class OS {
public:
	OS();

	void launchProgram(string programFile);

	//debugging
	void vmDump();

private:

	string program;
	string programAss;
	short mem[VirtualMachine::memSize];
	Assembler ass;
	VirtualMachine vm(mem);

	void loadFile();
	void assemble();
	void execute();
};

#endif
