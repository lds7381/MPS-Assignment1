#include <mpi.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Prompts user to enter three numbers. The given 'numbers' parameter must have
 * space for three numbers.
 */
void prompt( int *numbers )
{
  char line[80];
  int rc = 0;

  while (rc != 3) {
    printf( "Enter three numbers, separated by whitespace: " );
    fflush(NULL);

    if (fgets( line, 80, stdin ) == NULL) {
      MPI_Abort( MPI_COMM_WORLD, MPI_ERR_UNKNOWN );
    }

    rc = sscanf( line, "%d %d %d", numbers, numbers + 1, numbers + 2 );

    if (rc == EOF) {
      MPI_Abort( MPI_COMM_WORLD, MPI_ERR_UNKNOWN );
    }
  }
  printf("\n");
}

/**
 * Aborts MPI programs.
 */
void abortHandler( int signal )
{
  MPI_Abort( MPI_COMM_WORLD, MPI_ERR_UNKNOWN );
  _exit( 1 );
}

/**
 * Demonstrates communication between processes, using the broadcast, send, and
 * receive functions.
 */
int main( int argc, char **argv )
{
  int numtasks, rank, rc, *numbers, i, number;
  MPI_Status status;

  rc = MPI_Init( &argc, &argv );
  if (rc != MPI_SUCCESS) {
    fprintf( stderr, "Error starting MPI.\n" );
    MPI_Abort( MPI_COMM_WORLD, rc );
  }

  signal( SIGINT,  abortHandler );
  signal( SIGTERM, abortHandler );

  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &numtasks );

  numbers = (int*) malloc( sizeof(int) * 3 );

  if (rank == 0) {
    if (numtasks < 3) {
      printf( "\nRun this program with 3 or more processes.\n\n" );
      MPI_Abort( MPI_COMM_WORLD, MPI_ERR_UNKNOWN );
    }

    if (numtasks > 4) {
      printf(
"=Rank 0= The implementation of MPI on rocks.ce.rit.edu does not handle the\n"
"         stdin stream well, so the root task is just going to transmit the\n"
"         numbers (14, 32, 67) to each subtask.\n" );
      fflush(stdout);
      numbers[0] = 14; numbers[1] = 32; numbers[2] = 67;
    } else {
      prompt( numbers );
    }
  }

  MPI_Bcast( numbers, 3, MPI_INT, 0, MPI_COMM_WORLD );

  printf( "=Rank %d= numbers: %d %d %d\n",
          rank, numbers[0], numbers[1], numbers[2] );

  MPI_Barrier( MPI_COMM_WORLD );

  if (rank == 0) {
    printf("\n");
    for (i = 1; i < numtasks; i++) {
      MPI_Recv( &number, 1, MPI_INT,
                MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                &status );
      printf( "=Rank 0= Received message from rank %d:\n", status.MPI_SOURCE );
      printf( "=Rank 0=   number changed to %d\n", number );
    }
    printf("\n"
           "Notice how, if the random number generator is not seeded, all\n"
           "random numbers are the same!\n");
  } else {
    number = rand();
    MPI_Send( &number, 1, MPI_INT, 0, 1, MPI_COMM_WORLD );
  }

  free( numbers );
  MPI_Finalize();
}
