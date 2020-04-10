//
// Created by kiarash on 9/25/19.
//

#ifndef RMC_TREEWIDTH_CODE_MC_TREEDEC_H
#define RMC_TREEWIDTH_CODE_MC_TREEDEC_H

#define MAX_TREEWIDTH 50
#define MAXN 1510
#define EPSILON 1e-8

#include "vector.h"
#include <stdbool.h>

typedef struct edge
{
    int v, u;
    double delta, reward;
} edge;

typedef struct bag
{
    int id;
    int verCnt,kidCnt;
    vector vertices;
    vector kids;

    edge *edges[MAX_TREEWIDTH][MAX_TREEWIDTH];

} bag;

bool mcRemoved[MAXN];
bool mcReturned[MAXN];

void bag_init(bag *, int); //initializes the bag and its vectors
void bag_free(bag *); //free the vectors of the given bag
void bag_add_kid(bag *, bag *); //adds kids to the bag
void bag_add_vertex(bag *, int *); //adds vertex within the bag
void bag_add_edge(bag *, edge *); //add edges within bag vertices
void bag_print(bag *); //prints bag vertices, edges (with probs), and kids
int get_vertex_index(bag *,int); //returns the index of vertex u in the vectors of the bag
int *get_vertex(bag *, int); //returns the vertex given its index in the bag
double get_edge_prob(bag *, int, int);
bag *get_kid(bag*,int); // returns kid given its index in the bag
bool has_vertex(bag*,int); // returns whether the bag has a specific vertex
int get_first_extra_vertex(bag *, bag *);  //returns the first vertex in first bag not in second bag or -1
int get_last_extra_vertex(bag *, bag *);  //returns the last vertex in first bag not in second bag or -1
void print_edge(edge *); //prints an edge with endpoints and delta

#endif //RMC_TREEWIDTH_CODE_MC_TREEDEC_H
