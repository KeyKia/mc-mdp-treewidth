//
// Created by kiarash on 9/25/19.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tree_decomposition.h"


void bag_init(bag *b, int id)
{
    b->verCnt=0;
    b->kidCnt=0;
    b->id = id;
    vector_init(&(b->kids));
    vector_init(&(b->vertices));
    int i, j;
    for (i=0;i<MAX_TREEWIDTH;i++)
        for (j=0;j<MAX_TREEWIDTH;j++)
            b->edges[i][j]=NULL;
}

void bag_add_kid(bag *b, bag *kid)
{
    vector_add(&(b->kids), kid);
    b->kidCnt++;
}

void bag_add_vertex(bag *b, int *v)
{
    vector_add(&(b->vertices), v);
    b->verCnt++;
}

void bag_add_edge(bag *b, edge *e)
{
    int v=get_vertex_index(b,e->v);
    int u=get_vertex_index(b,e->u);
    b->edges[v][u]=e;
}

int get_vertex_index(bag *b,int v)
{
    int i;
    int vIndex=-1;
    for(i=0;i<b->verCnt;i++)
        if(*get_vertex(b,i)==v)
            vIndex=i;
    return vIndex;
}

int *get_vertex(bag *b,int ind)
{
    return vector_get(&(b->vertices),ind);
}

float get_edge_prob(bag *b,int v,int u)
{
    v=get_vertex_index(b,v);
    u=get_vertex_index(b,u);
    assert(v==-1 || u==-1);
    return b->edges[v][u]->prob;
}

bag *get_kid(bag *b,int ind)
{
    return vector_get(&(b->kids),ind);
}

void bag_print(bag *b)
{
    printf("-------------------------------\n");
    printf("id:%d\n",b->id);
    int i,j;
    printf("vertices: ");
    for(i=0;i<b->verCnt;i++)
        printf("%d ",*get_vertex(b,i));
    printf("\nedges:\n");
    for(i=0;i<b->verCnt;i++)
    {
        for (j = 0; j < b->verCnt; j++)
            if (b->edges[i][j]==NULL)
                printf("0.0 ");
            else
                printf("%f ", b->edges[i][j]->prob);
        printf("\n");
    }
    printf("kids:");
    for(i=0;i<b->kidCnt;i++)
        printf("%d ",get_kid(b,i)->id);
    printf("\n-------------------------------\n\n");
}

bool has_vertex(bag* b,int v)
{
    int i;
    for(i=0;i<b->verCnt;i++)
        if(*get_vertex(b,i)==v)
            return true;
    return false;
}

int get_first_extra_vertex(bag *b1, bag *b2)
{
    int i;
    for(i=0;i<b1->verCnt;i++)
        if(!has_vertex(b2,*get_vertex(b1,i)) && !removed[*get_vertex(b1,i)])
            return *get_vertex(b1,i);
    return -1;
}

int get_last_extra_vertex(bag *b1, bag *b2)
{
    int i;
    for(i=b1->verCnt-1;i>=0;i--)
        if(!has_vertex(b2,*get_vertex(b1,i)) && !removed[*get_vertex(b1,i)])
            return *get_vertex(b1,i);
    return -1;
}

void remove_vertex(bag *b,int v)
{
    assert(!removed[v]);
    removed[v]=true;
    int vIndex=get_vertex_index(b,v);
    printf("\nremoving: %d\n",v);
    int i, j;
    /*
     * removes vertex v and updates the prob of edges between
     * its neighbors (including the self-loops).
     */
    for (i=0;i<b->verCnt;i++)
        for (j=0;j<b->verCnt;j++)
        {
            if(i==vIndex || j==vIndex)
                continue;
            edge *in=b->edges[i][vIndex];
            edge *out=b->edges[vIndex][j];
            edge *connection=b->edges[i][j];

            connection->prob+=in->prob*out->prob;
            print_edge(connection);
        }
}

void print_edge(edge *e)
{
    printf("connecting edge: from %d to %d with %f\n",e->v,e->u,e->prob);
}

void init_res(bag *b,int target)
{
    int ind=get_vertex_index(b,target),v;
    res[target]=1;
    if(ind==0)
    {
        v = *get_vertex(b, 1);
        /*
         * finding the sum limit of the geometric sequence caused
         * by the self-loop of the non-target vertex
        */
        res[v]=b->edges[1][0]->prob/(1-b->edges[1][1]->prob);
    }
    else
    {
        v = *get_vertex(b, 0);

        /*
         * finding the sum limit of the geometric sequence caused
         * by the self-loop of the non-target vertex
        */
        res[v]=b->edges[0][1]->prob/(1-b->edges[0][0]->prob);;
    }
}

void return_vertex(bag *b, int v)
{
    assert(!removed[v]);
    removed[v]=true;
    int vIndex=get_vertex_index(b,v);
    int i;
    for(i=0;i<b->verCnt;i++)
    {
        if(i==vIndex)
            continue;
        else
            res[v]+=res[*get_vertex(b,i)]*b->edges[vIndex][i]->prob;
    }
}