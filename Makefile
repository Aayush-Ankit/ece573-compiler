CC	= g++
CFLAGS	= -g -o

entry:  entry.cc  entry.hh
	$(CC) -g -c -o  entry.o  entry.cc

helper:  helper.cc  entry.hh
	$(CC) -g -c -o  helper.o  helper.cc

assem:  assem.cc  assem.hh
	$(CC) -g -c -o  assem.o  assem.cc

scanner:  microLexer.lex  entry.o  helper.o
	flex -o  lex.yy.c  microLexer.lex

parser:  microParser.y  entry.o  helper.o  assem.o
	bison -d -o  microParser.cpp  microParser.y

compiler:  main.cc entry helper assem scanner parser
	$(CC) $(CFLAGS) func.o  microParser.cpp  lex.yy.c  entry.o  helper.o  assem.o  main.cc -lfl

clean:
	rm -f *.hpp *.cpp lex.yy.c* *.o
	rm -f  *.hpp  *.cpp  lex.yy.c*  *.o
