CC     = g++
CFLAGS = -g -O3 -pedantic -ansi -Werror
WFLAGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wstrict-overflow=5
FLAGS = $(CFLAGS) $(WFLAGS)
TARGETDIR = bin
SRCDIR = src

all: os

os: VirtualMachine.o Assembler.o
	$(CC) $(FLAGS) -o $(TARGETDIR)/os $(SRCDIR)/OS.cpp $(TARGETDIR)/VirtualMachine.o $(TARGETDIR)/Assembler.o

VirtualMachine.o:
	$(CC) $(FLAGS) -o $(TARGETDIR)/VirtualMachine.o -c $(SRCDIR)/vm/VirtualMachine.cpp

Assembler.o:
	$(CC) $(FLAGS) -o $(TARGETDIR)/Assembler.o -c $(SRCDIR)/sys/assembler/Assembler.cpp

clean:
	rm -f $(TARGETDIR)/*
