//---------------------------------------------------------------------------------------//
/* Eric Vo
 * ECE 4730 - Dr. Ligon
 * Cannon's Algorithm 
 * 
 * graph.c
 *
 * This file contains functions that writes, reads, and prints matrices
 * 
 * Format for files:
 * First two words are 32-bit int's containing the size of "row" and "col"
 * Also stores 2D array of "doubles"
 */
//---------------------------------------------------------------------------------------//

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <mpi.h>
 #include "MyMPI.h"
 #include "graph.h"

//---------------------------------------------------------------------------------------//
/* Writes_Serial_Graph() - stores row, col, and 2D array in a binary file                */
//---------------------------------------------------------------------------------------//

 void Writes_Serial_Graph(char *name_of_file, int r, int c, double **A){ // Writes Graph
   int i;
   int j;

   //write to binary file
   FILE *pointer = fopen(name_of_file, "wb");
   if (pointer == NULL){
     fprintf(stderr, "Could not open: %s\n", name_of_file);
     exit(-1);
   }

   fprintf(stdout, "writing matrix to file: %s\n", name_of_file);

   //store row and col
   fwrite(&r, sizeof(int), 1, pointer);
   fwrite(&c, sizeof(int), 1, pointer);
   
   //store 2D matrix of doubles
   for (i = 0; i < r; i++){
     for (j = 0; j < c; j++){
       fwrite(&A[i][j], sizeof(double), 1, pointer);
     }
   }
   fclose(pointer);
 }

//---------------------------------------------------------------------------------------//
// Writes_Graph_Parallel - Writes matrix for parallel code
//---------------------------------------------------------------------------------------//
 void Writes_Parallel_Graph(char *name_of_file, int r, int c, double *A){ // Writes Graph
   int i;

   //write to binary file
   FILE *pointer = fopen(name_of_file, "wb");
   if (pointer == NULL){
     fprintf(stderr, "Could not open: %s\n", name_of_file);
     exit(-1);
   }

   fprintf(stdout, "writing matrix to file: %s\n", name_of_file);

   //store row and col
   fwrite(&r, sizeof(int), 1, pointer);
   fwrite(&c, sizeof(int), 1, pointer);
   
   //store 2D matrix of doubles
   for (i = 0; i < r*c; i++){
     fwrite(&A[i], sizeof(double), 1, pointer);
   }
   fclose(pointer);
 }

//---------------------------------------------------------------------------------------//
/* Read_Graph - reads in row and col size, and a 2D array of doubles                     */
//---------------------------------------------------------------------------------------//

void Read_Graph(char *name_of_file, int *r, int *c, double ***A){

  int i;
  int j;
  int rows;
  int cols;
  double **graph = NULL;
  FILE *pointer2;

  fprintf(stdout, "Reading matrix from file: %s\n", name_of_file);

  //check and open binary file
  pointer2 = fopen(name_of_file, "rb");
  if (pointer2 == NULL){
    fprintf(stderr, "Could not open: %s\n", name_of_file);
    exit(-1);
  }

  //read in row and col size
  fread(&rows, sizeof(int), 1, pointer2);
  fread(&cols, sizeof(int), 1, pointer2);
  
  //allocate memory for matrix
  graph = (double **) malloc(rows * sizeof(double *));
  for (i = 0; i < rows; i++){
    graph[i] = (double *) malloc(cols * sizeof(double));
  }

  //read in 2D array of doubles
  for (i = 0; i < rows; i++){ 
    for (j = 0; j < cols; j++){
      fread(&graph[i][j], sizeof(double), 1, pointer2);
    }
  }
  fclose(pointer2);

  *r = rows, *c = cols, *A = graph; // returns array and #
}

//---------------------------------------------------------------------------------------//
/* Print_Graph() - prints the 2D array of doubles                                        */
//---------------------------------------------------------------------------------------//

void Print_Graph(int r, int c, double **A){ // Prints matrix

  int i;
  int row;
  int col;

  fprintf(stdout, "Array is a %d x %d Matrix\n\n", r, c);

  fprintf(stdout, "%5s", "|\t");
  for (i = 0; i < c; i++){
    fprintf(stdout, "%5d\t", i);
  }
  fprintf(stdout, "\n");

  fprintf(stdout, "%5s", "|\t");
  for (i = 0; i < c; i++){
    fprintf(stdout, "%s", " -----\t");
  }
  fprintf(stdout, "\n");

  for (row = 0; row < r; row++){

    fprintf(stdout, "%3d%2s", row, "|\t");

    for (col = 0; col < c; col++){
      fprintf(stdout, "%.2f\t ", A[row][col]);
    }
    fprintf(stdout, "\n");
  }

}

