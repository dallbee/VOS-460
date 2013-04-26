#include "VirtualMachine.h"
#include <map>
#include <set>
#include <string>
#include <stdio.h>


using namespace std;

/**
 * Construct object and create registers and memory
 *
 * @param
 */
 VirtualMachine::VirtualMachine ()
 {
//	int reg[regSize]={0};
// 	int mem[memSize]={0};
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

	for(; pc != memSize;) {
		ir = mem[pc++];
		CONST = ir & 0xFF;
		ADDR = ir & 0x3F;
		RS = (ir >>= 6) & 0x03;
		I = (ir >>= 2) & 0x01;
		RD = (ir >>= 1) & 0x03;
		OP = (ir >>= 5);
		(this->*instructions[OP])();
	}



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
	// 	sr |= 1;
	// } else{
	// 	sr &= 0xFFFE;
	// }
	sr = (reg[RD] & 0x10000) ? (sr | 1) : (sr & 0xFFFE);
}

/**
 * Sets 'greater than' bit in status register
 * while clearing equal and less than
 *
 * @return void
 */
void VirtualMachine::setGreater(){
	sr = (sr & 0xFFF3) | 2;
}

/**
 * Sets 'equal to' bit in status register
 * while clearing less and greater than
 *
 * @return void
 */
void VirtualMachine::setEqual(){
	sr = (sr & 0xFFF5) | 4;
}

/**
 * Sets 'less than' bit in status register
 * while clearing equal and greater than
 *
 * @return void
 */
void VirtualMachine::setLess(){
	sr = (sr & 0xFFF9) | 8;
}

/**
 * Checks for 'Carry' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
int VirtualMachine::getCarry(){
	return (sr & 1) ? 1 : 0;
}

/**
 * Checks for 'Greater than' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
int VirtualMachine::getGreater(){
	return (sr & 2) ? 1 : 0;
}

/**
 * Checks for 'Equal to' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
int VirtualMachine::getEqual(){
	return (sr & 4) ? 1 : 0;
}

/**
 * Checks for 'Less than' bit in status register
 *
 * @return 1 or 0. based on bit status
 */
int VirtualMachine::getLess(){
	return (sr & 8) ? 1 : 0;
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
	incrementClock(1);
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

	//Needs to dublicate the MSB as it shifts
	if (reg[RD] & 0x8000){
		reg[RD] = (reg[RD] >> 1) | 0x8000;
	} else{
		reg[RD] = (reg[RD] >> 1) & 0x7FFF;
	}
	setCarry(); //I dont get why we would set carry on a right shift? -Taylor
}

/**
 * Compares constant if immediate (I) is high
 * or Compares register's value if I is low
 * Then it sets the appropriate statusRegister (sr) bitmask
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
 * Sets the register equal to the statusRegister(sr)
 *
 * @return void
 */
void VirtualMachine::getstatExec()
{
	incrementClock(1);
	reg[RD] = sr;
}

/**
 * Sets the statusRegister(sr) equal to the register
 *
 * @return void
 */
void VirtualMachine::putstatExec()
{
	incrementClock(1);
	sr = reg[RD];
}

/**
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpExec()
{
	incrementClock(1);
	pc = ADDR;
}

/**
 * If the 'Less than' bit is high in the statusRegister(sr)
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumplExec()
{
	incrementClock(1);
	pc = getLess() ? ADDR : pc;
}

/**
 * If the 'Equal to' bit is high in the statusRegister(sr)
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpeExec()
{
	incrementClock(1);
	pc = getEqual() ? ADDR : pc;
}


/**
 * If the 'Greater than' bit is high in the statusRegister(sr)
 * Sets the program counter to the specified address
 *
 * @return void
 */
void VirtualMachine::jumpgExec()
{
	incrementClock(1);
	pc = getGreater() ? ADDR : pc;
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::callExec()
{
	incrementClock(4);
	/*
call and return instructions need special attention. As part of the execution
of the call instruction the status of the VM must be pushed on to the stack.
The status of the VM consist of pc, r[0]-r[3], and sr. The stack grows from
the bottom of memory up, therefore initially sp = 256. After a call, sp is
decremented by 6 as the values of pc, r[0]-r[3], and sr in the VM are pushed
on to the stack. When a return instruction is executed, sp is incremented by
6 as the new values of pc, r[0]-r[3], and sr are popped from the stack and
restored in the VM. When sp >= 256 stack is empty, and when sp < limit+6 stack
is full.
	*/

}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::returnExec()
{
	incrementClock(4);
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::readExec()
{
	incrementClock(28);
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::writeExec()
{
	incrementClock(28);
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::haltExec()
{
	incrementClock(1);
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::noopExec()
{
	incrementClock(1);
}
