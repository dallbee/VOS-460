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
	//static const int regSize = 4;
	//static const int memSize = 256;
	
	int loadExec();
	int storeExec();
 	int addExec(); 
	int addcExec();
	int subExec();
	int subcExec();
  	int andExec();  
 	int xorExec();  
 	int complExec();  
	int shlExec();
	int shlaExec();
	int shrExec();
	int shraExec();
	int comprExec();
	int getstatExec();
	int putstatExec();
	int jumpExec();
	int jumplExec();
	int jumpeExec();
	int jumpgExec();
	int callExec();
 	int returnExec(); 
	int readExec();
	int writeExec();
	int haltExec();
	int noopExec();
};
