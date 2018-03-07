.phony all:
all: trnschdl

trnschdl: trnschdl.cpp dispatcher.cpp station.cpp train.cpp train.h
	g++ trnschdl.cpp -g -o trnschdl

.PHONY clean:
clean:
	-rm train station dispatcher trnschdl
	-rm -rf *.o *.exe
	-rm -rf trnschdl.dSYM
