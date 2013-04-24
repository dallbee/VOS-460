#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H
#endif


#include <map>
#include <set>
#include <string>
using namespace std;

class virtualMachine {
public:
	virtualMachine();

	//run the assembly code
	void start(string assemblyCode);

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

	int reg[regSize];
 	int mem[memSize];

 	unsigned clock;
 	unsigned programCounter;
 	unsigned stackPointer;
 	unsigned base;
 	unsigned limit;
 	unsigned instructionRegister;
 	unsigned statusRegister;


	map<string, int> opCodes;

	//Basic Commands Required,
	//just stores data in memory
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
