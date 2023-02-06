MyLiteDB: REPL.o
	gcc -o MyLiteDB REPL.o

REPL.o: REPL.c
	gcc -c REPL.c