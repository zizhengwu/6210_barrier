#PBS -q cs6210
#PBS -l nodes=12:sixcore
#PBS -l walltime=00:05:00
#PBS -N Zizheng.openmpi
cd 6210_barrier/
for i in `seq 2 12`;
do
  >&2  echo "mpi_tournament "$i
  python run.py mpi_tournament $i
done
for i in `seq 2 12`;
do
  >&2  echo "mpi_dissemination "$i
  python run.py mpi_dissemination $i
done
