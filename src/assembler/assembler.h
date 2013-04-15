#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#endif

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
	set<string> immediateOpCodes;
	struct instruction {
		string command;
		int arg0;
		int arg1;
	};
	static instruction parse(const string &line);
};
