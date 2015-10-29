cmake_minimum_required(VERSION 3.3)
project(cs6210_barrier)

find_package(OpenMP)
find_package(MPI)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_executable(openmp hello_openmp.c gtmp_mcs.c)
add_executable(mpi hello_mpi.c gtmpi_dissemination.c)

target_link_libraries(openmp ${OpenMP_C_FLAGS} m)
target_link_libraries(mpi ${MPI_C_LIBRARIES})