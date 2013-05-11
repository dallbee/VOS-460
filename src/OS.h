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

class OS{
public:
	OS();
	friend class VirtualMachine;
private:

	//pcb is supposed to be private, then use ?
	PCB(string fileName, int tempBase, int tempLimit);
	queue<PCB *> readyQ, waitQ;
	list<PCB *>  jobs;
	PCB* running;
	int osClock;
	short tempBase;
	short tempLimit;


	int asLine;
	int Limit;
	string fAndF;
	string programAs;
	short mem[VirtualMachine::memSize]
};

#endif
