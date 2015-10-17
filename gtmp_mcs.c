#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "gtmp.h"
#include <stdbool.h>
#include <math.h>
#include <string.h>

/*
    From the MCS Paper: A scalable, distributed tree-based barrier with only local spinning.

    type treenode = record
        parentsense : Boolean
    parentpointer : ^Boolean
    childpointers : array [0..1] of ^Boolean
    havechild : array [0..3] of Boolean
    childnotready : array [0..3] of Boolean
    dummy : Boolean //pseudo-data

    shared nodes : array [0..P-1] of treenode
        // nodes[vpid] is allocated in shared memory
        // locally accessible to processor vpid
    processor private vpid : integer // a unique virtual processor index
    processor private sense : Boolean

    // on processor i, sense is initially true
    in nodes[i]:
       havechild[j] = true if 4 * i + j + 1 < P; otherwise false
       parentpointer = &nodes[floor((i-1)/4].childnotready[(i-1) mod 4],
           or dummy if i = 0
       childpointers[0] = &nodes[2*i+1].parentsense, or &dummy if 2*i+1 >= P
       childpointers[1] = &nodes[2*i+2].parentsense, or &dummy if 2*i+2 >= P
       initially childnotready = havechild and parentsense = false
    
    procedure tree_barrier
        with nodes[vpid] do
        repeat until childnotready = {false, false, false, false}
        childnotready := havechild //prepare for next barrier
        parentpointer^ := false //let parent know I'm ready
        // if not root, wait until my parent signals wakeup
        if vpid != 0
            repeat until parentsense = sense
        // signal children in wakeup tree
        childpointers[0]^ := sense
        childpointers[1]^ := sense
        sense := not sense
*/

typedef struct _node_t{
  bool parent_sense;
  bool* parent_pointer;
  bool* child_pointers[2];
  bool have_child[4];
  bool child_not_ready[4];
} node_t;

static node_t* nodes;

node_t* _gtmp_get_node(int i){
  return &nodes[i];
}

void gtmp_init(int num_threads){
  int num_nodes = num_threads;
  nodes = (node_t*) malloc(num_nodes * sizeof(node_t));

  int i;
  for (i = 0; i < num_nodes; ++i)
  {
    // fprintf(stderr, "initializing node %d\n", i);
    int j;
    for (j = 0; j < 4; ++j)
    {
      _gtmp_get_node(i)->have_child[j] = 4 * i + j + 1 < num_threads ? true : false;
      // fprintf(stderr, "have_child %d: %d\n", j, _gtmp_get_node(i)->have_child[j]);
    }
    _gtmp_get_node(i)->parent_pointer = i != 0 ? &_gtmp_get_node((int)floor((i-1)/4))->child_not_ready[(i-1) % 4] : NULL;
    _gtmp_get_node(i)->child_pointers[0] = 2 * i + 1 >= num_threads ? NULL : &_gtmp_get_node(2*i+1)->parent_sense;
    _gtmp_get_node(i)->child_pointers[1] = 2 * i + 2 >= num_threads ? NULL : &_gtmp_get_node(2*i+2)->parent_sense;
    // fprintf(stderr, "parent_pointer: %d->%d: %p, child_pointers[0]: %d, child_pointers[1]: %d\n", (int)floor((i-1)/4), (i-1) % 4, &_gtmp_get_node((int)floor((i-1)/4))->child_not_ready[(i-1) % 4], 2 * i + 1 >= num_threads ? -1 : 2*i+1, 2 * i + 2 >= num_threads ? -1 : 2*i+2);
    memcpy(_gtmp_get_node(i)->child_not_ready, _gtmp_get_node(i)->have_child, sizeof(_gtmp_get_node(i)->child_not_ready));
    _gtmp_get_node(i)->parent_sense = false;
  }
}

void gtmp_barrier(){
  bool sense = true;
  int vpid = omp_get_thread_num();
  while(!(_gtmp_get_node(vpid)->child_not_ready[0] == false && _gtmp_get_node(vpid)->child_not_ready[1] == false && _gtmp_get_node(vpid)->child_not_ready[2] == false && _gtmp_get_node(vpid)->child_not_ready[3] == false));
  memcpy(_gtmp_get_node(vpid)->child_not_ready, _gtmp_get_node(vpid)->have_child, sizeof(_gtmp_get_node(vpid)->child_not_ready));
  if (_gtmp_get_node(vpid)->parent_pointer != NULL)
  {
    *_gtmp_get_node(vpid)->parent_pointer = false;
  }
  if (vpid != 0)
  {
   while(_gtmp_get_node(vpid)->parent_sense != sense);
  }
  if (_gtmp_get_node(vpid)->child_pointers[0] != NULL)
  {
    *_gtmp_get_node(vpid)->child_pointers[0] = sense;
  }
  if (_gtmp_get_node(vpid)->child_pointers[1] != NULL)
  {
    *_gtmp_get_node(vpid)->child_pointers[1] = sense;
  }
  sense = !sense;
}

void gtmp_finalize(){
  free(nodes);
}
