//
// Created by kiarash on 9/25/19.
//

#ifndef RMC_TREEWIDTH_CODE_SLE_TREEDEC_H
#define RMC_TREEWIDTH_CODE_SLE_TREEDEC_H

#define MAX_TREEWIDTH 10
#define MAXN 1000100

#include "../commons/vector.h"
#include <stdbool.h>

typedef struct edge
{
    int v, u;
    vector eqs;
} edge;

typedef struct bag
{
    int id;
    int verCnt,kidCnt;
    vector vertices;
    vector kids;

    edge *edges[MAX_TREEWIDTH][MAX_TREEWIDTH];

} bag;

bool removed[MAXN];
bool varRemoved[MAXN];

void bag_init(bag *, int); //initializes the bag and its vectors
void bag_add_kid(bag *, bag *); //adds kids to the bag
void bag_add_vertex(bag *, int *); //adds vertex within the bag
void bag_add_edge(bag *, edge *); //add edges within bag vertices
void bag_print(bag *); //prints bag vertices, edges (with probs), and kids
int get_vertex_index(bag *,int); //returns the index of vertex u in the vectors of the bag
int *get_vertex(bag *, int); //returns the vertex given its index in the bag
bag *get_kid(bag*,int); // returns kid given its index in the bag
bool has_vertex(bag*,int); // returns whether the bag has a specific vertex
int get_first_extra_vertex(bag *, bag *);  //returns the first vertex in first bag not in second bag or -1
int get_last_extra_vertex(bag *, bag *);  //returns the last vertex in first bag not in second bag or -1
void init_res(bag *,int); //initializes the final hitting probs for the root bag (used once after dfs_up)
void edge_print(edge *);

#endif //RMC_TREEWIDTH_CODE_SLE_TREEDEC_H
