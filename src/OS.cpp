/**
 * Implementation of the os.cpp
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.0
 * @date May 17, 2013
 * @file os.cpp
 */

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
#include <iterator>
using namespace std;

/**
 * Creates a PCB object
 * Responsible for setting up the main file streams for the entire system
 */
PCB::PCB(string fileName) : name(fileName), pageTable(), reg(), pc(), sr(), ir(),
	sp(VirtualMachine::memSize - 1), base(), limit(), tempClock(), execTime(),
	waitTime(), turnTime(), ioTime(), largestStack(VirtualMachine::memSize - 1),
	pageFaults(), pageHits(),
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
	delete pageTable;
}

/**
 * Creates an OS object
 */
OS::OS(bool mode) : VM(), progs(), readyQ(), waitQ(), active(), exitCode(),
	systemTime(), systemCpuUtil(), userCpuUtil(), throughput(), idleTotal(),
	osOut(), processStack(), asLine(), limit(), programAs(), pageAlgorithm(mode)
{
}

/*
 * Reads and assembles all programs in the io directory.
 */
void OS::load()
{
	// Copies the paths of all program files in the io directory to progs file
	if (system("ls -d ../io/*/*.s > progs")) {
		throw "Error while attempting to get program listing";
	}

	ifstream progFile("progs");

	Assembler as("../src/sys/assembler/opcodes.lst");
	for (string line; getline(progFile, line);) {
		string name = line.substr(line.find_last_of("/") + 1,
		    line.find_last_of(".") - line.find_last_of("/") - 1);

		PCB *pcb = new PCB(name);
		pcb->pageTable = new PageTable(VM.frames, VM.clock, pageAlgorithm);

		// Assemble opcodes
		try {
			as.build(string(line));
		} catch(const char* error) {
			printf("[Assembler Error] %s \n", error);
		}

		progs.push_back(pcb);

		if (readyQ.size() <= 5) {
			readyQ.push(pcb);
		}
	}
	/*
	for(int i = 0; i < VM.memSize; ++i) {
		printf("Memory[%u] \t %u \n", i, VM.mem[i] & 0xFFFF );
	}
	*/
}

/**
 * Organizes the Wait and Ready queues, and sets up the appropriate PCB to run.
 */
