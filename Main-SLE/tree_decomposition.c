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
    b->edges[u][v]=e;
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
                printf("0");
            else
                printf("1");
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

void edge_print(edge *e)
{
    printf("edge: from %d to %d in: ",e->v,e->u);
    int i;
    for(i=0;i<vector_total(&(e->eqs));i++)
        printf("%d ",*(int *)vector_get(&(e->eqs),i));
    printf("\n");
}





