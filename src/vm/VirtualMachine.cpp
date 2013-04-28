#include "VirtualMachine.h"
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <stdio.h>


using namespace std;

/**
 * Construct object and create registers and memory
 *
 * @param
 */
 VirtualMachine::VirtualMachine(): clock(), fileName(), OP(), RD(), I(), RS(),
 	ADDR(), CONST(), pc(), ir(), sp(), base(), limit(), sr()
 {
 	fileName = "";

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
	instructions[0x0A] = &VirtualMachine::shlaExec;
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

	for(; pc != memSize;) {
		ir = mem[pc++];
		CONST = ir & 0xFF;
		ADDR = ir & 0x3F;
		RS = (ir >>= 6) & 0x03;
		I =  (ir >>= 2) & 0x01;
		RD = (ir >>= 1) & 0x03;
		OP = (ir >>= 5);
		(this->*instructions[OP])();
	}

	sp = memSize -1;
 }

/**
 * Dumps the VirtualMachine's contents so they can be read
 *
 * @return void
 */
 void VirtualMachine::machineDump() {
	for(int i = 0; i < memSize; ++i) {
		printf("Memory[%i] \t %i \n", i, mem[i] );
	}
	for(int i = 0; i < regSize; ++i) {
		printf("Register[%i] \t %i \n", i, reg[i] );
	}
	printf("Program Counter: \t %i \n", pc);
	printf("Stack Pointer: \t %i \n", sp);
	printf("Clock: \t \t %i \n", clock);
 }

/**
 * Pushes the input data to the stack, and increments the sp
 *
 * @param pcbItem Process Control Block item
 * @return void
 */
inline void VirtualMachine::pushStack(int pcbItem)
{
	mem[sp] = pcbItem;
	--sp;
}

/**
 * Decrements the sp, then pops out the data from the stack
 *
 * @return int
 */
inline int VirtualMachine::popStack()
{
	++sp;
	return mem[sp];
}

/**
 * Determines to set carry in status register or not
 *
 * @return void
 */
inline void VirtualMachine::setCarry()
{
	sr = (reg[RD] & 0x10000) ? (sr | 1) : (sr & 0xFFFE);
}

/**
 * Sets 'greater than' bit in status register
 * while clearing equal and less than bits
 *
 * @return void
 */
inline void VirtualMachine::setGreater()
{
	sr = (sr & 0xFFF3) | 2;
}

/**
 * Sets 'equal to' bit in status register
 * while clearing less and greater than bits
 *
 * @return void
 */
inline void VirtualMachine::setEqual()
{
	sr = (sr & 0xFFF5) | 4;
}

/**
 * Sets 'less than' bit in status register
 * while clearing equal and greater than bits
 *
 * @return void
 */
inline void VirtualMachine::setLess()
{
	sr = (sr & 0xFFF9) | 8;
}

/**
 * Sets the 'overflow' bit in status register
 *
 * @return void
 */
inline void VirtualMachine::setOverflow()
{
	sr |= 16;
}

/**
 * Checks for 'Carry' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
inline int VirtualMachine::getCarry()
{
	return (sr & 1) ? 1 : 0;
}

/**
 * Checks for 'Greater than' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
inline int VirtualMachine::getGreater()
{
	return (sr & 2) ? 1 : 0;
}

/**
 * Checks for 'Equal to' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
inline int VirtualMachine::getEqual()
{
	return (sr & 4) ? 1 : 0;
}

/**
 * Checks for 'Less than' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
inline int VirtualMachine::getLess()
{
	return (sr & 8) ? 1 : 0;
}

/**
 * Loads constant if immediate (I) is high
 * or loads from memory based on address if I is low
 *
 * @return void
 */
void VirtualMachine::loadExec()
{
	reg[RD] = I ? CONST : mem[ADDR];
	++clock;
}

/**
 * Stores the register into the specified memory address
 *
 * @return void
 */
void VirtualMachine::storeExec()
{
	mem[ADDR] = reg[RD];
	++clock;
}

/**
 * Adds constant if immediate (I) is high
 * or adds value register if I is low
 *
 * @return void
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
 *
 * @return void
 */
void VirtualMachine::addcExec()
{
	reg[RD] += I ? (CONST + getCarry()) : (reg[RS] + getCarry());
	setCarry();
	++clock;
}

/**
 * Subtracts constant if immediate (I) is high
 * or subtracts register's value if I is low
 *
 * @return void
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
 *
 * @return void
 */
void VirtualMachine::subcExec()
{

	reg[RD] -= I ? (CONST - getCarry()) : (reg[RS] - getCarry());
	setCarry();
	++clock;
}

/**
 * Bitwise AND's constant if immediate (I) is high
 * or AND's register's value if I is low
 *
 * @return void
 */
void VirtualMachine::andExec()
{

	reg[RD] &= I ? CONST : reg[RS];
	++clock;
}

