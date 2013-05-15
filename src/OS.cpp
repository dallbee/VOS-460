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
 * Creates a PCB object
 * Responsible for setting up the main file streams for the entire system
 */
PCB::PCB(string fileName) : name(fileName), reg(), pc(), sr(), ir(),
	sp(VirtualMachine::memSize - 1), base(), limit(), execTime(), waitTime(),
	turnTime(), ioTime(), largestStack(),
	oFile(new fstream(string("../io/" + name + "/" + name + ".o").c_str())),
	outFile(new fstream(string("../io/" + name + "/" + name +".out").c_str())),
	inFile(new fstream(string("../io/" + name + "/" + name + ".in").c_str())),
	stFile(new fstream(string("../io/" + name + "/" + name + ".st").c_str()))
{
}

/**
 * Deconstructs the PCB object
 */
PCB::~PCB()
{
	delete oFile;
	delete outFile;
	delete inFile;
	delete stFile;
}

/**
 * Creates an OS object
 */
OS::OS() : VM(), progs(), readyQ(), waitQ(), running(), exitCode(), userTotal(),
	idleTotal(), osOut(), processStack(), asLine(), limit(), programAs()
{
}

/*
 * Reads and assembles all programs in the io directory.
 */
void OS::load()
{
	// Copies the paths of all program files in the io directory to progs file
	if (system("ls -d ../io/**/*.s > progs")) {
		throw "Error while attempting to get program listing";
	}

	ifstream progFile("progs");
	for (string line; getline(progFile, line);) {
		string name = line.substr(line.find_last_of("/") + 1,
		    line.find_last_of(".") - line.find_last_of("/") - 1);

		PCB *pcb = new PCB(name);

		// Assemble opcodes
		try {
			Assembler as("../src/sys/assembler/opcodes.lst");
			as.build(string(line));
		} catch(const char* error) {
			printf("[Assembler Error] %s \n", error);
		}

		// Load object code into memory
		pcb->base = limit;
		pcb->pc = limit;

		for(string opCode; getline(*pcb->oFile, opCode);) {
			stringstream convert(opCode);
			convert >> VM.mem[limit++];
		}

		progs.push_back(pcb);
		readyQ.push(pcb);
	}
	//for(int i = 0; i < limit; ++i) { //outputting memory to make sure progs loaded into mem
    //printf("Memory[%u] \t %u \n", i, VM.mem[i]);
    //}
}

/**
 * Organizes the Wait and Ready queues, and sets up the appropriate PCB to run.
 */
void OS::schedule()
{
	for(unsigned i = 0; i < waitQ.size(); ++i) {
		if(waitQ.front()->ioTime <= VM.clock) {
			readyQ.push(waitQ.front());
			waitQ.pop();
		} else {
			waitQ.push(waitQ.front());
			waitQ.pop();
		}
	}

	if(readyQ.size()) {
		running = readyQ.front();
		readyQ.pop();
	} else {
		running = NULL;
	}
}

/*
 * Brings a new PCB into the Virtual Machine
 */
void OS::loadState()
{
	copy(&running->reg[0], &running->reg[VM.regSize], VM.reg);
	VM.pc = running->pc + running->base;
	VM.sp = running->sp;
	VM.sr = running->sr;
	VM.base = running->base;
	VM.limit = running->limit;
	VM.inFile = running->inFile;
	VM.outFile = running->outFile;
}

/*
 * Saves the current state of the Virtual Machine into a PCB.
 */
void OS::saveState()
{
	copy(&VM.reg[0], &VM.reg[VM.regSize], running->reg);
	running->pc = VM.pc - VM.base;
	running->sp = VM.sp;
	running->sr = VM.sr;
	running->base = VM.base;
	running->limit = VM.limit;
	running->inFile = VM.inFile;
	running->outFile = VM.outFile;
}

/*

 */
void OS::run()
{
	//progs(1).base;

}

/*

 */
void OS::processFinish()
{
	int systemTime = 0;
	float systemCpuUtil = ((float)(VM.clock - idleTotal)/(float)VM.clock)*100.0;
	float userCpuUtil = ((float)userTotal/(float)VM.clock)*100.0;
	float throughput = (float)progs.size()/(float)VM.clock/10000.0;

	running->outFile->precision(2);
	*running->outFile << endl << "[Program Statistics]" << endl;
	*running->outFile << "System Time: " << systemTime << endl;
	*running->outFile << "System CPU Util: " << systemCpuUtil << "%" << endl;
	*running->outFile << "User CPU Util: " << userCpuUtil << "%" << endl;
	*running->outFile << "Throughput: " << throughput << endl;
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
		os.schedule();
		os.run();
	} catch(const char* error) {
		printf("[Operating System Error] %s \n", error);
	}

	return 0;
}

//Old Code:

// int main(int argc, char *argv[])
// {
//	fAndF = argv[1]; //filesAndFolders
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
