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
	friend class OS;
private:
	string name;

	// Program execution information
	short reg[VirtualMachine::regSize];
	short pc;
	short sr;
	short sp;
	short base;
	short limit;

	// Accounting Information
	unsigned execTime;
	unsigned waitTime;
	unsigned turnTime;
	unsigned ioTime;
	unsigned largestStack;

	// File Streams
	fstream o;
	fstream out;
	fstream in;
	fstream st;
};

class OS {
public:
	OS();
	//VirtualMachine VM;
	void run();
	void load();
	void loadState();
	void saveState();
	void scheduler();
	void processFinish();
	void finish();

private:
	list<PCB *> progs;
	queue<PCB *> readyQ, waitQ;
	PCB* running;
	int osClock;
	short exitCode;

	//System Information
	int userTotal;
	int idleTotal;
	int systemCpuUtil;
	int userCpuUtil;
	int throughput;

	fstream osOut;
	fstream processStack;
	//pcb is supposed to be private, then use ?

	int asLine;
	short limit;
	string programAs;
	short mem[VirtualMachine::memSize];
};

#endif
