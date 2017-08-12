CC = g++
CFLAGS = -std=c++11 -g -Wall -O0
MOREFLAGS = 

all: tl

s_expression.o: s_expression.cpp s_expression.hpp
	$(CC) $(CFLAGS) $(MOREFLAGS) -c s_expression.cpp -o $@

util.o: util.cpp util.hpp
	$(CC) $(CFLAGS) $(MOREFLAGS) -c util.cpp -o $@

cell.o: cell.cpp cell.hpp
	$(CC) $(CFLAGS) $(MOREFLAGS) -c cell.cpp -o $@

closure.o: closure.cpp closure.hpp util.o cell.o
	$(CC) $(CFLAGS) $(MOREFLAGS) -c closure.cpp -o $@

tl: tl.cpp util.o s_expression.o cell.o closure.o
	$(CC) $(CFLAGS) $(MOREFLAGS) $^ -o $@

clean:
	rm tl s_expression.o cell.o closure.o util.o tl
