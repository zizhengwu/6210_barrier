#PBS -q cs6210
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:05:00
#PBS -N Zizheng.openmp
cd 6210_barrier/
for i in `seq 2 8`;
do
  >&2 echo "openmp_counter "$i
  python run.py openmp_counter $i
done
for i in `seq 2 8`;
do
  >&2 echo "openmp_mcs "$i
  python run.py openmp_mcs $i
done
