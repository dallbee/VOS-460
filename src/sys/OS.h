/**
 * Definition of the Operating System class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.0
 * @date 29 April, 2013
 * @file OS.h
 */

#ifndef OS_H
#define OS_H

#include "../vm/VirtualMachine.h"
using namespace std;

class OS {
public:
	OS();

private:
	short mem[VirtualMachine::memSize];
};

#endif
