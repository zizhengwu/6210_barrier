ADDITIONALFLAGS = -lm -lrt
#OpenMP Flags Etc.
OMPFLAGS = -g -Wall -fopenmp -DLEVEL1_DCACHE_LINESIZE=`getconf LEVEL1_DCACHE_LINESIZE`
OMPLIBS = -lgomp
CC = gcc

#MPI Flags Etc (may need to customize)
#MPICH = /usr/lib64/openmpi/1.4-gcc
MPIFLAGS = -g -Wall #-I$(MPICH)/include
MPILIBS =
# MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc
MPICC = mpicc

hello_openmp: hello_openmp.c
	$(CC) $(OMPFLAGS) -o $@ $^ $(OMPLIBS) $(ADDITIONALFLAGS)

hello_mpi: hello_mpi.c
	$(MPICC) $(MPIFLAGS) -o $@ $^ $(MPILIBS) $(ADDITIONALFLAGS)

gtmp_counter: gtmp_counter.c hello_openmp.c
	$(CC) $(OMPFLAGS) -o $@ $^ $(OMPLIBS) $(ADDITIONALFLAGS)

gtmp_mcs: gtmp_mcs.c hello_openmp.c
	$(CC) $(OMPFLAGS) -o $@ $^ $(OMPLIBS) $(ADDITIONALFLAGS)

gtmpi_counter: gtmpi_counter.c hello_mpi.c
	$(MPICC) $(MPIFLAGS) -o $@ $^ $(MPILIBS) $(ADDITIONALFLAGS)

gtmpi_tournament: gtmpi_tournament.c hello_mpi.c
	$(MPICC) $(MPIFLAGS) -o $@ $^ $(MPILIBS) $(ADDITIONALFLAGS)

gtmpi_dissemination: gtmpi_dissemination.c hello_mpi.c
	$(MPICC) $(MPIFLAGS) -o $@ $^ $(MPILIBS) $(ADDITIONALFLAGS)

hello_openmp_mpi: hello_openmp_mpi.c gtmpi_tournament.c gtmp_mcs.c
	$(MPICC) $(MPIFLAGS) $(OMPFLAGS) -o $@ $^ $(MPILIBS) $(OMPLIBS) $(ADDITIONALFLAGS)

clean:
	rm -rf *.o hello_openmp hello_mpi gtmp_counter gtmpi_counter gtmp_mcs gtmpi_tournament hello_openmp_mpi gtmpi_dissemination
