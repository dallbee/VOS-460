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
	static const int regSize = 4;
	static const int memSize = 256;

	typedef void(VirtualMachine::*FunctionPointer)();

	struct StatusRegister {
		int op;
		int rd;
		int i;
		int rs;
		int addr;
		int overflow;
		int less;
		int equal;
		int greater;
		int carry;
		int constant;
	} sr;

	struct PCB {
		int pc;
		int sp;
		int base;
		int limit;
		int ir;
		int sr;
	};

	FunctionPointer instructions[32];

	unsigned clock;
	int reg[regSize];
	int mem[memSize];

	//Register masking and misc hardware udpates
	void setCarry();
	void setGreater();
	void setEqual();
	void setLess();
	void writeStatus();

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
