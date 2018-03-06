.phony all:
all: trnschdl dispatcher station train

trnschdl: trnschdl.cpp
	g++ trnschdl.cpp -g -o trnschdl

dispatcher: dispatcher.cpp
	g++ -c dispatcher.cpp -g -o dispatcher

station: station.cpp
	g++ -c station.cpp -g -o station

train: train.cpp
	g++ -c train.cpp -g -o train

.PHONY clean:
clean:
	-rm train station dispatcher trnschdl
	-rm -rf *.o *.exe
	-rm -rf trnschdl.dSYM
