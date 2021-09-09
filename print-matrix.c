//---------------------------------------------------------------------------------------//
/* Eric Vo
 * ECE 4730 - Dr. Ligon
 * Cannon's Algorithm 
 * 
 * print-matrix.c
 *
 * reads in row size, col size, and 2D array of doubles from binary files
 * prints 2D array 
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

int main (int argc, char *argv[]){
  int i;
  int rows;
  int cols;
  double **graph;

  //set default file name unless one is given
  char *input_file = "default-make-matrix-file.dat";
  if(argc != 1){
  	input_file = argv[1];
  }

  //read in file
  Read_Graph(input_file, &rows, &cols, &graph);
  
  //print matrix
  Print_Graph(rows, cols, graph);

  for (i = 0; i < rows; i++)
  {
    free(graph[i]); // free memory
  }
  free(graph); // free memory

  return 0;
}
