//---------------------------------------------------------------------------------------//
/* Eric Vo
 * ECE 4730 - Dr. Ligon
 * Cannon's Algorithm 
 * 
 * mm-serial.c : this file takes in two inputs, input1 and input2, and multiplies them
 * 		 in serial. This works in row dominant order.  
 *
 */
//---------------------------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <time.h>
#include "graph.h"
#include <mpi.h>

int main (int argc, char *argv[]){
  int i;
  int rows1;
  int cols1;
  int rows2;
  int cols2;
  double **matrix1;
  double **matrix2;
  double **result;
  int x;
  int y;
  int r;
  int c;
  double mtime;

  //set default file names, unless names given
  char *input_file1 = "default-make-matrix-file.dat";
  char *input_file2 = "default-make-matrix-file.dat";
  char *output_file = "default-resulting-matrix-file.dat";
  if(argc == 4){
  	input_file1 = argv[1];
	input_file2 = argv[2];
	output_file = argv[3];
  }else if(argc == 3){
  	input_file1 = argv[1];
	input_file2 = argv[2];
  }else{
  	printf("Setting default file names\n");
  }

  MPI_Init(&argc, &argv);

  //read in and print first matrix from file
  Read_Graph(input_file1, &rows1, &cols1, &matrix1);

  //read in and print second matrix from file
  Read_Graph(input_file2, &rows2, &cols2, &matrix2);

  //set dimensions for resulting matrix
  r = rows1;
  c = cols2;

  //allocate space for the resulting matrix
  result = (double **) malloc(r * sizeof(double *));
  for(i = 0; i < r; i++)
  {
  	result[i] = (double *) malloc(c * sizeof(double));
  }

  //check to see if the two matrices are compatible for multiplication
  if(cols1 != rows2)
  {
  	printf("Error: The two input matrices cannot be multiplied (incompatible dimensions)\n");
	MPI_Finalize();
	exit(-1);
  }
  
  //Start timing
  MPI_Barrier(MPI_COMM_WORLD);
  mtime = -MPI_Wtime();
  
  //calculate matrix multiplication
  int a;
  for(x = 0; x < rows1; x++)
  {
  	for(y = 0; y < cols2; y++)
	{
		for(a = 0; a < cols1; a++)
		{
			result[x][y] = (matrix1[x][a] * matrix2[a][y]) + result[x][y];
		}
	}
  }

  //End timing
  mtime += MPI_Wtime();

  Writes_Serial_Graph(output_file, r, c, result);

  printf("Multiplication takes %.5f s\n", mtime);

  for (i = 0; i < rows1; i++)
  {
    free(result[i]); // free memory
  }
  free(result); // free memory
  
  for (i = 0; i < rows1; i++)
  {
    free(matrix1[i]); // free memory
  }
  free(matrix1); // free memory

  for (i = 0; i < rows2; i++)
  {
    free(matrix2[i]); // free memory
  }
  free(matrix2); // free memory

  MPI_Finalize();

  return 0;
}

