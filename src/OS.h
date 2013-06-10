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
	PageTable* pageTable;

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
	int pageFaults;
	int pageHits;

	// File Streams
	fstream *oFile;
	ofstream *outFile;
	fstream *inFile;
	fstream *stFile;
};

class OS {
public:
	OS(bool mode);
	virtual ~OS() {};
	void load();
	void schedule();
	void loadState();
	void saveState();
	void pageReplace();
	void pageSave();
	void run();
	void processFinish();

private:
	VirtualMachine VM;
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
	bool pageAlgorithm;
};

#endif
