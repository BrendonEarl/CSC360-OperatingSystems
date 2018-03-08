.phony all:
all: trnschdl

trnschdl: trnschdl.cpp dispatcher.cpp station.cpp train.cpp train.h
	g++ -pthread -g -o trnschdl trnschdl.cpp

.PHONY clean:
clean:
	-rm trnschdl
	-rm -rf *.o *.exe
	-rm -rf trnschdl.dSYM
