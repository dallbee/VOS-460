/**
 * Implementation of the Assembler class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.3
 * @date 27 April, 2013
 * @file Assembler.cpp
 */

#include "Assembler.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
using namespace std;

/**
 * Construct object and load opcodes file into a map of opCodes and
 * a set of immediates.
 *
 * @param opListPath The path to the opcode listing to be used for assembly.
 */
Assembler::Assembler(const string &opListPath): line(), lineNumber(), opCodes(),
	immediates(), rdSet(), loads()
{
	string opCode, flags;
	ifstream opListFile(opListPath.c_str());
	istream_iterator<string> it (opListFile), end;

	// Load opcodes file into memory
	for (int i = 0; it != end; ++i, ++it) {
		opCode = it->substr(0, it->find(':'));
		flags = it->substr(it->find(':') + 1, it->length() - it->find(':'));

		// Build set of opCodes capable of immediate addressing
		if (flags.find('i') != string::npos) {
			immediates.insert(opCode);
		}

		// Build a set of opCodes with register destinations
		if (flags.find('r') != string::npos) {
			rdSet.insert(opCode);
		}

		// Build a set of opCodes that do some form of loading
		if (flags.find('l') != string::npos) {
			loads.insert(opCode);
		}

		// Build map of generic opCodes
		opCodes.insert(pair<string, int>(opCode, i));
	}

	// Verify data existence
	if (opCodes.empty()) {
		throw "Could not read data in from operation codes file";
	}
}

/**
 * Load source file line by line and assemble opcode field of object code.
 *
 * @param sourcePath The path to the program that will be assembled
 */
void Assembler::build(const string &sourcePath)
{
	int object = 0;
	Instruction op = {"", 0, 0, 0, 0, 0, 0};
	ifstream sourceFile(sourcePath.c_str());
	ofstream programFile("test.o");

	// Parse source file and build assembly file
	for (lineNumber = 1; getline(sourceFile, line); ++lineNumber) {
		op = parse(line);

		// Pass upon no instruction
		if (op.text.empty()) {
			continue;
		}

		// Check for invalid instructions
		if (opCodes.find(op.text) == opCodes.end()) {
			string message = "Illegal instruction";
			throw parseError(lineNumber, message, line).c_str();
		}

		// Check for invalid immediate instructions
		if (op.i and immediates.find(op.text) == immediates.end()) {
			string message = "Illegal immediate instruction";
			throw parseError(lineNumber, message, line).c_str();
		}

		object = format(op);
		programFile << object << endl;
	}
}

/**
 * Takes in an instruction and formats the instruction into decimal object code
 *
 * @param op The instruction to format
 * @return The opcode as an integer
 */
int Assembler::format(Instruction &op)
{
	// Command does not contain RD argument
	if (rdSet.find(op.text) == rdSet.end()) {
		op.value = op.arg0;
	} else {
		op.rd = op.arg0;

		// Second argument is an address/constant
		if (op.i or loads.find(op.text) != loads.end()) {
			op.value = op.arg1;
		} else {
			op.rs = op.arg1;
		}
	}

	// Check that register source is within range
	if (op.rs > 3) {
		string message = "Register source value out of range";
		throw parseError(lineNumber, message, line).c_str();
	}

	// Check that register destination is within range
	if (op.rd > 3) {
		string message = "Register destination value out of range";
		throw parseError(lineNumber, message, line).c_str();
	}

	op.value |= op.rs << 6; // Build RS the last 8 bits of the command
	op.value &= 0xFF;

	// Build and return the entire command
	return ((opCodes[op.text] << 2 | op.rd) << 1 | op.i) << 8 | op.value;
}

/**
 * Splits an assembly instruction into its command and parameters
 *
 * @param line The source code instruction to be interpereted.
 * @return The opcode as an Assembler::instruction
 */
Assembler::Instruction Assembler::parse(const string &line)
{
	Instruction op = {"", 0, 0, 0, 0, 0, 0};
	int start = 0, end = 0;

	// Get the size of the instruction without comments
	int size = min(line.find_first_of("!"), line.size());

	// Extract the command
	if (size > 0) {
		start = line.find_first_not_of("\t ");
		end = min(line.find_first_of("\t ", start), line.size());
		op.text = line.substr(start, end - start);
		transform(op.text.begin(), op.text.end(), op.text.begin(), ::tolower);
	}

	// Set immediate bit and remove from command
	if (op.text[op.text.size() - 1] == 'i') {
		op.text.erase(op.text.size() - 1);
		op.i = 1;
	}

	// Extract the first argument, if it exists
	if (size > end) {
		start = line.find_first_not_of("\t ", end);
		end = min(line.find_first_of("\t ", start), line.size());
		istringstream(line.substr(start, end - start)) >> op.arg0;
	}

	// Extract the second argument, if it exists
	if (size > end) {
		start = line.find_first_not_of("\t ", end);
		end = min(line.find_first_of("\t ", start), line.size());
		istringstream(line.substr(start, end - start)) >> op.arg1;
	}

	return op;
}

/**
 * Formats parsing errors in a consistant and human readable way.
 * Ex: On line 5, Invalid Instruction: "add 2 172"
 *
 * @param lineNumber The line of which the error occurred
 * @param msg The error message to display
 * @param line The contents of the line in question
 * @return The error message
 */
string Assembler::parseError(int lineNumber, string msg, string line)
{
	stringstream ss;
	ss << lineNumber;
	string error = "On line: " + ss.str() + ", " + msg + ": \"" + line + "\"";
	return error;
}
