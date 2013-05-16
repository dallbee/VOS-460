/**
 * Definition of the os class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version .1
 * @date 10 May, 2013
 * @file os.h
 */
#ifndef OS_H
#define OS_H

#include "vm/VirtualMachine.h"
#include "sys/assembler/Assembler.h"
#include <fstream>
#include <list>
#include <queue>

using namespace std;

class PCB {
public:
	PCB(string name);
	virtual ~PCB();
private:
	friend class OS;
	string name;

	// Program execution information
	short reg[VirtualMachine::regSize];
	short pc;
	short sr;
	short ir;
	short sp;
	short base;
	short limit;

	// Accounting Information
	unsigned tempClock;
	unsigned execTime;
	unsigned waitTime;
	float turnTime;
	unsigned ioTime;
	unsigned largestStack;

	// File Streams
	fstream *oFile;
	fstream *outFile;
	fstream *inFile;
	fstream *stFile;
};

class OS {
public:
	OS();
	virtual ~OS() {};
	void load();
	void schedule();
	void loadState();
	void saveState();
	void run();
	void processFinish();

private:
	VirtualMachine VM;
	list<PCB *> progs;
	queue<PCB *> readyQ, waitQ;
	PCB* active;
	short exitCode;

	//System Information
	int startClock;
	int waitClock;
	int userTotal;
	int idleTotal;

	fstream osOut;
	fstream processStack;

	int asLine;
	short limit;
	string programAs;
};

#endif
