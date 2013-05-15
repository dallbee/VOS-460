/**
 * Definition of the Assembler class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.2
 * @date 22 April, 2013
 * @file Assembler.h
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <map>
#include <set>
#include <string>
using namespace std;

class Assembler {
public:
	Assembler(const string &opListPath);
	virtual ~Assembler() {};
	void build(const string &sourcePath);

private:
	string line;
	unsigned lineNumber;
	map<string, int> opCodes;

	// Opcode parameter sets
	set<string> immediates;
	set<string> rdSet;
	set<string> loads;

	struct Instruction {
		string text;
		int arg0;
		int arg1;
		int rd;
		int i;
		int rs;
		unsigned value;
	};
	int format(Instruction &op);
	static Instruction parse(const string &line);
	static string parseError(int lineNumber, string msg, string line);
};
#endif
