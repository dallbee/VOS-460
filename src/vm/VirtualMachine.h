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
	unsigned pc;
	unsigned sp;
	unsigned base;
	unsigned limit;
	unsigned ir;
	unsigned sr;

	void loadInstruction();
	void storeInstruction();
	void addInstruction();
	void addcInstruction();
	void subInstruction();
	void subcInstruction();
	void andInstruction();
	void xorInstruction();
	void complInstruction();
	void shlInstruction();
	void shlaInstruction();
	void shrInstruction();
	void shraInstruction();
	void comprInstruction();
	void getstatInstruction();
	void putstatInstruction();
	void jumpInstruction();
	void jumplInstruction();
	void jumpeInstruction();
	void jumpgInstruction();
	void callInstruction();
	void returnInstruction();
	void readInstruction();
	void writeInstruction();
	void haltInstruction();
	void noopInstruction();

};


