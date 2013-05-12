/**
 * Implementation of the os.cpp
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.0
 * @date 29 April, 2013
 * @file os.cpp
 */
#include <stdio.h>
#include <string>
#include "OS.h"
#include "vm/VirtualMachine.h"
#include "sys/assembler/Assembler.h"
using namespace std;

PCB::PCB(pc(), ir(), sp(memSize - 1), base(), limit(progLimit), sr()){

}

PCB::PCB(string fileName, int tempBase, int tempLimit){
	this -> fileName = fileName;
	//input  = filename +".in";
	//output = filename +"out";
	stack  = filename + ".st";
	reg[VirtualMachine::regSize];
	base  = tempBase;
	limit = tempLimit - tempBase;
	ioTime = 0;
	execTime = 0;
}

OS::OS(userTotal, osClock, tempLimit, tempBase, itdleTotal){

}

void OS::loadState(){ //loads PCB of running process, just before it starts
	VM.pc = running -> pc + running -> base;
	VM.sp = running -> sp;
	VM.r = running -> reg;
	VM.ir = running -> ir;
	VM.readNum = running -> readNum;
	VM.fileName = running -> fileName;
	VM.base = running -> base;
	VM.limit = running -> limit;
}


void OS::finish(){ //get total times

}

void OS::saveState(){ //saves PCB of running process upon finishing

}

void OS::scheduler(){

}

void OS::prcoessFinish(){

}

void OS::run(){

}

void OS::load(){

}

int main(){
	OS.os;
	os.load();
	os.scheduler();
	os.run()
	return 0;
}

//Old Code:

// int main(int argc, char *argv[])
// {
// 	fAndF = argv[1]; //filesAndFolders
// 	fAndF = "../io/"+(fAndF.substr(0, fAndF.find_last_of("."))).append("/")+fAndF;
// 	if(argc == 1){
// 		printf ("Please specify a Program");
// 	}


// 	// Assemble opcodes
// 	try {
// 		Assembler as("../src/sys/assembler/opcodes.lst");
// 		as.build(fAndF);
// 	} catch(const char* error) {
// 		printf ("[Assembler Error] %s \n", error);
// 		return 1;
// 	}


// 	//Load assembly code into memory
// 	mem[VirtualMachine::memSize]={0};
// 	programAs = fAndF;
// 	programAs = (programAs.substr(0, programAs.find_last_of("."))).append(".o");
//  	ifstream asFile(programAs.c_str());
//  	if (! asFile.good()){
//  		printf ("OS: could not find assembly code file");
//  	}
//  	int asLine = 0, limit = 0;
//  	asFile >> asLine;
//  	while(!asFile.eof()){
//  		mem[limit++] = asLine;
//  		asFile >> asLine;
//  	}


//  	//Execute the program
// 	try {
// 		VirtualMachine vm(fAndF, mem, limit);
// 	} catch(const char* error) {
// 		printf ("[VirtualMachine Error] %s \n", error);
// 		return 1;
// 	}


// 	printf("Program Complete!\n");
// 	return 0;
// }
