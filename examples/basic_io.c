#include <mpi.h>
#include <stdio.h>

int main( int argc, char **argv )
{
  int numtasks, rank, dest, source, rc, count, tag = 1;
  char inmsg, outmsg;
  MPI_Status stat;

  MPI_Init( &argc, &argv );
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Task %d: num tasks = %d\n", rank, numtasks );

  if (rank == 0) {
    dest = 1;
    source = 1;
    rc = MPI_Send( &outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD );
    rc = MPI_Recv( &inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &stat );
  } else if (rank == 1) {
    dest = 0;
    source = 0;
    rc = MPI_Recv( &inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &stat );
    rc = MPI_Send( &outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD );
  }

  printf( "Task %d: Received %d from task %d with tag %d\n",
          rank, (int) inmsg, stat.MPI_SOURCE, stat.MPI_TAG );

  MPI_Finalize();
}
