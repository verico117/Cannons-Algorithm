//---------------------------------------------------------------------------------------//
/* Eric Vo
 * ECE 4730 - Dr. Ligon
 * Cannon's Algorithm 
 * 
 * mm-parallel.c : this file takes in two inputs, input1 and input2, and multiplies them
 *                 using Cannon's algorithm. 
 *
 */
//---------------------------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "graph.h"
#include <mpi.h>
#include "MyMPI.h"

int main (int argc, char *argv[]){	
	int i, j, k;
	int left, right, up, down;
	int rank, size, rsize;
	int shift;
	int gridsz[2];
	int period[2];
	int r1, c1, r2, c2, n;
	double **a, **b, **c;
	double *sa, *sb, *sc;
	double *buff, *temp;
	double cannonTime;
	MPI_Comm cart_comm;
	MPI_Status status;
	
	if(argc != 4){
		printf("Error: Invalid number of inputs (input1 input2 output)\n");
		exit(-1);
	}
	
	MPI_Init(&argc, &argv);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	rsize = (int) sqrt(size);
	if(rsize*rsize != size){
		printf("Error: Invalid number of processors (must be square)\n");
		MPI_Finalize();
		exit(-1);
	}
	
	gridsz[0] = 0;
	gridsz[1] = 0;
	period[0] = 1;
	period[1] = 1;
	MPI_Dims_create(size, 2, gridsz);

	
	//MPI_Barrier(MPI_COMM_WOLRD);	
	MPI_Cart_create(MPI_COMM_WORLD, 2, gridsz, period, 0, &cart_comm);

	// read in data for A and B matrix
	read_col_striped_matrix(argv[1], (void ***) &a, (void **) &sa, MPI_DOUBLE, &r1, &c1, cart_comm);
	read_col_striped_matrix(argv[2], (void ***) &b, (void **) &sb, MPI_DOUBLE, &r2, &c2, cart_comm);
	
	n = r1;

	// allocate memory for resulting matrix, c
	c = (double **) malloc(n * sizeof(double *));
	for(i = 0; i < n; i++){
		c[i] = (double *) malloc(n * sizeof(double));
	}
	sc = (double *)calloc(n*n,sizeof(double));
	buff = (double *)malloc(n*n*sizeof(double));
	
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			sc[i*n+j]= 0.0;
		}
	}
	

	MPI_Cart_shift(cart_comm, 0, 1, &left, &right);
	MPI_Cart_shift(cart_comm, 1, 1, &up, &down);

	MPI_Barrier(cart_comm);
	cannonTime = -MPI_Wtime();

	// cannon's algorithm
	for(shift = 0; shift < gridsz[0]; shift++){
		for(i = 0; i < n; i++){
			for(j = 0; j < n; j++){
				for(k = 0; k < n; k ++){
					sc[i*n+k] = sc[i*n+k] + sa[i*n+j]*sb[j*n+k];
				}
			}
		}
		if(shift==gridsz[0]-1) break;	
	
		MPI_Sendrecv(sa, n*n, MPI_DOUBLE, left, 1, buff, n*n, MPI_DOUBLE, right, 1, cart_comm, &status);
		temp = buff;
		buff = sa;
		sa = temp;
		MPI_Sendrecv(sb, n*n, MPI_DOUBLE, up, 2, buff, n*n, MPI_DOUBLE, down, 2, cart_comm, &status);
		temp = buff;
		buff = sb;
		sb = temp;		
	}

	MPI_Barrier(cart_comm);
	cannonTime += MPI_Wtime();

	Writes_Parallel_Graph(argv[3], n, n, sc);

	printf("Cannon's Algorithm Time: %.5f\n", cannonTime);

	MPI_Finalize();


	for(i = 0; i < n; i++){
		free(c[i]);
	}	
	free(c);
	free(buff);
	return 0;
}

