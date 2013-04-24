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
	set<string> loads;
	struct Instruction {
		string command;
		int arg0;
		int arg1;
		unsigned rd;
		unsigned i;
		unsigned rs;
		unsigned value;
	};
	unsigned format(Instruction &op);
	static Instruction parse(const string &line);
	static string parseError(int lineNumber, string msg, string line);
};
