/** USE THIS FOR READING IN OPCODES
	std::vector<std::string> lines;
	for (std::string line; std::getline( ifs, line );)
   		lines.push_back( line );
   	*/
#include "OS.h"

OS::OS(): program(), programAss(), ass("assembler/opcodes.lst"), vm(){

}


/**
 *
 *
 * @return void
 */
void OS::launchProgram(string programFile){
	program = programFile;
	assemble();
	execute();
}
/*
 *
 *
 * @return void
 */
void OS::assemble(){
	ass.build(program);
}

/**
 *
 *
 * @return void
 */
void OS::execute(){
	programAss = "test.o";
	vm.execute(programAss);


}

/**
 *
 *
 * @return void
 */
void OS::vmDump(){
	vm.machineDump();
}
