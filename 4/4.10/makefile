all: main

main: main.c operations.c validation.c errors.c interpreter.c Trie.c types.c read_file.c write_settings.c debug.c
	gcc -g main.c operations.c validation.c errors.c interpreter.c Trie.c types.c read_file.c write_settings.c debug.c -o main -lm

clean:
	rm -rf *.o main