/**
 * Implementation of the VirtualMachine class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.0
 * @date 28 April, 2013
 * @file VirtualMachine.cpp
 */

#include "VirtualMachine.h"
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <stdio.h>
using namespace std;

/**
 * Construct object and create registers and memory
 */
 VirtualMachine::VirtualMachine(string file, short (&memory)[memSize],short progLimit):
	mem(memory), clock(), fileName(), OP(), RD(), I(), RS(), ADDR(), CONST(),
	pc(), ir(), sp(memSize - 1), base(), limit(), sr()
 {
	instructions[0x00] = &VirtualMachine::loadExec;
	instructions[0x01] = &VirtualMachine::storeExec;
	instructions[0x02] = &VirtualMachine::addExec;
	instructions[0x03] = &VirtualMachine::addcExec;
	instructions[0x04] = &VirtualMachine::subExec;
	instructions[0x05] = &VirtualMachine::subcExec;
	instructions[0x06] = &VirtualMachine::andExec;
	instructions[0x07] = &VirtualMachine::xorExec;
	instructions[0x08] = &VirtualMachine::complExec;
	instructions[0x09] = &VirtualMachine::shlExec;
	instructions[0x0A] = &VirtualMachine::shlExec;
	instructions[0x0B] = &VirtualMachine::shrExec;
	instructions[0x0C] = &VirtualMachine::shraExec;
	instructions[0x0D] = &VirtualMachine::comprExec;
	instructions[0x0E] = &VirtualMachine::getstatExec;
	instructions[0x0F] = &VirtualMachine::putstatExec;
	instructions[0x10] = &VirtualMachine::jumpExec;
	instructions[0x11] = &VirtualMachine::jumplExec;
	instructions[0x12] = &VirtualMachine::jumpeExec;
	instructions[0x13] = &VirtualMachine::jumpgExec;
	instructions[0x14] = &VirtualMachine::callExec;
	instructions[0x15] = &VirtualMachine::returnExec;
	instructions[0x16] = &VirtualMachine::readExec;
	instructions[0x17] = &VirtualMachine::writeExec;
	instructions[0x18] = &VirtualMachine::haltExec;
	instructions[0x19] = &VirtualMachine::noopExec;

	fileName = file;
	limit = progLimit;
	memoryDump(limit);

	for(; pc != memSize and OP != 0x18;) {
		ir = mem[pc++];
		CONST = ir & 0xFF;
		ADDR = ir & 0xFF;
		RS = (ir >>= 6) & 0x03;
		I =  (ir >>= 2) & 0x01;
		RD = (ir >>= 1) & 0x03;
		OP = (ir >>= 2) & 0x1F;
		if (sp - 1 == pc) {
			throw "Out of memory, stack pointer and program counter collision";
		}
		if (OP > 0x19) {
			throw "Unknown operation";
		}

		(this->*instructions[OP])();\
		machineDump();
	}
}

/**
 * Dumps the VirtualMachine's contents so they can be read
 *
 */
 void VirtualMachine::machineDump() {
 	printf("==================================================\n");
 	printf("Program Counter: \t %u\n", pc-1);
	printf("Stack Pointer: \t %u\n", sp);
	printf("Clock: \t \t %u\n", clock);
 	printf("CONST:%u ADDR:%u RS:%u I:%u RD:%u OP:%X\n", CONST, ADDR, RS, I, RD, OP );
	printf("Filename:%s \n", fileName.c_str());
	printf("statusReg:%X \n", sr);
	for(int i = 0; i < regSize; ++i) {
		printf("Register[%u] \t %u \n", i, reg[i] & 0xFFFF );
	}
	for(int i = memSize-1; i > sp; --i) {
		printf("Stack[%u] \t %u \n", i, mem[i] & 0xFFFF );
	}
 }

/**
 * Dumps the VirtualMachine's mem[] contents so they can be read
 * This is separate because it is much longer than the machineDump()
 *
 * @param memInt Controls how much memory will be printed.
 */
 void VirtualMachine::memoryDump(short memInt){
 		for(int i = 0; i < memSize and i < memInt; ++i) {
		printf("Memory[%u] \t %u \n", i, mem[i] & 0xFFFF );
	}
 }

