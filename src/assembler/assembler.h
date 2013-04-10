#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#endif

#include <vector>
#include <string>
using namespace std;

class Assembler {
public:
	Assembler(const string &opListPath);
	void build(const string &sourcePath);
private:
	vector<string> opCodes;
	vector<string> immediateOpCodes;
	struct instruction {
		string command;
		int arg0;
		int arg1;
	};
	static instruction parse(const string &line);
};
