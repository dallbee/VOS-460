/**
 * Implementation of the os.cpp
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.0
 * @date 29 April, 2013
 * @file os.cpp
 */

#include "OS.h"
#include "vm/VirtualMachine.h"
#include "sys/assembler/Assembler.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <list>
#include <queue>
using namespace std;

/**
 * Creates a PCB Object
 */
PCB::PCB(string fileName) : name(fileName), reg(), pc(), sr(),
	sp(VirtualMachine::memSize - 1), base(), limit(), execTime(), waitTime(),
	turnTime(), ioTime(), largestStack(),
	o(string("../io/" + name + "/" + name + ".o").c_str()),
	out(string("../io/" + name + "/" + name + ".out").c_str()),
	in(string("../io/" + name + "/" + name + ".in").c_str()),
	st(string("../io/" + name + "/" + name + ".st").c_str())
{
	//base  = tempBase;
	//limit = tempLimit - tempBase;
}

/**
 * Creates an OS object
 */
OS::OS() : progs(), readyQ(), waitQ(), running(), osClock(),
	tempBase(), tempLimit(), exitCode(), userTotal(), idleTotal(),
	systemCpuUtil(), userCpuUtil(), throughput(), osOut(), processStack(),
	asLine(), limit(), programAs(), mem()
{

}

/*
 * Brings a new PCB into the Virtual Machine
 */
/*
void OS::loadState()
{ //loads PCB of running process, just before it starts
	VM.pc = running->pc + running->base;
	VM.sp = running->sp;
	VM.reg = running->reg;
	VM.ir = running->ir;
	VM.fileName = running->fileName;
	VM.base = running->base;
	VM.limit = running->limit;
}*/

/**
 * [OS::finish description]
 */
/*
void OS::finish()
{ //get total times
	systemCpuUtil = (osClock - idleTotal)/osClock;
	userCpuUtil	= userTotal/osClock;
	throughput = progs.size()/osClock;

	//os output here. Still deciding how to handle.
}*/

/*
 * Saves the current state of the Virtual Machine into a PCB.
 */
/*
void OS::saveState()
{ //saves PCB of running process upon using its timeslice, or io starts
	running->pc = VM.pc = VM.base;
	running->sp = VM.sp;
	running->reg = VM.reg;
	running->ir = VM.ir;
	running->fileName = VM.fileName;
	running->base = VM.base;
	running->limit = VM.limit;
}*/

/**
 * 
 */
void OS::scheduler()
{

}

/*

 */
void OS::processFinish()
{
	//program output file
}

/*

 */
void OS::run()
{


}

/*
 * Reads and assembles all programs in the io directory.
 */
void OS::load()
{
	// Copies the name of all program files in the io directory to progs file
	if (system("ls -d -1 $PWD/../io/**/*.s | grep -Po '(?<=\\/)\\w*(?=\\/\\w*\\.s)' > progs")) {
		throw "Error while attempting to get program listing";
	}

	ifstream progFile("progs");
	for (string line; getline(progFile, line);) {
		PCB *pcb = new PCB(line);
		progs.push_back(pcb);

		// Assemble opcodes
	 	try {
	 		Assembler as("../src/sys/assembler/opcodes.lst");
	 		as.build(string("../io/" + line + "/" + line + ".s"));
	 	} catch(const char* error) {
	 		printf("[Assembler Error] %s \n", error);
	 	}

	 	// Load object code into memory
		for(string opCode; getline(pcb->o, opCode);) {
			stringstream convert(opCode);
			convert >> mem[pcb->limit++];
		}
	}

	if (system("rm -f progs")) {
		printf("Could not remove progs file");
	}
}

/**
 * Initializes the Operating system and boots.
 * 
 * @return Failure status
 */
int main()
{
	try {
		OS os;
		os.load();
		os.scheduler();
		os.run();
	} catch(const char* error) {
		printf("[Operating System Error] %s \n", error);
	}
	
	return 0;
}

//Old Code:

// int main(int argc, char *argv[])
// {
// 	fAndF = argv[1]; //filesAndFolders
// 	fAndF = "../io/"+(fAndF.substr(0, fAndF.find_last_of("."))).append("/")+fAndF;
// 	if(argc == 1)
//  {
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
//  	if (! asFile.good())
//	{
//  		printf ("OS: could not find assembly code file");
//  	}
//  	int asLine = 0, limit = 0;
//  	asFile >> asLine;
//  	while(!asFile.eof())
//	{
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