/**
 * Pushes the input data to the stack, and increments the sp
 *
 * @param pcbItem Process Control Block item
 */
inline void VirtualMachine::pushStack(short pcbItem)
{
	mem[sp--] = pcbItem;
}

/**
 * Decrements the sp, then pops out the data from the stack
 *
 * @return The top of the memory stack.
 */
inline short VirtualMachine::popStack()
{
	return mem[++sp];
}

/**
 * Determines to set carry in status register or not
 */
inline void VirtualMachine::setCarry()
{
	sr &= 0x0000;
	sr = (reg[RD] & 0x10000) ? (sr | 1) : sr ;
}

/**
 * Determines to set carry in status register or not
 * This one is for right shift bits, that would go too far.
 */
inline void VirtualMachine::setCarryRight()
{
	sr &= 0x0000;
	sr = (reg[RD] & 0x0001) ? (sr | 1) : sr ;
}

/**
 * Sets 'greater than' bit in status register
 * while clearing equal and less than bits
 */
inline void VirtualMachine::setGreater()
{
	sr = (sr & 0x0002) | 2;
}

/**
 * Sets 'equal to' bit in status register
 * while clearing less and greater than bits
 */
inline void VirtualMachine::setEqual()
{
	sr = (sr & 0x0004) | 4;
}

/**
 * Sets 'less than' bit in status register
 * while clearing equal and greater than bits
 */
inline void VirtualMachine::setLess()
{
	sr = (sr & 0x0008) | 8;
}

/**
 * Sets the 'overflow' bit in status register
 */
inline void VirtualMachine::setOverflow()
{
	sr |= 16;
}

/**
 * Checks for 'Carry' bit in status register
 *
 * @return The carry bit value
 */
inline int VirtualMachine::getCarry()
{
	return (sr & 1) != 0;
}

/**
 * Checks for 'Greater than' bit in status register
 *
 * @return The greater than bit value
 */
inline int VirtualMachine::getGreater()
{
	return (sr & 2) != 0;
}

/**
 * Checks for 'Equal to' bit in status register
 *
 * @return The equal bit value
 */
inline int VirtualMachine::getEqual()
{
	return (sr & 4) != 0;
}

/**
 * Checks for 'Less than' bit in status register
 *
 * @return The less than bit value
 */
inline int VirtualMachine::getLess()
{
	return (sr & 8) != 0;
}

/**
 * Checks for 'Less than' bit in status register
 *
 * @return The overflow bit value
 */
inline int VirtualMachine::getOverflow()
{
	return (sr & 16) != 0;
}

/**
 * Loads constant if immediate (I) is high
 * or loads from memory based on address if I is low
 */
void VirtualMachine::loadExec()
{
	reg[RD] = I ? CONST : mem[ADDR];
	++clock;
}

/**
 * Stores the register into the specified memory address
 */
void VirtualMachine::storeExec()
{
	mem[ADDR] = reg[RD];
	++clock;
}

/**
 * Adds constant if immediate (I) is high
 * or adds value register if I is low
 */
void VirtualMachine::addExec()
{
	reg[RD] += I ? CONST : reg[RS];
	setCarry();
	++clock;
}

/**
 * Adds constant if immediate (I) is high
 * or add register's value if I is low
 * also adds 1 if the carry bit is high.
 */
void VirtualMachine::addcExec()
{
	// I'm not sure that this will work with my signed char trick for
	// sign extension, Need to test.
	reg[RD] += I ? (CONST + getCarry()) : (reg[RS] + getCarry());
	setCarry();
	++clock;
}

/**
 * Subtracts constant if immediate (I) is high
 * or subtracts register's value if I is low
 */
void VirtualMachine::subExec()
{
	reg[RD] -= I ? CONST : reg[RS];
	setCarry();
	++clock;
}


/**
 * Subtracts constant if immediate (I) is high
 * or subtracts register's value if I is low
 * also subtracts 1 if the carry bit is high.
 */
void VirtualMachine::subcExec()
{
	// I'm not sure that this will work with my signed char trick for
	// sign extension, Need to test.
	reg[RD] -= I ? (CONST - getCarry()) : (reg[RS] - getCarry());
	setCarry();
	++clock;
}

