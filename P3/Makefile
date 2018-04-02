.phony all:
all: diskinfo disklist diskget

diskinfo: diskinfo.c readdisk.c disk.h
	gcc -std=c11 -g -Wall -Wextra -Werror -pedantic-errors -o diskinfo diskinfo.c

disklist: disklist.c readdisk.c disk.h
	gcc -std=c11 -g -Wall -Wextra -Werror -pedantic-errors -o disklist disklist.c

diskget: diskget.c readdisk.c disk.h
	gcc -std=c11 -g -Wall -Wextra -Werror -pedantic-errors -o diskget diskget.c

.PHONY clean:
clean:
	-rm diskinfo
	-rm disklist
	-rm diskget
	-rm -rf *.o *.exe
	-rm -rf diskinfo.dSYM
	-rm -rf disklist.dSYM
	-rm -rf diskget.dSYM
