CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = graph.o MyMPI.o make-matrix print-matrix mm-serial mm-parallel
MCC = mpicc
CC = gcc

all: $(PROGS)

graph.o: graph.c
		$(MCC) $(CFLAGS) graph.c -c

MyMPI.o: MyMPI.c
		$(MCC) $(CFLAGS) MyMPI.c -c

make-matrix: make-matrix.c
		$(MCC) $(CFLAGS) make-matrix.c graph.o MyMPI.o -o make-matrix

print-matrix: print-matrix.c
		$(MCC) $(CFLAGS) print-matrix.c graph.o MyMPI.o -o print-matrix

mm-serial: mm-serial.c
		$(MCC) $(CFLAGS) mm-serial.c graph.o MyMPI.o -o mm-serial

mm-parallel: mm-parallel.c
		$(MCC) $(CFLAGS) mm-parallel.c graph.o MyMPI.o -o mm-parallel -lm

clean:
		rm -f $(PROGS) *.o core*
