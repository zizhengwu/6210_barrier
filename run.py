from subprocess import call
import sys


def openmp_mpi(process_num, thread_num):
    print(sys.argv[1])
    call(["make", "hello_openmp_mpi"])
    call(["/opt/openmpi-1.4.3-gcc44/bin/mpirun", "-np", str(process_num), "hello_openmp_mpi", str(process_num), str(thread_num)])


def openmp_counter(thread_num):
    call(["make", "gtmp_counter"])
    call(["./gtmp_counter",str(thread_num)])


def openmp_mcs(thread_num):
    call(["make", "gtmp_mcs"])
    call(["./gtmp_mcs",str(thread_num)])


def mpi_tournament(process_num):
    call(["make", "gtmpi_tournament"])
    call(["/opt/openmpi-1.4.3-gcc44/bin/mpirun", "-np", str(process_num), "gtmpi_tournament", str(process_num)])

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("usage: python run.py openmp_mpi process_num thread_num")
        print("usage: python run.py openmp_counter thread_num")
        print("usage: python run.py openmp_mcs thread_num")
        print("usage: python run.py mpi_tournament process_num")
    elif sys.argv[1] == "openmp_mpi":
        if len(sys.argv) < 4:
            print("usage: python run.py openmp_mpi process_num thread_num")
        else:
            openmp_mpi(sys.argv[2], sys.argv[3])
    elif sys.argv[1] == "openmp_counter":
        if len(sys.argv) < 3:
            print("usage: python run.py openmp_counter thread_num")
        else:
            openmp_counter(sys.argv[2])
    elif sys.argv[1] == "openmp_mcs":
        if len(sys.argv) < 3:
            print("usage: python run.py openmp_mcs thread_num")
        else:
            openmp_mcs(sys.argv[2])
    elif sys.argv[1] == "mpi_tournament":
        if len(sys.argv) < 3:
            print("usage: python run.py mpi_tournament process_num")
        else:
            mpi_tournament(sys.argv[2])
    else:
        print("usage: python run.py openmp_mpi process_num thread_num")
        print("usage: python run.py openmp_counter thread_num")
        print("usage: python run.py openmp_mcs thread_num")
        print("usage: python run.py mpi_tournament process_num")
