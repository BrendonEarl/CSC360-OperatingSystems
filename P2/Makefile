.phony all:
all: trnschdl entry schedule dispatcher station train

trnschdl: trnschdl.cpp
	g++ trnschdl.cpp -g -o trnschdl

entry: entry.cpp
	g++ -c entry.cpp -g -o entry

schedule: schedule.cpp
	g++ -c schedule.cpp -g -o schedule

dispatcher: dispatcher.cpp
	g++ -c dispatcher.cpp -g -o dispatcher

station: station.cpp
	g++ -c station.cpp -g -o station

train: train.cpp
	g++ -c train.cpp -g -o train

.PHONY clean:
clean:
	-rm train station dispatcher schedule entry trnschdl
	-rm -rf *.o *.exe
	-rm -rf trnschdl.dSYM
