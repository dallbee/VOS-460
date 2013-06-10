/**
 * Definition of the os class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 2.1
 * @date 15 May, 2013
 * @file os.h
 */
#ifndef OS_H
#define OS_H

#include "sys/assembler/Assembler.h"
#include "vm/VirtualMachine.h"
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
	//Process Info
	unsigned execTime;
	unsigned waitTime;
	float turnTime;
	unsigned ioTime;
	unsigned largestStack;
	float userTotal;

	// File Streams
	fstream *oFile;
	ofstream *outFile;
	fstream *inFile;
	fstream *stFile;
};

class Memory : public VirtualMachine {
public:
	Memory();
	virtual ~Memory() {};

	// [] Operator -> & Designates setter
	short operator [](int index) const;
    short & operator [](int index)

private:
	short page[32];
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
	Memory VM;
	list<PCB *> progs;
	queue<PCB *> readyQ, waitQ;
	PCB* active;
	short exitCode;

	//System Information
	int systemTime;
	float systemCpuUtil;
	float userCpuUtil;
	float throughput;
	int userTotal;
	int idleTotal;

	fstream osOut;
	fstream processStack;

	int asLine;
	short limit;
	string programAs;
};

#endif