/**
 * Bitwise AND's constant if immediate (I) is high
 * or AND's register's value if I is low
 */
void VirtualMachine::andExec()
{
	reg[RD] &= I ? CONST : reg[RS];
	++clock;
}

/**
 * Bitwise XOR's constant if immediate (I) is high
 * or XOR's register's value if I is low
 */
void VirtualMachine::xorExec()
{
	reg[RD] ^= I ? CONST : reg[RS];
	++clock;
}

/**
 * Creates Compliment (opposite bits) of reg[RD]
 */
void VirtualMachine::complExec()
{
	reg[RD] = ~reg[RD];
	++clock;
}

/**
 * Shifts bits left by 1
 */
void VirtualMachine::shlExec()
{
	reg[RD] <<= 1;
	setCarry();
	++clock;
}

/**
 * Shifts bits right by 1
 */
void VirtualMachine::shrExec()
{
	setCarryRight();
	reg[RD] = (unsigned)reg[RD] >> 1;
	++clock;
}

/**
 * Arithmetically shifts bits left by 1
 * which means it must check the Most Significant Bit (MSB) and
 * make sure the next MSB is the same
 */
void VirtualMachine::shraExec()
{
	setCarryRight();
	reg[RD] >>= 1;
	++clock;
}

/**
 * Compares constant if immediate (I) is high
 * or Compares register's value if I is low
 * Then it sets the appropriate sr bitmask
 */
void VirtualMachine::comprExec()
{
	int compareValue = I ? CONST : reg[RS];

	if (reg[RD] < compareValue) {
		setLess();
	} else if (reg[RD] == compareValue) {
		setEqual();
	} else {
		setGreater();
	}

	++clock;
}

/**
 * Sets the register equal to the sr
 */
void VirtualMachine::getstatExec()
{
	reg[RD] = sr;
	++clock;
}

/**
 * Sets the sr equal to the register
 */
void VirtualMachine::putstatExec()
{
	sr = reg[RD];
	++clock;
}

/**
 * Sets the program counter to the specified address
 */
void VirtualMachine::jumpExec()
{
	pc = ADDR;
	++clock;
}

/**
 * If the 'Less than' bit is high in the sr
 * Sets the program counter to the specified address
 */
void VirtualMachine::jumplExec()
{
	pc = getLess() ? ADDR : pc;
	++clock;
}

/**
 * If the 'Equal to' bit is high in the sr
 * Sets the program counter to the specified address
 */
void VirtualMachine::jumpeExec()
{
	pc = getEqual() ? ADDR : pc;
	++clock;
}


/**
 * If the 'Greater than' bit is high in the sr
 * Sets the program counter to the specified address
 */
void VirtualMachine::jumpgExec()
{
	pc = getGreater() ? ADDR : pc;
	++clock;
}

/**
 * Jumps to a new address, and stores the current Process Control Block
 */
void VirtualMachine::callExec()
{
	pushStack(pc);
	pushStack(sr);
	pushStack(reg[0]);
	pushStack(reg[1]);
	pushStack(reg[2]);
	pushStack(reg[3]);
	pc = ADDR;
	clock += 4;
}

/**
 * Returns to the previous state of the Process Control Block
 */
void VirtualMachine::returnExec()
{
	reg[3] = popStack();
	reg[2] = popStack();
	reg[1] = popStack();
	reg[0] = popStack();
	sr = popStack();
	pc = popStack();
	clock += 4;
}

/**
 * Reads in data from a ".in" file to a register
 */
void VirtualMachine::readExec()
{
	fileName = (fileName.substr(0, fileName.find_last_of("."))).append(".in");
	ifstream inputFile(fileName.c_str());
	inputFile >> reg[RD];
	clock += 28;
}

/**
 * Saves/Writes out a register's data to a ".out" file
 */
void VirtualMachine::writeExec()
{
	fileName = (fileName.substr(0, fileName.find_last_of("."))).append(".out");
	ofstream outputFile(fileName.c_str());
	outputFile << reg[RD];
	clock += 28;
}

/**
 * Halts the VirtualMachine
 */
void VirtualMachine::haltExec()
{
	pc = memSize;
	++clock;
}

/**
 * Only increments the CPU clock
 */
void VirtualMachine::noopExec()
{
	++clock;
}
