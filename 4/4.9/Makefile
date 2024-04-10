CC = gcc
CFLAGS = -std=c99 -Wall -Werror 
DEBUG = gdb
OUT_DIR = src/out
INCLUDE_DIR = include
MAIN_SOURCES = src/main.c
GLOBALS_SOURCES = $(wildcard $(INCLUDE_DIR)/*/*.c)
GEN_SOURCES = include/random.c
GEN_OUT = src/out/random.out
FILE_OUT = src/out/main.out

all: 
	$(CC) $(CFLAGS) $(MAIN_SOURCES) $(GLOBALS_SOURCES) -o $(FILE_OUT)
	$(CC) $(CFLAGS) $(GLOBALS_SOURCES) $(GEN_SOURCES) -o $(GEN_OUT)

main: $(MAIN_SOURCES)
	$(CC) $(CFLAGS) $(MAIN_SOURCES) $(GLOBALS_SOURCES) -o $(FILE_OUT)

generate: $(GEN_SOURCES)
	$(CC) $(CFLAGS) $(GLOBALS_SOURCES) $(GEN_SOURCES) -o $(GEN_OUT)

clean:
	rm -f src/out/*.out

cmp: main

run_main: main
	./$(FILE_OUT) $(ARGS)

run_generate: generate
	./$(GEN_OUT) $(FILE)
