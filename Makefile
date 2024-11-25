### 
## Makefile skeleton
## INFO0030: Projet 1
###

## Variables

# Tools & flags
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes
LD=gcc
LDFLAGS=

# Files
EXEC=pnm
MODULES=main.c pnm.c
OBJECTS=main.o pnm.o
SRC_FILES=$(MODULES)
PROG=$(EXEC)

## Rules

.PHONY: all clean valgrind gdb cppcheck

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LD) -o $(EXEC) $(OBJECTS) $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

pnm.o: pnm.c
	$(CC) -c pnm.c -o pnm.o $(CFLAGS)

valgrind:
	valgrind --track-origins=yes --leak-check=full ./$(PROG)

gdb:
	gdb ./$(PROG)

cppcheck:
	cppcheck --enable=all --check-config --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)
	cppcheck --enable=warning,style,performance,portability,information --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)
	cppcheck --enable=unusedFunction --check-config --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)
	cppcheck --enable=performance --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)
	cppcheck --enable=all --inconclusive --verbose --force -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)
	cppcheck --enable=style --inconclusive --verbose -I. --suppress=missingIncludeSystem --output-file=cppcheck_report.txt $(SRC_FILES)
	cppcheck --enable=all --inconclusive --verbose --suppress=missingIncludeSystem -I. --output-file=cppcheck_report.txt $(SRC_FILES)
	cppcheck --enable=style --inconclusive --verbose --suppress=missingIncludeSystem -I. --output-file=cppcheck_report.txt $(SRC_FILES)


clean:
	rm -f *.o $(EXEC) *~ cppcheck_report.txt
