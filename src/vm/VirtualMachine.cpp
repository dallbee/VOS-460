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
* Also sets CARRY =1, seems redundant to have it in sr and separate variable...
* couldn't we just check the correct bit of sr isntead of CARRY/LESS/EQUAL etc?
*
* @return void
*/
void VirtualMachine::setCarry(){
//we shouldn't just be blindly setting CARRY should we?
//I thought it needed some logic behind it.
	// if(reg[RD] & 0x10000 ){
	// 	sr |= 1;
	// } else{
	// 	sr &= 0xFFFE;
	// }
	sr = (reg[RD] & 0x10000) ? (sr | 1) : (sr & 0xFFFE);
}

void VirtualMachine::setGreater(){
	sr = (sr & 0xFFF3) | 2;

}
void VirtualMachine::setEqual(){
	sr = (sr & 0xFFF5) | 4;

}
void VirtualMachine::setLess(){
	sr = (sr & 0xFFF9) | 8;
}

int VirtualMachine::getCarry(){
	return CARRY = (sr & 1) ? 1 : 0;
}
int VirtualMachine::getGreater(){
	return CARRY = (sr & 2) ? 1 : 0;
}
int VirtualMachine::getEqual(){
	return CARRY = (sr & 4) ? 1 : 0;
}
int VirtualMachine::getLess(){
	return CARRY = (sr & 8) ? 1 : 0;
}

void VirtualMachine::incrementClock(int cycles){
	clock += cycles;
}


/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::loadExec()
{
	incrementClock(1);
	reg[RD] = I ? CONST : mem[ADDR];
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::storeExec()
{
	incrementClock(1);
	mem[ADDR] = reg[RD];
}

/**
 * [VirtualMachine::loadExec description]
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
 * [VirtualMachine::loadExec description]
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
 * [VirtualMachine::loadExec description]
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
 * [VirtualMachine::loadExec description]
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
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::andExec()
{
	incrementClock(1);
	reg[RD] &= I ? CONST : reg[RS];
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::xorExec()
{
	incrementClock(1);
	reg[RD] ^= I ? CONST : reg[RS];
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::complExec()
{
	incrementClock(1);
	reg[RD] = ~reg[RD];
}

/**
 * [VirtualMachine::loadExec description]
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
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::shlaExec()
{
	incrementClock(1);
	shlExec();
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::shrExec()
{
	incrementClock(1);
	reg[RD] = reg[RD] >> 1;
	setCarry();
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::shraExec()
{
	incrementClock(1);

	//Needs to dublicate the MSB as it shifts
	if (reg[RD] & 0x8000){
		reg[RD] = (reg[RD] >> 1) | 0x8000;
	} else{
		reg[RD] = (reg[RD] >> 1) & 0x7FFF;
	}
	setCarry();
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::comprExec()
{
	incrementClock(1);
	int constOrReg = I ? CONST : reg[RS];
	//(reg[RD] < constOrReg) ? setLess() : ((reg[RD] < constOrReg) ? setEqual() : setGreater());

	if (reg[RD] < constOrReg){
		setLess();
	} else if (reg[RD] == constOrReg){
		setEqual();
	} else{
		setGreater();
	}
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::getstatExec()
{
	incrementClock(1);
	reg[RD] = sr;
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::putstatExec()
{
	incrementClock(1);
	sr = reg[RD];
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::jumpExec()
{
	incrementClock(1);
	pc = ADDR;
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::jumplExec()
{
	incrementClock(1);
	pc = LESS ? ADDR : pc;
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::jumpeExec()
{
	incrementClock(1);
	pc = EQUAL ? ADDR : pc;
}

/**
 * [VirtualMachine::loadExec description]
 *
 * @return void
 */
void VirtualMachine::jumpgExec()
{
	incrementClock(1);
	pc = GREATER ? ADDR : pc;
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
