CC      = g++
CFLAGS  = -g -Wall -O3
TARGETDIR = bin
SRCDIR = src

all: boot

boot: os.o virtualMachine.o assembler.o
	$(CC) $(CFLAGS) -o $(TARGETDIR)/boot.exe $(SRCDIR)/boot.cpp $(TARGETDIR)/os.o $(TARGETDIR)/virtualMachine.o $(TARGETDIR)/assembler.o

os.o:
	$(CC) $(CFLAGS) -o $(TARGETDIR)/os.o -c $(SRCDIR)/os/os.cpp

virtualMachine.o:
	$(CC) $(CFLAGS) -o $(TARGETDIR)/virtualMachine.o -c $(SRCDIR)/kernel/virtualMachine.cpp

assembler.o:
	$(CC) $(CFLAGS) -o $(TARGETDIR)/assembler.o -c $(SRCDIR)/assembler/assembler.cpp

clean:
	$(RM) $(TARGET)