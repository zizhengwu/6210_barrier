#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "gtmpi.h"
// #include <math.h>
#include <string.h>

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

int power(int x, unsigned int y)
{
    if( y == 0)
        return 1;
    else if (y%2 == 0)
        return power(x, y/2)*power(x, y/2);
    else
        return x*power(x, y/2)*power(x, y/2);
}

const char* role(int i, int k)
{
  if (k > 0 && 
    i % (int)power(2, k) == 0 &&
    i + (int) power(2, k-1) < P &&
    (int) power(2, k) < P)
  {
    return "winner";
  }
  else if(k > 0 &&
    i % (int) power(2, k) == 0 &&
    i + (int) power(2, k-1) >= P)
  {
    return "bye";
  }
  else if(k > 0 &&
    i % (int) power(2, k) == power(2, k-1))
  {
    return "loser";
  }
  else if(k > 0 &&
    i == 0 &&
    (int) power(2, k) >= P)
  {
    return "champion";
  }
  else if(k == 0)
  {
    return "dropout";
  }
  return "undefined";
}

int opponent(int i, int k, const char* role)
{
  if (!strcmp(role, "loser"))
  {
    return i - (int) power(2, k-1);
  }
  if (!strcmp(role, "winner") || !strcmp(role, "champion"))
  {
    return i + (int) power(2, k-1);
  }
  return -1;
}

void gtmpi_init(int num_threads){
  P = num_threads;
}

void gtmpi_barrier(){
  int opponent(int i, int k, const char* role);
  const char* role(int i, int k);
  int vpid, round;

  MPI_Comm_rank(MPI_COMM_WORLD, &vpid);
  round = 1;
  while(1)
  {
    if (!strcmp(role(vpid, round), "loser"))
    {
      // fprintf(stderr, "loser round %d thread %d send to thread %d\n", round, vpid, opponent(vpid, round, role(vpid, round)));
      MPI_Send(NULL, 0, MPI_INT, opponent(vpid, round, role(vpid, round)), 1, MPI_COMM_WORLD);
      // fprintf(stderr, "loser round %d thread %d waiting for thread %d\n", round, vpid, opponent(vpid, round, role(vpid, round)));
      MPI_Recv(NULL, 0, MPI_INT, opponent(vpid, round, role(vpid, round)), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // fprintf(stderr, "loser round %d thread %d received from thread %d\n", round, vpid, opponent(vpid, round, role(vpid, round)));
      break;
    }
    else if (!strcmp(role(vpid, round), "winner"))
    {
      // fprintf(stderr, "winner round %d thread %d waiting for thread %d\n", round, vpid, opponent(vpid, round, role(vpid, round)));
      MPI_Recv(NULL, 0, MPI_INT, opponent(vpid, round, role(vpid, round)), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // fprintf(stderr, "winner round %d thread %d received from thread %d\n", round, vpid, opponent(vpid, round, role(vpid, round)));
    }
    else if(!strcmp(role(vpid, round), "bye"))
    {
      // nothing
    }
    else if(!strcmp(role(vpid, round), "champion"))
    {
      // fprintf(stderr, "champion round %d thread %d waiting for thread %d\n", round, vpid, opponent(vpid, round, role(vpid, round)));
      MPI_Recv(NULL, 0, MPI_INT, opponent(vpid, round, role(vpid, round)), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // fprintf(stderr, "champion round %d thread %d send to thread %d\n", round, vpid, opponent(vpid, round, role(vpid, round)));
      MPI_Send(NULL, 0, MPI_INT, opponent(vpid, round, role(vpid, round)), 1, MPI_COMM_WORLD);
      break;
    }
    else
    {
      // exit(1);
    }
    round++;
  }
  // fprintf(stderr, "thread %d wakeup\n", vpid);
  while(1)
  {
    round--;
    if (!strcmp(role(vpid, round), "loser"))
    {
      break;
    }
    if (!strcmp(role(vpid, round), "winner"))
    {
      // fprintf(stderr, "wakeup round %d thread %d send to thread %d\n", round, vpid, opponent(vpid, round, role(vpid, round)));
      MPI_Send(NULL, 0, MPI_INT, opponent(vpid, round, role(vpid, round)), 1, MPI_COMM_WORLD);
    }
    if (!strcmp(role(vpid, round), "bye"))
    {
      // nothing
    }
    if (!strcmp(role(vpid, round), "champion"))
    {
      break;
    }
    if (!strcmp(role(vpid, round), "dropout"))
    {
      break;
    }
  }
}

void gtmpi_finalize(){

}

