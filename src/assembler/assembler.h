#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#endif

#define INT_MASK 0xFF

#include <map>
#include <set>
#include <string>
using namespace std;

class Assembler {
public:
	Assembler(const string &opListPath);
	void build(const string &sourcePath);
private:
	string line;
	unsigned lineNumber;
	map<string, int> opCodes;
	set<string> immediates;
	set<string> rdSet;
	struct instruction {
		string command;
		unsigned arg0;
		unsigned arg1;
		unsigned rd;
		unsigned i;
		unsigned rs;
		unsigned value;
	};
	unsigned format(instruction &op);
	static instruction parse(const string &line);
	static string parseError(int lineNumber, string msg, string line);
};
