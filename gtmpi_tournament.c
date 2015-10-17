#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "gtmpi.h"
#include <math.h>

/*
    From the MCS Paper: A scalable, distributed tournament barrier with only local spinning

    type round_t = record
        role : (winner, loser, bye, champion, dropout)
	opponent : ^Boolean
	flag : Boolean
    shared rounds : array [0..P-1][0..LogP] of round_t
        // row vpid of rounds is allocated in shared memory
	// locally accessible to processor vpid

    processor private sense : Boolean := true
    processor private vpid : integer // a unique virtual processor index

    //initially
    //    rounds[i][k].flag = false for all i,k
    //rounds[i][k].role = 
    //    winner if k > 0, i mod 2^k = 0, i + 2^(k-1) < P , and 2^k < P
    //    bye if k > 0, i mode 2^k = 0, and i + 2^(k-1) >= P
    //    loser if k > 0 and i mode 2^k = 2^(k-1)
    //    champion if k > 0, i = 0, and 2^k >= P
    //    dropout if k = 0
    //    unused otherwise; value immaterial
    //rounds[i][k].opponent points to 
    //    round[i-2^(k-1)][k].flag if rounds[i][k].role = loser
    //    round[i+2^(k-1)][k].flag if rounds[i][k].role = winner or champion
    //    unused otherwise; value immaterial
    procedure tournament_barrier
        round : integer := 1
	loop   //arrival
	    case rounds[vpid][round].role of
	        loser:
	            rounds[vpid][round].opponent^ :=  sense
		    repeat until rounds[vpid][round].flag = sense
		    exit loop
   	        winner:
	            repeat until rounds[vpid][round].flag = sense
		bye:  //do nothing
		champion:
	            repeat until rounds[vpid][round].flag = sense
		    rounds[vpid][round].opponent^ := sense
		    exit loop
		dropout: // impossible
	    round := round + 1
	loop  // wakeup
	    round := round - 1
	    case rounds[vpid][round].role of
	        loser: // impossible
		winner:
		    rounds[vpid[round].opponent^ := sense
		bye: // do nothing
		champion: // impossible
		dropout:
		    exit loop
	sense := not sense
*/

static int P;

const char* role(int i, int k)
{
  if (k > 0 && 
    i % (int)pow(2, k) == 0 &&
    i + (int) pow(2, k-1) < P &&
    (int) pow(2, k) < P
    )
  {
    return "winner";
  }
  else if(k > 0 &&
    i % (int) pow(2, k) == 0 &&
    i + (int) pow(2, k-1) >= P)
  {
    return "bye";
  }
  else if(k > 0 &&
    i % (int) pow(2, k) == pow(2, k-1))
  {
    return "loser";
  }
  else if(k > 0 &&
    i == 0 &&
    (int) pow(2, k) >= P)
  {
    return "champion";
  }
  else if(k == 0)
  {
    return "dropout";
  }
  return "undefined";
}

void gtmpi_init(int num_threads){
  P = num_threads;
}

void gtmpi_barrier(){
  int vpid, round;

  MPI_Comm_rank(MPI_COMM_WORLD, &vpid);
  round = 2;
  printf("round: %d, vpid: %d, %s\n", round, vpid, role(vpid, round));
  // while(1)
  // {

  // }
}

void gtmpi_finalize(){

}

