# Jinx
To submit jobs to Jinx, please `cd 6210_barrier`. 
Modify `Makefile`, comment line `MPICC = mpicc` and uncomment line `MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc`. 
Modify `run.py`, comment line `mpirun = "mpirun"` and comment line `mpirun = "/opt/openmpi-1.4.3-gcc44/bin/mpirun"`. 
At last, `qsub openmp.sh`, or `qsub openmpi.sh`, or `qsub mp_mpi.sh`.

# Local
To run barriers on local machine, please `cd 6210_barrier`. Then you can run barriers through the script `run.py`.

    python run.py openmp_mpi process_num thread_num
    python run.py openmp_counter thread_num
    python run.py openmp_mcs thread_num
    python run.py mpi_tournament process_num
    python run.py mpi_dissemination process_num

Example: `python run.py openmp_mpi 2 3`, or `python run.py openmp_mcs 3`
