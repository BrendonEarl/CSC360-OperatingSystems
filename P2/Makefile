.phony all:
all: trnschdl dispatcher station train

trnschdl: trnschdl.cpp dispatcher.cpp station.cpp train.cpp train.h
	g++ trnschdl.cpp -g -o trnschdl

dispatcher: dispatcher.cpp train.h
	g++ -c dispatcher.cpp -g -o dispatcher

station: station.cpp train.h
	g++ -c station.cpp -g -o station

train: train.cpp train.h
	g++ -c train.cpp -g -o train

.PHONY clean:
clean:
	-rm train station dispatcher trnschdl
	-rm -rf *.o *.exe
	-rm -rf trnschdl.dSYM
