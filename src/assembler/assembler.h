#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#endif

#include <vector>
#include <string>
using namespace std;

class Assembler {
public:
	Assembler(string opListPath);
	void build(string sourcePath);
private:
	vector<string> opCodes;
	vector<string> immediateOpCodes;
};
