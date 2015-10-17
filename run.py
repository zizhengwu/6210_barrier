from subprocess import call
call(["make", "hello_openmp_mpi"])
call(["mpirun", "-np", "8", "hello_openmp_mpi", "8", "100"])