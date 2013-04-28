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
 VirtualMachine::VirtualMachine ()
 {
	// int reg[regSize]={0};
 	// int mem[memSize]={0};
 	filename = "";
 	/*

 	string opCode, flags;
	ifstream opListFile(opListPath.c_str());
	istream_iterator<string> it (opListFile), end;

	// Load opcodes file into memory
	for (int i = 0; it != end; ++i, ++it) {
		opCode = it->substr(0, it->find(':'));
		flags = it->substr(it->find(':') + 1, it->length() - it->find(':'));

		// Build map of generic opCodes
		opCodes.insert(pair<string, int>(opCode, i));
	}

	// Verify data existence
	if (opCodes.empty()) {
		throw "Could not read data in from operation codes file";
	}
	*/
	instructions.reserve(32);
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

	for(; programCounter != memSize;) {
		instructionReg = mem[programCounter++];
		CONST = instructionReg & 0xFF;
		ADDR = instructionReg & 0x3F;
		RS = (instructionReg >>= 6) & 0x03;
		I =  (instructionReg >>= 2) & 0x01;
		RD = (instructionReg >>= 1) & 0x03;
		OP = (instructionReg >>= 5);
		(this->*instructions[OP])();
	}

	stackPointer = memSize -1;

 }

/**
 * Dumps the VirtualMachine's contents so they can be read
 *
 * @return void
 */
 void VirtualMachine::machineDump(){
	for(int i = 0; i < memSize; i++){ //memory
		printf("Memory[%i] \t %i \n", i, mem[i] );
	}
	for(int i = 0; i < regSize; i++){ //reg
		printf("Register[%i] \t %i \n", i, reg[i] );
	}
	printf("Program Counter: \t %i \n", programCounter);
	printf("Stack Pointer: \t %i \n", stackPointer);
	printf("Clock: \t \t %i \n", clock);
 }

/**
 * Increments the clock the input number of cycles
 *
 * @param cycles The number by which clock in incremented.
 * @return void
 */
void VirtualMachine::incrementClock(unsigned cycles){
	clock += cycles;
}

/**
 * Pushes the input data to the stack, and increments the stackPointer
 *
 * @param pcbItem Process Control Block item
 * @return void
 */
void VirtualMachine::pushStack(int pcbItem){
	mem[stackPointer] = pcbItem;
	stackPointer--;
}

/**
 * Decrements the stackPointer, then pops out the data from the stack
 *
 * @return int
 */
int VirtualMachine::popStack(){
	stackPointer++;
	return mem[stackPointer];
}

/**
 * Determines to set carry in status register or not
 *
 * @return void
 */
void VirtualMachine::setCarry(){

	//more readable if preferred: or just delete if statement
		//if turnary is preferred.
	// if(reg[RD] & 0x10000 ){
	// 	statusReg |= 1;
	// } else{
	// 	statusReg &= 0xFFFE;
	// }
	statusReg = (reg[RD] & 0x10000) ? (statusReg | 1) : (statusReg & 0xFFFE);
}

/**
 * Sets 'greater than' bit in status register
 * while clearing equal and less than bits
 *
 * @return void
 */
void VirtualMachine::setGreater(){
	statusReg = (statusReg & 0xFFF3) | 2;
}

/**
 * Sets 'equal to' bit in status register
 * while clearing less and greater than bits
 *
 * @return void
 */
void VirtualMachine::setEqual(){
	statusReg = (statusReg & 0xFFF5) | 4;
}

/**
 * Sets 'less than' bit in status register
 * while clearing equal and greater than bits
 *
 * @return void
 */
void VirtualMachine::setLess(){
	statusReg = (statusReg & 0xFFF9) | 8;
}

/**
 * Sets the 'overflow' bit in status register
 *
 * @return void
 */
void VirtualMachine::setOverflow(){
	statusReg |= 16;
}

/**
 * Checks for 'Carry' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
int VirtualMachine::getCarry(){
	return (statusReg & 1) ? 1 : 0;
}

/**
 * Checks for 'Greater than' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
int VirtualMachine::getGreater(){
	return (statusReg & 2) ? 1 : 0;
}

/**
 * Checks for 'Equal to' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
int VirtualMachine::getEqual(){
	return (statusReg & 4) ? 1 : 0;
}

/**
 * Checks for 'Less than' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
int VirtualMachine::getLess(){
	return (statusReg & 8) ? 1 : 0;
}

/**
 * Loads constant if immediate (I) is high
 * or loads from memory based on address if I is low
 *
 * @return void
 */
void VirtualMachine::loadExec()
{
	incrementClock(1);
	reg[RD] = I ? CONST : mem[ADDR];
}

/**
 * Stores the register into the specified memory address
 *
 * @return void
 */
void VirtualMachine::storeExec()
{
	incrementClock(1);
	mem[ADDR] = reg[RD];
}

/**
 * Adds constant if immediate (I) is high
 * or adds value register if I is low
 *
 * @return void
 */
void VirtualMachine::addExec()
{
	incrementClock(1);
	reg[RD] += I ? CONST : reg[RS];
	setCarry();
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
	incrementClock(1);
	reg[RD] += I ? (CONST + getCarry()) : (reg[RS] + getCarry());
	setCarry();
}

/**
 * Subtracts constant if immediate (I) is high
 * or subtracts register's value if I is low
 *
 * @return void
 */
void VirtualMachine::subExec()
{
	incrementClock(1);
	reg[RD] -= I ? CONST : reg[RS];
	setCarry();
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
	incrementClock(1);
	reg[RD] -= I ? (CONST - getCarry()) : (reg[RS] - getCarry());
	setCarry();
}

