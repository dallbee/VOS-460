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
	map<string, int> opCodes;
	set<string> immediates;
	struct instruction {
		string command;
		unsigned int immediate;
		unsigned int rd;
		unsigned int value;
	};
	static instruction parse(const string &line);
	static string parseError(int lineNumber, string msg, string line);
};
