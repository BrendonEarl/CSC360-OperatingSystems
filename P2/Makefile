.phony all:
all: trnschdl entry schedule dispatcher station train

trnschdl: trnschdl.cpp
	g++ trnschdl.cpp -g -o trnschdl

entry: entry.cpp
	g++ entry.cpp -g -o entry

schedule: schedule.cpp
	g++ schedule.cpp -g -o schedule

dispatcher: dispatcher.cpp
	g++ dispatcher.cpp -g -o dispatcher

station: station.cpp
	g++ station.cpp -g -o station

train: train.cpp
	g++ train.cpp -g -o train

.PHONY clean:
clean:
	-rm train station dispatcher schedule entry trnschdl
	-rm -rf *.o *.exe
	-rm -rf trnschdl.dSYM
