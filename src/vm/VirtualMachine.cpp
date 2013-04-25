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
* Each of the opCodes available
*
* @param
* no data to them yet
*/

void VirtualMachine::loadExec()
{
	reg[RD] = I ? CONST : mem[ADDR];
}

void VirtualMachine::storeExec()
{
	mem[ADDR] = reg[RD];
}

void VirtualMachine::addExec()
{
	reg[RD] += I ? CONST : reg[RS];
	CARRY = 1;
}

void VirtualMachine::addcExec()
{
	reg[RD] += I ? (CONST + CARRY) : (reg[RS] + CARRY);
	CARRY = 1;
}

void VirtualMachine::subExec()
{
	reg[RD] -= I ? CONST : reg[RS];
	CARRY = 1;
}

void VirtualMachine::subcExec()
{
	reg[RD] -= I ? (CONST - CARRY) : (reg[RS] - CARRY);
	CARRY = 1;
}

void VirtualMachine::andExec()
{
	reg[RD] &= I ? CONST : reg[RS];
}

void VirtualMachine::xorExec()
{
	reg[RD] ^= I ? CONST : reg[RS];
}

void VirtualMachine::complExec()
{
	reg[RD] = ~reg[RD];
}

void VirtualMachine::shlExec()
{
	reg[RD] = reg[RD] << 1;
	CARRY = 1;
}

void VirtualMachine::shlaExec()
{

}

void VirtualMachine::shrExec()
{
	reg[RD] = reg[RD] >> 1;
	CARRY = 1;
}

void VirtualMachine::shraExec()
{

}

void VirtualMachine::comprExec()
{

}

void VirtualMachine::getstatExec()
{
	reg[RD] = sr;
}

void VirtualMachine::putstatExec()
{
	sr = reg[RD];
}

void VirtualMachine::jumpExec()
{
	pc = ADDR;
}

void VirtualMachine::jumplExec()
{
	pc = LESS ? ADDR : pc;
}

void VirtualMachine::jumpeExec()
{
	pc = EQUAL ? ADDR : pc;
}

void VirtualMachine::jumpgExec()
{
	pc = GREATER ? ADDR : pc;
}

void VirtualMachine::callExec()
{

}

void VirtualMachine::returnExec()
{

}

void VirtualMachine::readExec()
{

}

void VirtualMachine::writeExec()
{

}

void VirtualMachine::haltExec()
{

}

void VirtualMachine::noopExec()
{

}
