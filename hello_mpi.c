#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include "mpi.h"
#include "gtmpi.h"
#include <time.h>

int main(int argc, char **argv)
{
  struct timespec ts0;
  int req_processes = strtol(argv[1], NULL, 10);
  int my_id, num_processes;
  struct utsname ugnm;

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
  clock_gettime(CLOCK_REALTIME, &ts0);
  fprintf(stderr, "initial: %ld \n", ts0.tv_nsec);

  int i;
  for (i = 0; i < 10000; ++i)
  {
    gtmpi_init(req_processes);
    gtmpi_barrier();
  }
  // fprintf(stderr, "thread %d exiting barrier\n", my_id);

  clock_gettime(CLOCK_REALTIME, &ts0);
  fprintf(stderr, "exit: %ld \n", ts0.tv_nsec);

  MPI_Finalize();
  gtmpi_finalize();
  return 0;
}
