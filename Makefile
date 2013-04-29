CC      = g++
CFLAGS  = -g -O3 -Wall -Wextra -Wshadow -Wstrict-aliasing -Wstrict-overflow=5 -Weffc++ -Werror -pedantic -ansi
TARGETDIR = bin
SRCDIR = src

all: boot

boot: VirtualMachine.o Assembler.o
	$(CC) $(CFLAGS) -o $(TARGETDIR)/os $(SRCDIR)/os.cpp $(TARGETDIR)/VirtualMachine.o $(TARGETDIR)/Assembler.o

#os.o:
#	$(CC) $(CFLAGS) -o $(TARGETDIR)/OS.o -c $(SRCDIR)/sys/OS.cpp

VirtualMachine.o:
	$(CC) $(CFLAGS) -o $(TARGETDIR)/VirtualMachine.o -c $(SRCDIR)/vm/VirtualMachine.cpp

Assembler.o:
	$(CC) $(CFLAGS) -o $(TARGETDIR)/Assembler.o -c $(SRCDIR)/sys/assembler/Assembler.cpp

clean:
	rm -f $(TARGETDIR)/*

