.phony all:
all: trnschdl dispatcher station train

trnschdl: trnschdl.cpp
	g++ trnschdl.cpp -g -o trnschdl

dispatcher: dispatcher.cpp
	g++ dispatcher.cpp -g -o dispatcher

station: station.cpp
	g++ station.cpp -g -o station

train: train.cpp
	g++ train.cpp -g -o train

.PHONY clean:
clean:
	-rm train station dispatcher trnschdl
	-rm -rf *.o *.exe
	-rm -rf trnschdl.dSYM
