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
 VirtualMachine::VirtualMachine():
	mem(), clock(), OP(), RD(), I(), RS(), ADDR(), CONST(), pc(), ir(),
	sp(memSize - 1), base(), limit(), sr()
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
}

/**
 * Dumps the VirtualMachine's contents so they can be read
 */
 void VirtualMachine::machineDump()
 {
 	printf("==================================================\n");
 	printf("Program Counter: \t %u\n", pc);
	printf("Stack Pointer: \t %u\n", sp);
	printf("Clock: \t \t %u\n", clock);
 	printf("CONST:%u ADDR:%u RS:%u I:%u RD:%u OP:%X\n", CONST, ADDR, RS, I, RD, OP );
	printf("statusReg:%X \n", sr);
	for(int i = 0; i < regSize; ++i) {
		printf("Register[%u] \t %u \n", i, reg[i] & 0xFFFF );
	}
	for(int i = memSize-1; i > sp; --i) {
		printf("Stack[%u] \t %u \n", i, mem[i] & 0xFFFF );
	}
 }

/**
 * Run the virtual machine. Sets the status register upon end of time slice and
 * other return conditions.
 */
void VirtualMachine::run()
{//machineDump();
	sr &= 0xFF1F;
	for(int timeslice = 0; timeslice < 15 and !(sr & 0xE0); ++timeslice) {
		machineDump();
		ir = mem[pc++];
		CONST = ir & 0xFF;
		ADDR = ir & 0xFF;
		RS = (ir >>= 6) & 0x03;
		I = (ir >>= 2) & 0x01;
		RD = (ir >>= 1) & 0x03;
		OP = (ir >>= 2) & 0x1F;

		(this->*instructions[OP])();
		++clock;
		if (sp > largestStack){
			largestStack = sp;
		}

		if (pc > limit or pc < base) { // Reference out of bounds
			sr = (sr & 0xFF5F) | 64;
		}
		else if (sp - 1 == pc) { // Stack Overflow
			sr = (sr & 0xFF7F) | 96;
		}
		else if (sp == 256) { // Stack Underflow
			sr = (sr & 0xFF9F) | 128;
		}
		else if (OP > 0x19) { // Invalid Opcode
			sr = (sr & 0xFFAF) | 160;
		}
		else if (OP == 0x18) { // Halt
			sr = (sr & 0xFF3F) | 32;
		}
		else if (OP == 0x16) { // Read Operations
			sr = (sr & 0xFFCF) | 192;
		}
		else if (OP == 0x17) { // Write Operation
			sr |= 224;
		}
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
inline void VirtualMachine::setCarry(int value)
{
	sr &= 0xFFFE;
	sr = (value & 0x10000) ? (sr | 1) : sr;
}

/**
 * Determines to set carry in status register or not
 * This one is for right shift bits, that would go too far.
 */
inline void VirtualMachine::setCarryRight()
{
	sr &= 0xFFFE;
	sr = (reg[RD] & 0x0001) ? (sr | 1) : sr;
}

/**
 * Sets 'greater than' bit in status register
 * while clearing equal and less than bits
 */
inline void VirtualMachine::setGreater()
{
	sr = (sr & 0xFFFD) | 2;
}

/**
 * Sets 'equal to' bit in status register
 * while clearing less and greater than bits
 */
inline void VirtualMachine::setEqual()
{
	sr = (sr & 0xFFFB) | 4;
}

/**
 * Sets 'less than' bit in status register
 * while clearing equal and greater than bits
 */
inline void VirtualMachine::setLess()
{
	sr = (sr & 0xFFF7) | 8;
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
	clock += 3;
}

/**
 * Stores the register into the specified memory address
 */
void VirtualMachine::storeExec()
{
	mem[ADDR] = reg[RD];
	clock += 3;
}

/**
 * Adds constant if immediate (I) is high
 * or adds value register if I is low
 */
void VirtualMachine::addExec()
{
	int value = I ? reg[RD] + CONST : reg[RD] + reg[RS];
	reg[RD] = value & 0xFFFF;
	setCarry(value);
}

/**
 * Adds constant if immediate (I) is high
 * or add register's value if I is low
 * also adds 1 if the carry bit is high.
 */
void VirtualMachine::addcExec()
{
	int carry = getCarry();
	int value = I ? reg[RD] + CONST + carry: reg[RD] + reg[RS] + carry;
	reg[RD] = value & 0xFFFF;
	setCarry(value);
}

/**
 * Subtracts constant if immediate (I) is high
 * or subtracts register's value if I is low
 */
void VirtualMachine::subExec()
{
	int value = I ? reg[RD] - CONST : reg[RD] - reg[RS];
	reg[RD] = value & 0xFFFF;
	setCarry(value);
}


/**
 * Subtracts constant if immediate (I) is high
 * or subtracts register's value if I is low
 * also subtracts 1 if the carry bit is high.
 */
void VirtualMachine::subcExec()
{
	int carry = getCarry();
	int value = I ? reg[RD] - CONST - carry : reg[RD] - reg[RS] - carry;
	reg[RD] = value & 0xFFFF;
	setCarry(value);
}

/**
 * Bitwise AND's constant if immediate (I) is high
 * or AND's register's value if I is low
 */
void VirtualMachine::andExec()
{
	reg[RD] &= I ? CONST : reg[RS];
}

/**
 * Bitwise XOR's constant if immediate (I) is high
 * or XOR's register's value if I is low
 */
void VirtualMachine::xorExec()
{
	reg[RD] ^= I ? CONST : reg[RS];
}

/**
 * Creates Compliment (opposite bits) of reg[RD]
 */
void VirtualMachine::complExec()
{
	reg[RD] = ~reg[RD];
}

/**
 * Shifts bits left by 1
 */
void VirtualMachine::shlExec()
{
	int value = reg[RD] << 1;
	reg[RD] = value & 0xFFFF;
	setCarry(value);
}

/**
 * Shifts bits right by 1
 */
void VirtualMachine::shrExec()
{
	setCarryRight();
	reg[RD] = (unsigned)reg[RD] >> 1;
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

}

/**
 * Sets the register equal to the sr
 */
void VirtualMachine::getstatExec()
{
	reg[RD] = sr;
}

/**
 * Sets the sr equal to the register
 */
void VirtualMachine::putstatExec()
{
	sr = reg[RD];
}

/**
 * Sets the program counter to the specified address
 */
void VirtualMachine::jumpExec()
{
	pc = ADDR;
}

/**
 * If the 'Less than' bit is high in the sr
 * Sets the program counter to the specified address
 */
void VirtualMachine::jumplExec()
{
	pc = getLess() ? ADDR : pc;
}

/**
 * If the 'Equal to' bit is high in the sr
 * Sets the program counter to the specified address
 */
void VirtualMachine::jumpeExec()
{
	pc = getEqual() ? ADDR : pc;
}


/**
 * If the 'Greater than' bit is high in the sr
 * Sets the program counter to the specified address
 */
void VirtualMachine::jumpgExec()
{
	pc = getGreater() ? ADDR : pc;
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
	clock += 3;
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
	clock += 3;
}

/**
 * Reads in data from a ".in" file to a register
 */
void VirtualMachine::readExec()
{
	*inFile >> reg[RD];
}

/**
 * Saves/Writes out a register's data to a ".out" file
 */
void VirtualMachine::writeExec()
{
	*outFile << reg[RD];
}
