#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H
#endif

#include <string>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

class VirtualMachine {
public:
	VirtualMachine();
	void machineDump();

private:
	static const int regSize = 4;
	static const int memSize = 256;
	int reg[regSize];
	int mem[memSize];
	unsigned clock;
	string fileName;

	typedef void(VirtualMachine::*FunctionPointer)();
	FunctionPointer instructions[32];

	int OP;
	int RD;
	int I;
	int RS;
	int ADDR;
	int CONST;

	int pc;
	int ir;
	int sp;
	int base;
	int limit;
	int sr;

	void pushStack(int pcbItem);
	int popStack();

	// Status Register Masking
	void setCarry();
	void setGreater();
	void setEqual();
	void setLess();
	void setOverflow();
	int getCarry();
	int getGreater();
	int getEqual();
	int getLess();
	int getOverflow();

	// ALU instructions
	void loadExec();
	void storeExec();
	void addExec();
	void addcExec();
	void subExec();
	void subcExec();
	void andExec();
	void xorExec();
	void complExec();
	void shlExec();
	void shlaExec();
	void shrExec();
	void shraExec();
	void comprExec();
	void getstatExec();
	void putstatExec();
	void jumpExec();
	void jumplExec();
	void jumpeExec();
	void jumpgExec();
	void callExec();
	void returnExec();
	void readExec();
	void writeExec();
	void haltExec();
	void noopExec();
};
