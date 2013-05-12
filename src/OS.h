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

using namespace std;

class PCB{
public:
	PCB();
	friend class VirtualMachine;
private:
	PCB(string fileName, int tempBase, int tempLimit);
	string fileName;
	short reg[VirtualMachine::regSize];
	short pc;
	short ir;
	short sp;
	short base;
	short limit;
	short sr;
//	string input;
//	string output;
	string stack;
	string errorMessage;

	//Process Specific
	int execTime;
	int waitTime;
	int turnTime;
	int ioTime;
	int LargestStack;



};

class OS{
public:
	OS();
	VirtualMachine VM;
	Assembler as;
	void run();
	void load();
	void loadState();
	void saveState();
	void scheduler();
	void processFinish();
	void finish();


private:
	list<PCB *>  jobs;
	queue<PCB *> readyQ, waitQ;
	PCB* running;
	int osClock;
	short tempBase;
	short tempLimit;
	short exitCode;

	//System Information
	int userTotal;
	int idleTotal;
	int systemCpuUtil;
	int userCpuUtil;
	int throughput;

	fstream osOutput;
	fstream processStack;
	//pcb is supposed to be private, then use ?


	int asLine;
	int Limit;
	string fAndF;
	string programAs;
	short mem[VirtualMachine::memSize]
};

#endif
