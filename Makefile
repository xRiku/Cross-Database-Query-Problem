CC		:= gcc
CFLAGS	:= -Wall -Wextra -O3

BIN		:= bin
SRC		:= src
INCLUDE	:= include

PROGARGS	:= 3 3 0,1,4 4,0,1 inputs/file1.txt inputs/file2.txt outputs/fileCompare.txt

PROGARGS2	:= 2 4 1,0 0,2 inputs/arquivo1.txt inputs/arquivo2.txt outputs/arquivo.txt

EXECUTABLE	:= trab2
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)

VALGRIND	:= 

CINCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS		:= $(SOURCES:.c=.o)

all: clean $(EXECUTABLE)

.PHONY: clean
clean:
	-$(RM) $(OBJECTS)


run: all
	./$(EXECUTABLE) $(PROGARGS)

run2: all
	./$(EXECUTABLE) $(PROGARGS2)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(CINCLUDES) $^ -o $@ $(LIBRARIES) -lm

val: all
	valgrind ./$(EXECUTABLE) $(PROGARGS)

val2: all
	valgrind ./$(EXECUTABLE) $(PROGARGS2)

full: all
	- valgrind -v --leak-check=full ./$(EXECUTABLE) $(PROGARGS)

full2: all
	- valgrind -v --leak-check=full ./$(EXECUTABLE) $(PROGARGS2)