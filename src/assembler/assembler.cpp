#include <iostream>
#include <fstream>


using namespace std;


class assembler{
public:

private:
	int  loadi	//0000 OPcode mask
	int loadi	//0000 OPcode mask
	int store	//0001 OPcode mask
	int add
	int addi
	int addc
	int addci
	int sub
	int subi
	int subc
	int subci
	int  and
	int andi
	int xor
	int xori
	int compl
	int shl
	int shla
	int shr
	int shra
	int compr
	int compri
	int getstat
	int putstat
	int jump
	int jumpl
	int jumpe
	int jumpg
	int call
	int return
	int  read
	int write
	int halt		//11000 OP/code mask
	int  noop 	//11001 OPcode mask


}

assembler::readParse(string filename){
	//simply read in the file
	ifstream file;
  	file.open (filename);
	//parsing ignore   !  exclamation  treat as comments

}


assembler::opcode(vector<string> opcodeText){

}

assembler::checkError(int currentLine, int testLimit){
	//compare the lines left, so we don't overshoot our program's limit
}


