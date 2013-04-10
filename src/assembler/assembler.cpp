#include "assembler.h"
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
using namespace std;

/**
 * Construct object and load opcodes file into vectors opCodes and
 * immediateOpCodes.
 *
 * @param opListPath The path to the opcode listing to be used for assembly.
 */
Assembler::Assembler(string opListPath)
{
	// Load opcodes file to a vector
	ifstream opListFile(opListPath.c_str());
	copy(istream_iterator<string>(opListFile),
          istream_iterator<string>(),
          back_inserter(this->opCodes));

	// Verify data existence
	if  (this->opCodes.empty()) {
		throw "Operation codes file empty or not found";
	}

	// Build list of opcodes with immediate addressing capability into a vector
	char immediate;
	vector<string>::iterator i = this->opCodes.begin();
	for (; i != this->opCodes.end(); ++i) {
		immediate = i->at(i->size() - 1);
		i->erase(i->size() - 2);
		if(immediate == '1') {
			this->immediateOpCodes.push_back(*i);
		}
	}
}

/**
 * [Assembler::build description]
 *
 * @param sourcePath The path to the program that will be assembled
 * @return void
 */
void Assembler::build(string sourcePath)
{

}





/* Opcodes for reference

	int loadi	//0000 OPcode mask
	int loadi	//0000 OPcode mask
	int store	//0001 OPcode mask
	int add
	int addi
	int addc
	int addci
	int sub
	int subi
	int subc
	int subci
	int and
	int andi
	int xor
	int xori
	int compl
	int shl
	int shla
	int shr
	int shra
	int compr
	int compri
	int getstat
	int putstat
	int jump
	int jumpl
	int jumpe
	int jumpg
	int call
	int return
	int read
	int write
	int halt		//11000 OP/code mask
	int noop 	//11001 OPcode mask
*/
