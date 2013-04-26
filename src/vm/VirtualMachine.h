#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H
#endif

#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

class VirtualMachine {
public:
	VirtualMachine();

	//useful debugging
	void memory_dump();
	void register_dump();
	void print_pc();
	void print_sp();
	void print_clock();
	void machine_dump();

private:
	static const unsigned regSize = 4;
	static const unsigned memSize = 256;
	vector<void (VirtualMachine::*)()> instructions;

	int reg[regSize];
	int mem[memSize];

	unsigned OP;
	unsigned RD;
	unsigned I;
	unsigned RS;
	unsigned ADDR;
	unsigned CARRY;
	unsigned OVERFLOW;
	unsigned LESS;
	unsigned EQUAL;
	unsigned GREATER;
	int CONST;

	unsigned clock;
	//PCB related
	unsigned pc;
	unsigned sp;
	unsigned base;
	unsigned limit;
	unsigned ir;
	unsigned sr;//bits 15 -> 6 unused last 5: oVerflow, Less, Equal, Greater, Carry

	//Register masking and misc hardware udpates
	void setCarry();
	void writeStatus();
	void incrementClock(int cycles);

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
