# constants
CC = gcc
FLAGS = -g -Wall -pedantic -ansi -Wno-long-double 
GCC_LIBS = -lgsl -lgslcblas -lm

# generate binary of test program by linking .o files 
run: onn.o gaonn.o  
	$(CC) $(FLAGS) onn.o gaonn.o -o run 

# generate object files via compilation with -c
onn.o: onn.c onn.h 
	$(CC) $(FLAGS) -c onn.c 

# remove temporary files
clean:
	rm -f run *.swp *.o core *~ Makefile.bak
