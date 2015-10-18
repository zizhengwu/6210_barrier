#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include "mpi.h"
#include "gtmpi.h"

int main(int argc, char **argv)
{
  int req_processes = 7;
  int my_id, num_processes;
  struct utsname ugnm;

  gtmpi_init(req_processes);
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  if(num_processes != req_processes){
    fprintf(stderr, "Wrong number of processes.  Required: %d, Actual: %d\n",
        req_processes, num_processes);
    exit(1);
  }

  uname(&ugnm);

  // fprintf(stderr, "Hello World from thread %d of %d, running on %s.\n", my_id, num_processes, ugnm.nodename);
  gtmpi_barrier();
  // fprintf(stderr, "thread %d exiting barrier\n", my_id);

  MPI_Finalize();
  gtmpi_finalize();
  return 0;
}
