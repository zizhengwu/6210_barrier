#include <omp.h>
#include "gtmp.h"
#include "stdio.h"
#include <stdbool.h>
/*
    From the MCS Paper: A sense-reversing centralized barrier

    shared count : integer := P
    shared sense : Boolean := true
    processor private local_sense : Boolean := true

    procedure central_barrier
        local_sense := not local_sense // each processor toggles its own sense
	if fetch_and_decrement (&count) = 1
	    count := P
	    sense := local_sense // last processor toggles global sense
        else
           repeat until sense = local_sense
*/
static int count;
static bool sense;

void gtmp_init(int num_threads){
    count = num_threads;
    sense = true;
}

void gtmp_barrier(){
    bool local_sense = false;
    // fprintf(stderr, "I am thread %d\n", omp_get_thread_num());
    if (__sync_fetch_and_sub(&count, 1) == 1)
    {
        // fprintf(stderr, "fetch_and_sub %d\n", count);
        count = omp_get_num_threads();
        sense = local_sense;
    }
    else
    {
        while(local_sense != sense);
    }
}

void gtmp_finalize(){

}
