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

	//debugging
	void machineDump();

private:
	static const unsigned regSize = 4;
	static const unsigned memSize = 256;
	vector<void (VirtualMachine::*)()> instructions;

	int reg[regSize];
	int mem[memSize];

	unsigned OP;	//Operation
	unsigned RD;	//Register-Destination
	unsigned I;		//Immediate
	unsigned RS;	//Register-Source
	unsigned ADDR;	//Address
	int CONST;		//Constant

	unsigned clock;

	string filename;

	void pushStack(int pcbItem);
	int  popStack();
	unsigned programCounter;
	unsigned stackPointer;
	unsigned base;
	unsigned limit;
	unsigned instructionReg;
	unsigned statusReg;//bits 15 -> 6 unused last 5: oVerflow, Less, Equal, Greater, Carry

	//hardware udpates

	void writeStatus();
	void incrementClock(unsigned cycles);

	//Status Register Masking
	void setCarry();
	void setGreater();
	void setEqual();
	void setLess();
	void setOverflow();
	int getCarry();
	int getGreater();
	int getEqual();
	int getLess();


	//instructions
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
