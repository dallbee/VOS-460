/**
 * Definition of the VirtualMachine class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 2.0
 * @date 15 May, 2013
 * @file VirtualMachine.h
 */

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Memory {
public:
	Memory(int memSize, int tlbSize);
	virtual ~Memory() {};

	short operator [](int logical) const;
	short & operator [](int logical);

private:
	vector<short> mem;
	map<short, short> buffer;
	int bufferSize;
};

class VirtualMachine {
public:
	static const int tlbSize = 8;
	static const int regSize = 4;
	static const int memSize = 256;

	VirtualMachine();
	virtual ~VirtualMachine() {};
	void run();
	void machineDump();
	void memoryDump(short memInt);

protected:
	friend class OS;

	short reg[regSize];
	Memory mem;
	short frames[32];

	unsigned clock;
	ofstream* outFile;
	fstream* inFile;
	string name;

	short pc;
	short ir;
	short sp;
	short base;
	short limit;
	short sr;
	short largestStack;

private:
	int OP;
	int RD;
	int I;
	int RS;
	unsigned char ADDR;
	signed char CONST;

	typedef void(VirtualMachine::*FunctionPointer)();
	FunctionPointer instructions[26];

	void pushStack(short pcbItem);
	short popStack();

	// Status Register Masking
	void setCarry(int value);
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

	// Instructions
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
	void haltExec() {};
	void noopExec() {};
};

#endif
