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
	instructions[0x00] = &VirtualMachine::loadInstruction;
	instructions[0x01] = &VirtualMachine::storeInstruction;
	instructions[0x02] = &VirtualMachine::addInstruction;
	instructions[0x03] = &VirtualMachine::addcInstruction;
	instructions[0x04] = &VirtualMachine::subInstruction;
	instructions[0x05] = &VirtualMachine::subcInstruction;
	instructions[0x06] = &VirtualMachine::andInstruction;
	instructions[0x07] = &VirtualMachine::xorInstruction;
	instructions[0x08] = &VirtualMachine::complInstruction;
	instructions[0x09] = &VirtualMachine::shlInstruction;
	instructions[0x0A] = &VirtualMachine::shlaInstruction;
	instructions[0x0B] = &VirtualMachine::shrInstruction;
	instructions[0x0C] = &VirtualMachine::shraInstruction;
	instructions[0x0D] = &VirtualMachine::comprInstruction;
	instructions[0x0E] = &VirtualMachine::getstatInstruction;
	instructions[0x0F] = &VirtualMachine::putstatInstruction;
	instructions[0x10] = &VirtualMachine::jumpInstruction;
	instructions[0x11] = &VirtualMachine::jumplInstruction;
	instructions[0x12] = &VirtualMachine::jumpeInstruction;
	instructions[0x13] = &VirtualMachine::jumpgInstruction;
	instructions[0x14] = &VirtualMachine::callInstruction;
	instructions[0x15] = &VirtualMachine::returnInstruction;
	instructions[0x16] = &VirtualMachine::readInstruction;
	instructions[0x17] = &VirtualMachine::writeInstruction;
	instructions[0x18] = &VirtualMachine::haltInstruction;
	instructions[0x19] = &VirtualMachine::noopInstruction;

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
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::loadInstruction()
{
	reg[RD] = I ? CONST : mem[ADDR];
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::storeInstruction()
{
	mem[ADDR] = reg[RD];
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::addInstruction()
{
	reg[RD] += I ? CONST : reg[RS];
	CARRY = 1;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::addcInstruction()
{
	reg[RD] += I ? (CONST + CARRY) : (reg[RS] + CARRY);
	CARRY = 1;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::subInstruction()
{
	reg[RD] -= I ? CONST : reg[RS];
	CARRY = 1;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::subcInstruction()
{
	reg[RD] -= I ? (CONST - CARRY) : (reg[RS] - CARRY);
	CARRY = 1;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::andInstruction()
{
	reg[RD] &= I ? CONST : reg[RS];
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::xorInstruction()
{
	reg[RD] ^= I ? CONST : reg[RS];
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::complInstruction()
{
	reg[RD] = ~reg[RD];
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::shlInstruction()
{
	reg[RD] = reg[RD] << 1;
	CARRY = 1;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::shlaInstruction()
{

}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::shrInstruction()
{
	reg[RD] = reg[RD] >> 1;
	CARRY = 1;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::shraInstruction()
{

}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::comprInstruction()
{

}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::getstatInstruction()
{
	reg[RD] = sr;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::putstatInstruction()
{
	sr = reg[RD];
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::jumpInstruction()
{
	pc = ADDR;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::jumplInstruction()
{
	pc = LESS ? ADDR : pc;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::jumpeInstruction()
{
	pc = EQUAL ? ADDR : pc;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::jumpgInstruction()
{
	pc = GREATER ? ADDR : pc;
}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::callInstruction()
{

}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::returnInstruction()
{

}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::readInstruction()
{

}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::writeInstruction()
{

}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::haltInstruction()
{

}

/**
 * [VirtualMachine::loadInstruction description]
 *
 * @return void
 */
void VirtualMachine::noopInstruction()
{

}
