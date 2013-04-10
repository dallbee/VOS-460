// TODO
// - test parser
// - opCodes vector to a map with value => id
// - finish builder
// - extend for overflow, less, equal, greater, carry
// - add error checks to invalid commands/arguments
// - extend opcodes.lst to allow for carries
// - finish function documentation

#include "assembler.h"
#include <vector>
#include <string>
#include <sstream>
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
Assembler::Assembler(const string &opListPath)
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
void Assembler::build(const string &sourcePath)
{
	ifstream sourceFile(sourcePath.c_str());
	string line;
	instruction operation;
	while (getline(sourceFile, line)) {
		operation = parse(line);
		// Check operation validity

		// Check argument 0 range

		// Check argument 1 range


	}
}

/**
 * [Assembler::parse description]
 * @param  line
 * @return
 */
Assembler::instruction Assembler::parse(const string &line)
{
	instruction inst;
	inst.arg0 = 0;
	inst.arg1 = 0;
	int size = min(line.find_first_of("!"), line.size() - 1);
	int start = 0;
	int end = 0;

	// Extract the command
	if (size > 0) {
		start = line.find_first_not_of("\t ");
		end = line.find_first_of("\t ", start);
		inst.command = line.substr(start, end - start);
	}

	// Extract the first argument, if it exists
	if (size > end) {
		start = line.find_first_of("0123456789", end);
		if (start < 0) {
			return inst;
		}
		end = line.find_first_not_of("0123456789", start);
		istringstream(line.substr(start, end - start)) >> inst.arg0;
	}

	// Extract the second argument, if it exists
	if (size > end) {
		start = line.find_first_of("0123456789", end);
		if (start < 0) {
			return inst;
		}
		end = line.find_first_not_of("0123456789", start);
		istringstream(line.substr(start, end - start)) >> inst.arg1;
	}

	return inst;
}
