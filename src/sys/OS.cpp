/**
 * Implementation of the Assembler class
 *
 * @authors Dylan Allbee, Taylor Sanchez
 * @version 1.3
 * @date 27 April, 2013
 * @file Assembler.cpp
 */

#include "OS.h"
#include "assembler/Assembler.h"
#include "../vm/VirtualMachine.h"
#include <string>
#include <stdio.h>
#include <fstream>

OS::OS()
{
	printf("VOS-460 Loaded\n");
	printf("Created By: Dylan Allbee & Taylor Sanchez\n");
	bool quit = true;
	char input[255];
	do {
		quit = true;
		printf("\nProgram File: ");
		scanf("%s", input);
		printf("\nAssembling...");
		try {
			Assembler as("../src/sys/assembler/opcodes.lst");
			as.build(input);
		} catch(const char* error) {
			printf ("[Assembler Error] %s \n", error);
		}
		printf("Done!\n");
		printf("\nRunning Program...");
		try {
			VirtualMachine vm(input, memory);
			as.build(input);
		} catch(const char* error) {
			printf ("[VirtualMachine Error] %s \n", error);
		}
		printf("Program Complete!\n\n");
		printf("Would you like to run again? (y/n): ");
		scanf("%s", input);
		if (input[0] == 'y') {
			quit = false;
		}
	} while( ! quit);
}


/** USE THIS FOR READING IN OPCODES
	std::vector<std::string> lines;
	for (std::string line; std::getline( ifs, line );)
   		lines.push_back( line );
   	*/

	/*

	try {
		Assembler test ("../src/sys/assembler/opcodes.lst");
		test.build("../src/sys/assembler/test.asm");
	} catch(const char* error) {
		printf ("[Assembler Error] %s \n", error);
	}
	*/
	//virtualMachine()
