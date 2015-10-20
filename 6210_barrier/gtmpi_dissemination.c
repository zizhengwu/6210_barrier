#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "gtmpi.h"
#include <math.h>

/*
    From the MCS Paper: The scalable, distributed dissemination barrier with only local spinning.

    type flags = record
        myflags : array [0..1] of array [0..LogP - 1] of Boolean
    partnerflags : array [0..1] of array [0..LogP - 1] of ^Boolean
    
    processor private parity : integer := 0
    processor private sense : Boolean := true
    processor private localflags : ^flags

    shared allnodes : array [0..P-1] of flags
        //allnodes[i] is allocated in shared memory
    //locally accessible to processor i

    //on processor i, localflags points to allnodes[i]
    //initially allnodes[i].myflags[r][k] is false for all i, r, k
    //if j = (i+2^k) mod P, then for r = 0 , 1:
    //    allnodes[i].partnerflags[r][k] points to allnodes[j].myflags[r][k]

    procedure dissemination_barrier
        for instance : integer :0 to LogP-1
        localflags^.partnerflags[parity][instance]^ := sense
        repeat until localflags^.myflags[parity][instance] = sense
    if parity = 1
        sense := not sense
    parity := 1 - parity


MPI_Request sendReq;
MPI_Request recReq;
MPI_Status sendStat;
MPI_Status recStat;

*/
int tNum;
int rounds; 

void gtmpi_init(int num_threads){
    tNum = num_threads;
    rounds = ceil(log(tNum)/log(2));
}

void gtmpi_barrier(){
    int i = 0, myID, toID, fromID;
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);
    for (i = 0; i < rounds; i++)
    {
        toID = (myID + (int)pow(2, i)) % tNum;
        fromID = myID - (int)pow(2, i);
        if (fromID < 0)
            fromID += tNum;
        
        MPI_Send(NULL, 0, MPI_INT, toID, 0, MPI_COMM_WORLD);
        MPI_Recv(NULL, 0, MPI_INT, fromID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }   
}

void gtmpi_finalize(){

}