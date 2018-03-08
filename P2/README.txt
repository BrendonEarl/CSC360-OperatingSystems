# Welcome to my Train Scheduler

## A little about me
Name: Brendon Earl  
Student #: V00797149  

## And why I wrote this code
Course: CSC 360 (Operating Systems)  
Assignment: #2 - Train Scheduler  

## How to use this thing
Ensure you have a C compiler setup  
run `make all` and let the makefile build it  
run it by entering `./trnschdl <train_schedule_input.txt>`  
where `<train_schedule_input.txt>` is a local file in the 
format specified in the input section

## Input
```
D0 LT0 CT0
D1 LT1 CT1
.
.
.
DN LTN CTN
```

Where D is the direction and priority, being:  
E -> East, high priority,  
e -> East, low priority,  
W -> West, high priority,  
w -> west, low priority  
and N is the train number parsed from the input file in order.

## Expected behaviour  
If the input file is mis-shaped, the trnshdl program is not intended to handle it 
__it will crash__.  
The output should look something like:  
```
00:00:01:2 Train 10 queued and ready to go East
00:00:01:4 Train 9  is OFF the main track after going West
00:00:01:4 Train 10 is ON the main track going East
00:00:01:8 Train 3  queued and ready to go East
00:00:01:8 Train 6  queued and ready to go West
00:00:02:0 Train 10 is OFF the main track after going East
```

According to the assignment spec, the highest priority of train is let across the bridge.
If two trains of equal priority are on the same side, the one which arrived their first is 
given priority. If two trains of equal priority are on either side of the bridge, the 
train on the opposite side of the source of the last train to pass over the bridge is 
given priority. Prior to being allowed to queue to cross the bridge, the trains must first 
'load'.

__Happy Marking!__
