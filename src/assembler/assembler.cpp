// TODO
// - Needs to handle different instruction types
// - Needs to handle blank lines

#include "assembler.h"
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
Assembler::Assembler(const string &opListPath)
{
	char immediate;
	string opCode;
	ifstream opListFile(opListPath.c_str());
	istream_iterator<string> it (opListFile), end;

	// Load opcodes file into memory
	for (int i = 0; it != end; ++i, ++it) {
		immediate = it->at(it->size() - 1);
		opCode = it->substr(0, it->size() - 2);

		// Build set of opCodes capable of immediate addressing
		if(immediate == '1') {
			immediates.insert(opCode);
		}

		// Build map of generic opCodes
		opCodes.insert(pair<string, int>(opCode, i));
	}

	// Verify data existence
	if  (this->opCodes.empty()) {
		throw "Could not read data in from operation codes file";
	}
}

/**
 * Load source file line by line and assemble object code.
 *
 * @param sourcePath The path to the program that will be assembled
 * @return void
 */
void Assembler::build(const string &sourcePath)
{
	int object = 0, lineNumber = 0;
	instruction op = {"", 0};
	ifstream sourceFile(sourcePath.c_str());
	ofstream programFile("test.o");

	// Parse source file and build assembly file
	for (string line; getline(sourceFile, line); ++lineNumber) {
		op = parse(line);

		// Check for invalid instructions
		if (opCodes.find(op.command) == opCodes.end()) {
			string message = "Illegal instruction";
			throw parseError(lineNumber, message, line).c_str();
		}

		// Check for invalid immediate instructions
		if (op.immediate and immediates.find(op.command) == immediates.end()) {
			string message = "Illegal immediate instruction";
			throw parseError(lineNumber, message, line).c_str();
		}

		object = ((opCodes[op.command] << 2 | 0) << 1 | op.immediate) << 8 | op.value;
		programFile << object;
	}
}

/**
 * Splits an assembly instruction into its command and parameters
 * 
 * @param line The source code instruction to be interpereted.
 * @return Assembler::instruction
 */
Assembler::instruction Assembler::parse(const string &line)
{
	instruction op = {"", 0};
	int start = 0, end = 0;

	// Get the size of the instruction without comments
	int size = min(line.find_first_of("!"), line.size() - 1);

	// Convert instruction to lowercase
	for (int i = 0; i != size; ++i) {
		line[i] = tolower(line[i]);
	}

	// Extract the command
	if (size > 0) {
		start = line.find_first_not_of("\t ");
		end = line.find_first_of("\t ", start);
		op.command = line.substr(start, end - start);

		// Set immediate bit and remove from command
		if (op.command[op.command.size() - 1] == 'i') {
			op.command.erase(op.command.size() - 1);
			op.immediate = 1;
		}
	}

	// Extract the first argument, if it exists
	if (size > end) {
		start = line.find_first_of("0123456789", end);
		if (start < 0) {
			return op;
		}
		end = line.find_first_not_of("0123456789", start);
		//istringstream(line.substr(start, end - start)) >> op.arg0;
	}

	// Extract the second argument, if it exists
	if (size > end) {
		start = line.find_first_of("0123456789", end);
		if (start < 0) {
			return op;
		}
		end = line.find_first_not_of("0123456789", start);
		//istringstream(line.substr(start, end - start)) >> op.arg1;
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
 * @return string
 */
string Assembler::parseError(int lineNumber, string msg, string line)
{
	stringstream ss;
	ss << lineNumber;
	string error = "On line: " + ss.str() + ", " + msg + ": \"" + line + "\"";
	return error;
}
