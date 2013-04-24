#include "virtualMachine.h"
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
 virtualMachine::virtualMachine ()
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

	for(pc != memSize) {
		ir = mem[pc++];
		CONST = ir & 0xFF;
		ADDR = ir & 0x3F;
		RS = (ir >>= 6) & 0x03;
		I = (ir >>= 2) & 0x01;
		RD = (ir >>= 1) & 0x03;
		OP = (ir >>= 5);
	}
	


 }

/**
* Each of the opCodes available
*
* @param
* no data to them yet
*/

void virtualMachine::loadExec()
{
	reg[RD] = I ? CONST : mem[ADDR];
}

void virtualMachine::storeExec()
{
	mem[ADDR] = reg[RD];
}

void virtualMachine::addExec()
{
	reg[RD] += I ? CONST : reg[RS];
	CARRY = 1;
}

void virtualMachine::addcExec()
{
	reg[RD] += I ? (CONST + CARRY) : (reg[RS] + CARRY);
	CARRY = 1;
}

void virtualMachine::subExec()
{
	reg[RD] -= I ? CONST : reg[RS];
	CARRY = 1;
}

void virtualMachine::subcExec()
{
	reg[RD] -= I ? (CONST - CARRY) : (reg[RS] - CARRY);
	CARRY = 1;
}

void virtualMachine::andExec()
{
	reg[RD] &= I ? CONST : reg[RS];
}

void virtualMachine::xorExec()
{
	reg[RD] ^= I ? CONST : reg[RS];
}

void virtualMachine::complExec()
{
	reg[RD] = ~reg[RD];
}

void virtualMachine::shlExec()
{
	reg[RD] = reg[RD] << 1;
	CARRY = 1;
}

void virtualMachine::shlaExec()
{

}

void virtualMachine::shrExec()
{
	reg[RD] = reg[RD] >> 1;
	CARRY = 1;
}

void virtualMachine::shraExec()
{

}

void virtualMachine::comprExec()
{

}

void virtualMachine::getstatExec()
{
	reg[RD] = sr;
}

void virtualMachine::putstatExec()
{
	sr = reg[RD];
}

void virtualMachine::jumpExec()
{
	pc = ADDR;
}

void virtualMachine::jumplExec()
{
	pc = LESS ? ADDR : pc;
}

void virtualMachine::jumpeExec()
{
	pc = EQUAL ? ADDR : pc;
}

void virtualMachine::jumpgExec()
{
	pc = GREATER ? ADDR : pc;
}

void virtualMachine::callExec()
{

}

void virtualMachine::returnExec()
{

}

void virtualMachine::readExec()
{

}

void virtualMachine::writeExec()
{

}

void virtualMachine::haltExec()
{

}

void virtualMachine::noopExec()
{

}
