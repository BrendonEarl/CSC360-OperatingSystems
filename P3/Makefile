.phony all:
all: diskinfo

diskinfo: diskinfo.c readdisk.c disk.h
	gcc -std=c11 -g -Wall -Wextra -Werror -pedantic-errors -o diskinfo diskinfo.c

.PHONY clean:
clean:
	-rm diskinfo
	-rm -rf *.o *.exe
	-rm -rf diskinfo.dSYM
