.phony all:
all: smrtshll inf args

smrtshll: smrtshll.c
	gcc smrtshll.c -g -lreadline -lhistory -ltermcap -o smrtshll

inf: inf.c
	gcc inf.c -o inf

args: args.c
	gcc args.c -o args

.PHONY clean:
clean:
	-rm args inf smrtshll
	-rm -rf *.o *.exe
	-rm -rf smrtshll.dSYM
