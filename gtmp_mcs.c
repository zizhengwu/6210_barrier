#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "gtmp.h"
#include <stdbool.h>
#include <math.h>
#include <string.h>


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
  nodes = (node_t*) malloc(num_nodes * sizeof(num_nodes));

  int i;
  for (i = 0; i < num_nodes; ++i)
  {
    fprintf(stderr, "initializing node %d\n", i);
    int j;
    for (j = 0; j < 4; ++j)
    {
      _gtmp_get_node(i)->have_child[j] = 4 * i + j + 1 < num_threads ? true : false;
      fprintf(stderr, "have_child %d: %d\n", j, _gtmp_get_node(i)->have_child[j]);
    }
    _gtmp_get_node(i)->parent_pointer = i != 0 ? &_gtmp_get_node(floor((i-1)/4))->child_not_ready[(i-1) % 4] : NULL;
    _gtmp_get_node(i)->child_pointers[0] = 2 * i + 1 >= num_threads ? NULL : &_gtmp_get_node(2*i+1)->parent_sense;
    _gtmp_get_node(i)->child_pointers[1] = 2 * i + 2 >= num_threads ? NULL : &_gtmp_get_node(2*i+2)->parent_sense;
    fprintf(stderr, "parent_pointer: %d->%d, child_pointers[0]: %d, child_pointers[1]: %d\n", (int)floor((i-1)/4), (i-1) % 4, 2 * i + 1 >= num_threads ? -1 : 2*i+1, 2 * i + 2 >= num_threads ? -1 : 2*i+2);
    memcpy(_gtmp_get_node(i)->child_not_ready, _gtmp_get_node(i)->have_child, sizeof(_gtmp_get_node(i)->child_not_ready));
    _gtmp_get_node(i)->parent_sense = false;
  }
}

void gtmp_barrier(){

}

void gtmp_finalize(){

}
