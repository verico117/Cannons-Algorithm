//---------------------------------------------------------------------------------------//
/* Eric Vo
 * ECE 4730 - Dr. Ligon
 * Cannon's Algorithm 
 * 
 * make-matrix.c
 *
 * creates an row x col  2D matrix and stores it in a binary file
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

void Process_Command_Line(int argc, char *argv[], int *rows, int *cols,
   int *low, int *high, char *ofile[]);

int main (int argc, char *argv[]) 
{
  int i;
  int j;
  int rows = 5;
  int cols = 5;
  int low = 50;
  int high = 500000;
  double **graph = NULL;
  char *output_file = "default-make-matrix-file.dat";
  time_t t;
  double v;

  // process command line arguments
  Process_Command_Line(argc, argv, &rows, &cols, &low, &high, &output_file);

  // allocate memory for 2D matrix of doublles
  graph = (double **) malloc(rows * sizeof(double *));
  for (i = 0; i < rows; i++)
  {
    graph[i] = (double *) malloc(cols * sizeof(double));
  }

  // generate 2D matrix of doubles based on "rows" and "cols"
  // values generated using "u" and "l" (low and high)
  srand(time(&t));
  for(i = 0; i < rows; i++)
  {
  	for(j = 0; j < cols; j++)
	{
		v = ((double)low + ((double)rand() / (high-low))) / 1000.0;
		graph[i][j] = v;
	}
  }

  //write 2D matrix to binary file
  Writes_Serial_Graph(output_file, rows, cols, graph);

  if (strcmp(output_file, "default-make-matrix-file.dat") != 0)
  {
    free(output_file);
  }

  for (i = 0; i < rows; i++)
  {
    free(graph[i]); // Free space
  }
  free(graph); // Free space

  return 0;
}

//Process command line arguments (r, u, l, u, o)
void Process_Command_Line(int argc, char *argv[], int *rows,
   int *cols, int *low, int *high, char *ofile[])
{
    int c;
    while ((c = getopt(argc, argv, "r:c:l:u:o:")) != -1)
    {
      switch(c) // switch statement for case types /converts string
      {
          case 'r': // row size
            *rows = atoi(optarg);
            break;
          case 'c': // col size
            *cols = atoi(optarg);
            break;
          case 'l': // low
            *low = atoi(optarg);
            break;  // upper (or high)
	  case 'u':
	    *high = atoi(optarg);
	    break;
          case 'o': // output file name
            if (strcmp(optarg, "default-make-matrix-file.dat") != 0)
            {
              *ofile = strdup(optarg);
            }
            break;
      }
    }
}