/**
 * Bitwise AND's constant if immediate (I) is high
 * or AND's register's value if I is low
 *
 * @return void
 */
void VirtualMachine::andExec()
{
	incrementClock(1);
	reg[RD] &= I ? CONST : reg[RS];
}

/**
 * Bitwise XOR's constant if immediate (I) is high
 * or XOR's register's value if I is low
 *
 * @return void
 */
void VirtualMachine::xorExec()
{
	incrementClock(1);
	reg[RD] ^= I ? CONST : reg[RS];
}

/**
 * Creates Compliment (opposite bits) of reg[RD]
 *
 * @return void
 */
void VirtualMachine::complExec()
{
	incrementClock(1);
	reg[RD] = ~reg[RD];
}

/**
 * Shifts bits left by 1
 *
 * @return void
 */
void VirtualMachine::shlExec()
{
	incrementClock(1);
	reg[RD] = reg[RD] << 1;
	setCarry();
}

/**
 * Arithmetically shifts bits left by 1
 *
 * @return void
 */
void VirtualMachine::shlaExec()
{
	shlExec();
}

/**
 * Shifts bits right by 1
 *
 * @return void
 */
void VirtualMachine::shrExec()
{
	incrementClock(1);
	reg[RD] = reg[RD] >> 1;
	setCarry(); //I dont get why we would set carry on a right shift? -Taylor
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
	incrementClock(1);

	//I may want to clean this up because I could just 'OR' it with an 'AND' of reg[RD]'s 15th bit.
	//But I'm not sure that I cleaner yet.
	reg[RD] = (reg[RD] & 0x8000) ? (reg[RD] >> 1) | 0x8000 : (reg[RD] >> 1) & 0x7FFF;
	//Needs to dublicate the MSB as it shifts
	// if (reg[RD] & 0x8000){
	// 	reg[RD] = (reg[RD] >> 1) | 0x8000;
	// } else{
	// 	reg[RD] = (reg[RD] >> 1) & 0x7FFF;
	// }
	setCarry(); //I dont get why we would set carry on a right shift? -Taylor
}

/**
 * Compares constant if immediate (I) is high
 * or Compares register's value if I is low
 * Then it sets the appropriate statusReg bitmask
 *
 *
 * @return void
 */
void VirtualMachine::comprExec()
{
	incrementClock(1);
	int constOrReg = I ? CONST : reg[RS];
	(reg[RD] < constOrReg) ? setLess() : ((reg[RD] < constOrReg) ? setEqual() : setGreater());

	// if (reg[RD] < constOrReg){
	// 	setLess();
	// } else if (reg[RD] == constOrReg){
	// 	setEqual();
	// } else{
	// 	setGreater();
	// }
}

/**
 * Sets the register equal to the statusReg
 *
 * @return void
 */
void VirtualMachine::getstatExec()
{
	incrementClock(1);
	reg[RD] = statusReg;
}

/**
 * Sets the statusReg equal to the register
 *
 * @return void
 */
void VirtualMachine::putstatExec()
{
	incrementClock(1);
	statusReg = reg[RD];
}

/**
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpExec()
{
	incrementClock(1);
	programCounter = ADDR;
}

/**
 * If the 'Less than' bit is high in the statusReg
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumplExec()
{
	incrementClock(1);
	programCounter = getLess() ? ADDR : programCounter;
}

/**
 * If the 'Equal to' bit is high in the statusReg
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpeExec()
{
	incrementClock(1);
	programCounter = getEqual() ? ADDR : programCounter;
}


/**
 * If the 'Greater than' bit is high in the statusReg
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpgExec()
{
	incrementClock(1);
	programCounter = getGreater() ? ADDR : programCounter;
}

/**
 * Jumps to a new address, and stores the current Process Control Block
 *
 * @return void
 */
void VirtualMachine::callExec()
{
	incrementClock(4);
	pushStack(programCounter);
	pushStack(statusReg);
	pushStack(reg[0]);
	pushStack(reg[1]);
	pushStack(reg[2]);
	pushStack(reg[3]);
	programCounter = ADDR;
}

/**
 * Returns to the previous state of the Process Control Block
 *
 * @return void
 */
void VirtualMachine::returnExec()
{
	incrementClock(4);
	programCounter = popStack();
	statusReg = popStack();
	reg[0] = popStack();
	reg[1] = popStack();
	reg[2] = popStack();
	reg[3] = popStack();
}

/**
 * Reads in data from a ".in" file to a register
 *
 * @return void
 */
void VirtualMachine::readExec()
{
	incrementClock(28);
	string inputFile = (filename.substr(0, filename.find_first_of("."))).append(".in");
	ifstream istream(inputFile.c_str());
	istream >> reg[RD];
	istream.close();
}

/**
 * Saves/Writes out a register's data to a ".out" file
 *
 * @return void
 */
void VirtualMachine::writeExec()
{
	incrementClock(28);
	string outputFile = (filename.substr(0, filename.find_first_of("."))).append(".out");
	ofstream ostream(outputFile.c_str());
	ostream << reg[RD];
	ostream.close();
}

/**
 * Halts the VirtualMachine
 *
 * @return void
 */
void VirtualMachine::haltExec()
{
	incrementClock(1);
	//destruct VirtualMachine?
}

/**
 * Only increments the CPU clock
 *
 * @return void
 */
void VirtualMachine::noopExec()
{
	incrementClock(1);
}