void OS::schedule()
{
	for(unsigned i = 0; i < waitQ.size(); ++i) {
		if((waitQ.front()->tempClock) <= VM.clock) {
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
	active->waitTime += (VM.clock - active->tempClock);

	copy(&active->reg[0], &active->reg[VM.regSize], VM.reg);
	VM.pc = active->pc;
	VM.sp = active->sp;
	VM.sr = active->sr;
	VM.base = active->base;
	VM.limit = active->limit;
	VM.inFile = active->inFile;
	VM.outFile = active->outFile;
	VM.largestStack = active->largestStack;
	VM.mem.hits = 0;

	string line = "";

	active->stFile->open(string("../io/" + active->name + "/" +
	                      active->name + ".st").c_str(), ios::in);

	while (getline(*(active->stFile), line)) {
		stringstream convert(line);
		convert >> VM.mem[VM.sp--];
	}

	active->stFile->close();
	pageReplace();
}

/*
 * Saves the current state of the Virtual Machine into a PCB.
 */
void OS::saveState()
{
	copy(&VM.reg[0], &VM.reg[VM.regSize], active->reg);
	active->pc = VM.pc;
	active->sp = VM.memSize - 1;
	active->sr = VM.sr;
	active->base = VM.base;
	active->limit = VM.limit;
	active->inFile = VM.inFile;
	active->outFile = VM.outFile;
	active->largestStack = VM.largestStack;
	active->pageHits = VM.mem.hits;

	active->stFile->open(string("../io/" + active->name + "/" + active->name +
	                      ".st").c_str(), ios::out | ios::trunc);

	for (int i = VM.memSize - 1; i > VM.sp; --i) {
		*active->stFile << VM.mem[i] << endl;
	}

	active->stFile->close();
}

void OS::pageReplace()
{
	if ((active->sr >> 5) & 0x27 == 32) {
		// FIFO
		if (pageAlgorithm) {
			// ADD FIFO COUNTER
			// DO FIFO / URL ALGORITHMS
		// LRU
		} else {
			unsigned temp = -1;
			unsigned value = 0;
			for (int i = 0; i < 32; ++i) {
				if (VM.pageTable.table[i] < temp) {
					temp = VM.pageTable.table[i];
					value = i;
				}
			}
		}

		// If dirty bit isn't set, don't bother saving!
		if (VM.pageTable.table[i] & 2) {
			// Inverse Page Table -> File Save
			/*
			string line;
			active->oFile->open(string("../io/" + active->name + "/" +
			                      active->name + ".o").c_str(), ios::in);

			while (getline(*(active->oFile), line)) {
				//stringstream convert(line);
				//convert >> VM.mem[VM.sp--];
			}

			active->stFile->close();*/
		}

		// Grab new from disk
		// 
		
		// FIFO counter
		if (pageAlgorithm) {
			
		}
		
	}
	
	VM.mem.refresh();
}

/*
 * Executes the VM and handles its return statuses
 */
void OS::run()
{
	while(active or waitQ.size()) {

		if ( ! active and waitQ.size()) {
			VM.clock += 17;// noop until waitQ is ready.
			unsigned leastWait = -1;
			leastWait >>= 1;
			for(unsigned i = 0; i < waitQ.size(); ++i) {
				if (waitQ.front()->tempClock - VM.clock < leastWait){
					leastWait = waitQ.front()->tempClock - VM.clock;
				}
				waitQ.push(waitQ.front());
				waitQ.pop();
				VM.clock += leastWait;
				systemTime += leastWait;
				idleTotal += leastWait;
			}
			schedule();// Then run scheduler
		}

		loadState();
		VM.run();
		active->execTime += (VM.clock - active->tempClock);

		switch((active->sr >> 5) & 0x27) { // Status bits + Page Fault
			// Time slice
			case 0:
				saveState();
				readyQ.push(active);
				VM.clock += 5; //Cost of context switch
				systemTime += 5; //Cost of context switch
				break;

			// Halt
			case 1:
				processFinish();
				break;

			// Reference out of bounds
			case 2:
				printf("Virtual Machine: Reference out of bounds\n");
				printf("Culprit: %s, ", active->name.c_str());
				saveState();
				break;

			// Stack Overflow
			case 3:
				printf("Virtual Machine: Stack overflow!\n");
				printf("Culprit: %s \n", active->name.c_str());
				saveState();
				break;

			// Stack Underflow
			case 4:
				printf("Virtual Machine: Stack underflow\n");
				printf("Culprit: %s \n", active->name.c_str());
				saveState();
				break;

			// Invalid Opcode
			case 5:
				printf("Virtual Machine: Invalid Opcode\n");
				printf("Culprit: %s \n", active->name.c_str());
				saveState();
				break;

			// Read Operation
			case 6:
				active->tempClock = VM.clock +27;
				active->ioTime += 27;
				saveState();
				waitQ.push(active);
				break;

			// Write Operation
			case 7:
				active->tempClock = VM.clock + 27;
				active->ioTime += 27;
				saveState();
				waitQ.push(active);
				break;

			// Page Fault
			case 32:
				active->tempClock = VM.clock + 34;
				active->pageFaults++;
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

	systemCpuUtil = ((float)(VM.clock - idleTotal)/(float)VM.clock)*100.0;
	userCpuUtil = ((float)active->execTime / (float)VM.clock) * 100.0;
	throughput = (float)progs.size() / (float)VM.clock * 10000.0;
	active->largestStack = VM.memSize - active->largestStack - 1;

	*active->outFile << endl << "[Process Information]" << endl;
	*active->outFile << "CPU Time: " << active->execTime << " ticks" << endl;
	*active->outFile << "Waiting Time: " << active->waitTime<< " ticks" << endl;
	*active->outFile << "Turnaround Time: " << VM.clock << " ticks" << endl;
	*active->outFile << "I/O Time: " << active->ioTime << " ticks" << endl;
	*active->outFile << "Largest Stack Size: " << active->largestStack << endl;
	*active->outFile << "Page Faults: " << active->pageFaults << endl;
	*active->outFile << "Hit Ratio: " << (float)active->pageHits /
		(float)active->pageFaults << endl;

	*active->outFile << endl << "[System Information]" << endl;
	*active->outFile << "System Time: " << systemTime << " ticks" << endl;
	*active->outFile << "System CPU Util: " << systemCpuUtil << "%" << endl;
	*active->outFile << "User CPU Util: " << userCpuUtil << "%" << endl;
	*active->outFile << "Throughput: " << throughput << " Per Second" << endl;

	remove(string("../io/" + active->name + "/" + active->name +".st").c_str());
	delete active;

	if (readyQ.size() <= 5 and progs.size() >= 5) {
		list<PCB *>::iterator i = progs.begin();
		advance(i, 5);
		readyQ.push(*i);
	}
}

/**
 * Initializes the Operating system and boots.
 *
 * @return Failure status
 */
int main(int argc, char *argv[])
{
	string pageAlgorithm;

	if (argc == 2) {
		pageAlgorithm = argv[1];
	}

	if (pageAlgorithm != "-lru" and pageAlgorithm != "-fifo") {
		printf ("Please specify a page algorithm to use: -lru or -fifo\n");
		return 1;
	}

	bool mode = (pageAlgorithm == "-fifo") ? 1 : 0;

	try {
		OS os(mode);
		printf("initialized\n");
		os.load();
		printf("loaded\n");
		os.schedule();
		printf("scheduled\n");
		os.run();
		printf("finished\n");
	} catch(const char* error) {
		printf("[Operating System Error] %s \n", error);
	}

	return 0;
}
