/**
 * Implementation of the os.cpp
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.0
 * @date 29 April, 2013
 * @file os.cpp
 */

//TO DO:
 //fill in case statement's code


#include "OS.h"
#include "sys/assembler/Assembler.h"
#include "vm/VirtualMachine.h"
#include <cstdlib>
#include <fstream>
#include <list>
#include <queue>
#include <sstream>
#include <stdio.h>
#include <string>
using namespace std;

/**
 * Creates a PCB object
 * Responsible for setting up the main file streams for the entire system
 */
PCB::PCB(string fileName) : name(fileName), reg(), pc(), sr(), ir(),
	sp(VirtualMachine::memSize - 1), base(), limit(), tempClock(), execTime(),
	waitTime(), turnTime(), ioTime(), largestStack(),
	oFile(new fstream(string("../io/" + name + "/" + name + ".o").c_str())),
	outFile(new ofstream(string("../io/" + name + "/" + name + ".out").c_str())),
	inFile(new fstream(string("../io/" + name + "/" + name + ".in").c_str())),
	stFile(new fstream())
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
OS::OS() : VM(), progs(), readyQ(), waitQ(), active(), exitCode(), userTotal(),
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

	Assembler as("../src/sys/assembler/opcodes.lst");
	for (string line; getline(progFile, line);) {
		string name = line.substr(line.find_last_of("/") + 1,
		    line.find_last_of(".") - line.find_last_of("/") - 1);

		PCB *pcb = new PCB(name);

		// Assemble opcodes
		try {
			as.build(string(line));
		} catch(const char* error) {
			printf("[Assembler Error] %s \n", error);
		}

		// Load object code into memory
		pcb->base = limit;
		pcb->pc = limit;

		int assLine = 0;
	  	*pcb->oFile >> assLine;
		while(!((*pcb->oFile).eof())){
	 		VM.mem[limit++] = assLine;
	 		*pcb->oFile >> assLine;
	 	}
		pcb->limit = limit;
		printf("%s:\tpc:%ib:%i\tl:%i\n", pcb->name.c_str(), pcb->pc, pcb->base, pcb->limit);
		progs.push_back(pcb);
		readyQ.push(pcb);
	}
	for(int i = 0; i < VM.memSize; ++i) {
		printf("Memory[%u] \t %u \n", i, VM.mem[i] & 0xFFFF );
	}
}

/**
 * Organizes the Wait and Ready queues, and sets up the appropriate PCB to run.
 */
void OS::schedule()
{
	VM.clock += 5;

	for(unsigned i = 0; i < waitQ.size(); ++i) {
		if((waitQ.front()->tempClock + 27) <= VM.clock) {
			waitQ.front()->tempClock = VM.clock;
			readyQ.push(waitQ.front());
			waitQ.pop();
		} else {
			waitQ.push(waitQ.front());
			waitQ.pop();
		}
	}

	if(readyQ.size()) {
		active = readyQ.front();
		readyQ.pop();
	} else {
		active = NULL;
	}
}

/*
 * Brings a new PCB into the Virtual Machine
 */
void OS::loadState()
{
	printf("loadState() called\n");
	active->waitTime += (VM.clock - active->tempClock);
	active->tempClock = VM.clock;
	copy(&active->reg[0], &active->reg[VM.regSize], VM.reg);
	VM.pc = active->pc;
	VM.sp = active->sp;
	VM.sr = active->sr;
	VM.base = active->base;
	VM.limit = active->limit;
	VM.inFile = active->inFile;
	VM.outFile = active->outFile;
	VM.largestStack = active->largestStack;

	string line = "";

	active->stFile->open(string("../io/" + active->name + "/" +
	                      active->name + ".st").c_str(), ios::in);

	for (int i = VM.memSize; getline(*(active->stFile), line); --i) {
		stringstream convert(line);
		convert >> VM.mem[i] ;
	}
	active->stFile->close();
}

/*
 * Saves the current state of the Virtual Machine into a PCB.
 */
void OS::saveState()
{
	printf("saveState() called\n");
	active->tempClock = VM.clock;
	copy(&VM.reg[0], &VM.reg[VM.regSize], active->reg);
	active->pc = VM.pc;
	active->sp = VM.sp;
	active->sr = VM.sr;
	active->base = VM.base;
	active->limit = VM.limit;
	active->inFile = VM.inFile;
	active->outFile = VM.outFile;
	active->largestStack = VM.largestStack;

	active->stFile->open(string("../io/" + active->name + "/" + active->name +
	                      ".st").c_str(), ios::out | ios::trunc);
	
	for (int i = VM.sp; i < VM.memSize - 1; ++i) {
		*active->stFile << VM.mem[i] << endl;
	}
	active->stFile->close();
}

/*
 * Executes the VM and handles its return statuses
 */
void OS::run()
{
	while(active or waitQ.size()) {

		if ( ! active and waitQ.size()) {
			VM.clock += 17;// noop until waitQ is ready.
			schedule();// Then run scheduler
		}

		loadState();
		printf("==================================================\n%s\n",active->name.c_str() );

		VM.run();
		active->execTime += (VM.clock - active->tempClock);
		switch((active->sr >> 5) & 7) { //Looks only at the 3 VM return status bits
			// Time slice
			case 0:
				saveState();
				readyQ.push(active);
				break;

			// Halt
			case 1:
				processFinish();
				break;

			// Reference out of bounds
			case 2:
				printf("Virtual Machine: Reference out of bounds\n");
				processFinish();
				break;

			// Stack Overflow
			case 3:
				printf("Virtual Machine: Stack overflow!\n");
				processFinish();
				break;

			// Stack Underflow
			case 4:
				printf("Virtual Machine: Stack underflow\n");
				processFinish();
				break;

			// Invalid Opcode
			case 5:
				printf("Virtual Machine: Invalid Opcode\n");
				processFinish();
				break;

			// Read Operation
			case 6:
				active->ioTime += 27;
				saveState();
				waitQ.push(active);
				break;

			// Write Operation
			case 7:
				active->ioTime += 27;
				saveState();
				waitQ.push(active);
				break;
		}
		schedule();
	}
}

/*
 * Generates accounting information about the process, removes the PCB, and
 * outputs that information to a file.
 */
void OS::processFinish()
{
	printf("%s FINISHED! \n", active->name.c_str());
	int systemTime = 0;
	float systemCpuUtil = ((float)(VM.clock - idleTotal)/(float)VM.clock)*100.0;
	float userCpuUtil = ((float)userTotal / (float)VM.clock) * 100.0;
	float throughput = (float)progs.size() / (float)VM.clock / 10000.0;

	active->turnTime = (active->execTime + active->waitTime + active->ioTime)
		/ 10000.0;

	*active->outFile << endl << "[Process Information]" << endl;
	*active->outFile << "CPU Time: " << active->execTime << endl;
	*active->outFile << "Waiting Time: " << active->waitTime << endl;
	*active->outFile << "Turnaround Time: " << active->turnTime << endl;
	*active->outFile << "I/O Time: " << active->ioTime << endl;
	*active->outFile << "Largest Stack Size: " << active->largestStack << endl;

	*active->outFile << endl << "[System Information]" << endl;
	*active->outFile << "System Time: " << systemTime << endl;
	*active->outFile << "System CPU Util: " << systemCpuUtil << "%" << endl;
	*active->outFile << "User CPU Util: " << userCpuUtil << "%" << endl;
	*active->outFile << "Throughput: " << throughput << endl;

	remove(string("../io/" + active->name + "/" + active->name +".st").c_str());
	delete active;
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