/**
 * Bitwise XOR's constant if immediate (I) is high
 * or XOR's register's value if I is low
 *
 * @return void
 */
void VirtualMachine::xorExec()
{

	reg[RD] ^= I ? CONST : reg[RS];
	++clock;
}

/**
 * Creates Compliment (opposite bits) of reg[RD]
 *
 * @return void
 */
void VirtualMachine::complExec()
{

	reg[RD] = ~reg[RD];
	++clock;
}

/**
 * Shifts bits left by 1
 *
 * @return void
 */
void VirtualMachine::shlExec()
{

	reg[RD] = reg[RD] << 1;
	setCarry();
	++clock;
}

/**
 * Arithmetically shifts bits left by 1
 *
 * @return void
 */
void VirtualMachine::shlaExec()
{
	shlExec();
	++clock;
}

/**
 * Shifts bits right by 1
 *
 * @return void
 */
void VirtualMachine::shrExec()
{

	reg[RD] = reg[RD] >> 1;
	setCarry(); //I dont get why we would set carry on a right shift? -Taylor
	++clock;
}

/**
 * Arithmetically shifts bits left by 1
 * which means it must check the Most Significant Bit (MSB) and
 * make sure the next MSB is the same
 *
 * @return void
 */
void VirtualMachine::shraExec()
{


	//I may want to clean this up because I could just 'OR' it with an 'AND' of reg[RD]'s 15th bit.
	//But I'm not sure that I cleaner yet.
	reg[RD] = (reg[RD] & 0x8000) ? (reg[RD] >> 1) | 0x8000 : (reg[RD] >> 1) & 0x7FFF;
	//Needs to dublicate the MSB as it shifts
	// if (reg[RD] & 0x8000) {
	// 	reg[RD] = (reg[RD] >> 1) | 0x8000;
	// } else{
	// 	reg[RD] = (reg[RD] >> 1) & 0x7FFF;
	// }
	setCarry(); //I dont get why we would set carry on a right shift? -Taylor
	++clock;
}

/**
 * Compares constant if immediate (I) is high
 * or Compares register's value if I is low
 * Then it sets the appropriate sr bitmask
 *
 *
 * @return void
 */
void VirtualMachine::comprExec()
{

	int constOrReg = I ? CONST : reg[RS];
	(reg[RD] < constOrReg) ? setLess() : ((reg[RD] < constOrReg) ? setEqual() : setGreater());

	// if (reg[RD] < constOrReg) {
	// 	setLess();
	// } else if (reg[RD] == constOrReg) {
	// 	setEqual();
	// } else{
	// 	setGreater();
	// }
	++clock;
}

/**
 * Sets the register equal to the sr
 *
 * @return void
 */
void VirtualMachine::getstatExec()
{

	reg[RD] = sr;
	++clock;
}

/**
 * Sets the sr equal to the register
 *
 * @return void
 */
void VirtualMachine::putstatExec()
{

	sr = reg[RD];
	++clock;
}

/**
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpExec()
{

	pc = ADDR;
	++clock;
}

/**
 * If the 'Less than' bit is high in the sr
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumplExec()
{

	pc = getLess() ? ADDR : pc;
	++clock;
}

/**
 * If the 'Equal to' bit is high in the sr
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpeExec()
{

	pc = getEqual() ? ADDR : pc;
	++clock;
}


/**
 * If the 'Greater than' bit is high in the sr
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpgExec()
{

	pc = getGreater() ? ADDR : pc;
	++clock;
}

/**
 * Jumps to a new address, and stores the current Process Control Block
 *
 * @return void
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
 *
 * @return void
 */
void VirtualMachine::returnExec()
{
	pc = popStack();
	sr = popStack();
	reg[0] = popStack();
	reg[1] = popStack();
	reg[2] = popStack();
	reg[3] = popStack();
	clock += 4;
}

/**
 * Reads in data from a ".in" file to a register
 *
 * @return void
 */
void VirtualMachine::readExec()
{
	string inputFile = (fileName.substr(0, fileName.find_first_of("."))).append(".in");
	ifstream istream(inputFile.c_str());
	istream >> reg[RD];
	istream.close();
	clock += 28;
}

/**
 * Saves/Writes out a register's data to a ".out" file
 *
 * @return void
 */
void VirtualMachine::writeExec()
{
	string outputFile = (fileName.substr(0, fileName.find_first_of("."))).append(".out");
	ofstream ostream(outputFile.c_str());
	ostream << reg[RD];
	ostream.close();
	clock += 28;
}

/**
 * Halts the VirtualMachine
 *
 * @return void
 */
void VirtualMachine::haltExec()
{
	++clock;
}

/**
 * Only increments the CPU clock
 *
 * @return void
 */
void VirtualMachine::noopExec()
{
	++clock;
}
