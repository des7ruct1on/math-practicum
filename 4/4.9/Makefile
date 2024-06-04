CC = gcc
DEBUG = gdb
OUT_DIR = src/out
INCLUDE_DIR = include
MAIN_SOURCES = src/main.c
GLOBALS_SOURCES = $(wildcard $(INCLUDE_DIR)/*/*.c)
GEN_SOURCES = src/random.c
GEN_OUT = src/out/random.out
FILE_OUT = src/out/main.out
FLAGS = -lm

all: 
	$(CC) $(MAIN_SOURCES) $(GLOBALS_SOURCES) $(FLAGS)  -o $(FILE_OUT)
	$(CC) $(GLOBALS_SOURCES) $(GEN_SOURCES) $(FLAGS)  -o $(GEN_OUT)

main: $(MAIN_SOURCES)
	$(CC) $(MAIN_SOURCES) $(GLOBALS_SOURCES) $(FLAGS) -o $(FILE_OUT)

generate: $(GEN_SOURCES)
	$(CC) $(GLOBALS_SOURCES) $(GEN_SOURCES) $(FLAGS)  -o $(GEN_OUT)

clean:
	rm -f src/out/*.out

cmp: main

run_main: main
	./$(FILE_OUT) $(ARGS)

run_generate: generate
	./$(GEN_OUT) $(FILE)
