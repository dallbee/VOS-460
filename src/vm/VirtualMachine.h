/**
 * Definition of the VirtualMachine class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.1
 * @date 28 April, 2013
 * @file VirtualMachine.h
 */

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H


#include <string>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

class VirtualMachine {
public:
	static const int regSize = 4;
	static const int memSize = 256;

	VirtualMachine(string file, short (&memory)[memSize],short progLimit);
	void execute(string programAss);
	void machineDump();
	void memoryDump(short memInt);

private:
	short reg[regSize];
	short (&mem)[memSize];
	unsigned clock;
	string fileName;

	typedef void(VirtualMachine::*FunctionPointer)();
	FunctionPointer instructions[26];

	int OP;
	int RD;
	int I;
	int RS;
	unsigned char ADDR;
	signed char CONST;

	short pc;
	short ir;
	short sp;
	short base;
	short limit;
	short sr;

	void pushStack(short pcbItem);
	short popStack();

	// Status Register Masking
	void setCarry();
	void setCarryRight();
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

#endif
