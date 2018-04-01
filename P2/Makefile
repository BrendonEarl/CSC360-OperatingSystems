.phony all:
all: mts

mts: trnschdl.cpp dispatcher.cpp station.cpp train.cpp train.h
	g++ -pthread -g -o mts trnschdl.cpp

.PHONY clean:
clean:
	-rm mts
	-rm -rf *.o *.exe
	-rm -rf mts.dSYM
