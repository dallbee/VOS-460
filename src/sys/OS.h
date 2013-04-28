#ifndef OS_H
#define OS_H
#endif

#include <string>
#include "Assember.h"
#include "VirtualMachine.h"
using namespace std;

class OS {
public:
	OS();

	void launchProgram(string program);
	//debugging
	void vmDump();

private:

	string program;

	Assembler ass;
	VirtualMachine vm;

	void loadFile();
	void assemble();
	void execute();
};
