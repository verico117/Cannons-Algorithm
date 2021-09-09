//---------------------------------------------------------------------------------------//
/* Eric Vo
 * ECE 4730 - Dr. Ligon
 * Cannon's Algorithm 
 * 
 * graph.h
 *
 * This file contains declarations for functions that writes, reads, and prints matrices
 * 
 */
//---------------------------------------------------------------------------------------//
#define ROOT 0

void Writes_Serial_Graph(char *name_of_file, int r, int c, double **A);

void Writes_Parallel_Graph(char *name_of_file, int r, int c, double *A);

void write_checkerboard_matrix (char *, void **,
   MPI_Datatype,int, int, MPI_Comm);

int Check_Vertices(int vertex_a, int vertex_b);

void Read_Graph(char *name_of_file, int *r, int *c, double ***A);

void Print_Graph(int r, int c, double **A);

