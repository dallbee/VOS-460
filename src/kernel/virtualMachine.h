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


	//Basic Commands Required
	//I was thinking to suffix each with 'VM' -Taylor
	// statementVM()
	// I wanted to keep consistency though, not just the overloaded ones -Taylor
	int load();
	int store();
//	int add(); //overloaded statement
	int addc();
	int sub();
	int subc();
//	int and(); //overloaded statement
//	int xor(); //overloaded statement
//	int compl(); //overloaded statement
	int shl();
	int shla();
	int shr();
	int shra();
	int compr();
	int getstat();
	int putstat();
	int jump();
	int jumpl();
	int jumpe();
	int jumpg();
	int call();
//	int return(); //overloaded statement
	int read();
	int write();
	int halt();
	int noop();
};
