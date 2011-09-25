GCC=gcc
CFLAGS=-g3 -pg -std=c99 -I gensrc -I src 

all: interpreter

gensrc/lexer.c: src/lexer.lex
	flex -o gensrc/lexer.c src/lexer.lex 

gensrc/parser.c: src/parser.y
	bison -d src/parser.y -o gensrc/parser.c

gensrc/parser.h: gensrc/parser.c

o/lexer.o: gensrc/lexer.c gensrc/parser.h
	$(GCC) $(CFLAGS) -c gensrc/lexer.c -lfl -o o/lexer.o

o/parser.o: gensrc/parser.c gensrc/parser.h
	$(GCC) $(CFLAGS) -c gensrc/parser.c -o o/parser.o

o/stack.o: src/stack.c src/stack.h
	$(GCC) $(CFLAGS) -c src/stack.c -o o/stack.o

o/map.o: src/map.c src/map.h
	$(GCC) $(CFLAGS) -c src/map.c -o o/map.o

o/memory.o: src/memory.c src/memory.h
	$(GCC) $(CFLAGS) -c src/memory.c -o o/memory.o

o/error.o: src/error.c src/error.h
	$(GCC) $(CFLAGS) -c src/error.c -o o/error.o

interpreter: src/interpreter.c src/interpreter.h o/lexer.o o/parser.o o/stack.o o/map.o o/memory.o o/error.o
	$(GCC) $(CFLAGS) src/interpreter.c o/lexer.o o/parser.o o/stack.o o/map.o o/memory.o o/error.o -o interpreter -lfl

clean: 
	rm o/* 	gensrc/* interpreter

