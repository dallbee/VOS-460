#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H
#endif

#include <map>
#include <set>
#include <string>
using namespace std;

typedef void (*ScriptFunction)(void);
typedef std::map<std::string, ScriptFunction> script_map;

class VirtualMachine {
public:
	VirtualMachine();

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

	map<string, int> opCodes;

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

typedef void (VirtualMachine::*math_method_t)(double);
typedef std::map<std::string, math_method_t> math_func_map_t;
