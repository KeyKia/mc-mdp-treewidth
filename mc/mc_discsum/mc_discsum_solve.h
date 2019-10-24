//
// Created by kiarash on 10/22/19.
//

#ifndef RMC_TREEWIDTH_CODE_MC_DISCSUM_SOLVE_H
#define RMC_TREEWIDTH_CODE_MC_DISCSUM_SOLVE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../../utils/vector.h"
#include "../mc_treedec.h"

// TODO: There should be no parallel edges and self-loops in the input

vector mcE; //contains all possible edges of each bag (even with 0 delta)
bag mcBags[MAXN]; //the bags
int mcTotV, mcTotE, mcOneHat, mcBagsNum; // vertices, edges, and mcTarget in the Markov chain
double mcDiscSum[MAXN], mcLambda;

/*
 * Adds an edge with delta=d and reward=r between vertices
 * at index i1 and i2 of bag b. And takes care of potential
 * parallel edges and loops.
 */
void add_edge(bag *b, int i1, int i2, double d, double r)
{
    // TODO: possible infinite loop?
    if (fabs(d) <= EPSILON)
        return;

//    printf("Adding edge from %d to %d with delta %f and reward %f\n", *get_vertex(b, i1), *get_vertex(b, i2), d, r);

    edge *ej = b->edges[i1][i2];

    // no parallel edge && no self-loop
    if (fabs(ej->delta) <= EPSILON && i1 != i2) // TODO: ej->delta < 0
    {
        ej->delta = d;
        ej->reward = r;
    }
    // parallel edge
    else if (i1 != i2 )
    {
        ej->reward = (ej->delta * ej->reward + d * r) / (ej->delta + d);
        ej->delta += d;
    }
    // self-loop
    else
    {
        assert(1 - d * mcLambda != 0);
        int i;
        for (i=0; i<b->verCnt; i++)
        {
            if (i==i1 || mcRemoved[*get_vertex(b, i)])
                continue;
            b->edges[i1][i]->delta /= 1 - d * mcLambda;
        }
        add_edge(b, i1, get_vertex_index(b, mcOneHat), d * r / ((1 - d * mcLambda)*mcLambda), 0.);
//        printf("-----------------%d %d\n", *get_vertex(b, i1), *get_vertex(b, i2));
    }

//    printf("The edge from %d to %d now has delta %f and reward %f\n\n", *get_vertex(b, i1), *get_vertex(b, i2),
//               ej->delta, ej->reward);
}

/*
 * remove_vertices() removes all vertices one by one, while keeping the Expected
 * Discounted Sums of the remaining part unchanged. After all vertices are removed,
 * return_vertices() returns them one by one and computes the Expected Discounted Sums.
*/
void remove_vertices(int curIndex, int parIndex)
{
//    bag_print(mcBags+curIndex);
//    fflush(stdout);
    int i, j;
    bag *b = mcBags + curIndex;

    for (i = 0; i < b->kidCnt; i++)
        if (get_kid(b, i)->id != parIndex)
            remove_vertices(get_kid(b, i)->id, curIndex);

    if (parIndex != -1)
    {
        /* We are currently in a leaf bag. The below while loop checks
         * whether this bag has any extra vertex comparing to its parent bag.
         * We remove the extra vertices as long as this leaf bag is not a subset
         * of its parent.
         */
        while (get_first_extra_vertex(b, mcBags + parIndex) != -1)
        {
            int u = get_first_extra_vertex(b, mcBags + parIndex);
//            printf("removing: %d\n", u);
//            if (u==18)
//                bag_print(b);
            assert(!mcRemoved[u]);
            mcRemoved[u] = true;

            int uIndex = get_vertex_index(b, u);

            /* removes vertex u and updates the delta and reward of edges among
             * its neighbors.
             */

            int oneHatIdx = get_vertex_index(b, mcOneHat);
            for (i = 0; i < b->verCnt; i++)
            {
                if (mcRemoved[*get_vertex(b, i)] || i == oneHatIdx) // TODO mcOneHat
                    continue;

                edge *in = b->edges[i][uIndex];
//                if (u==18)
//                {
//                    printf("Adding edge from %d to %d with delta %f * %f / %f = %f\n", *get_vertex(b, i),
//                           *get_vertex(b, oneHatIdx),
//                           in->delta, in->reward, mcLambda, in->delta * in->reward / mcLambda);
//                }
                add_edge(b, i, oneHatIdx, in->delta * in->reward / mcLambda, 0.);

                for (j = 0; j < b->verCnt; j++)
                {
                    if (mcRemoved[*get_vertex(b, j)]) // TODO mcOneHat
                        continue;
                    edge *out = b->edges[uIndex][j];
//                    if (u==18)
//                        printf("Adding edge from %d to %d with delta %f and reward %f\n", *get_vertex(b, i), *get_vertex(b, j),
//                               in->delta * out->delta * mcLambda, out->reward);
                    add_edge(b, i, j, in->delta * out->delta * mcLambda, out->reward);
                }
            }
//            if (u==18)
//                bag_print(b);
        }
    }

}

/*
 * Starting from root which has only one vertex (1^),
 * returns all vertices in each bag in the reversed order they
 * were removed in remove_vertices(). While returning computes
 * the correct Expected Discounted Sums of the returned vertex
 */
void return_vertices(int curIndex, int parIndex)
{
    int i, j;
    bag *b = mcBags + curIndex;
    for (i = 0; i < b->kidCnt; i++)
    {
        bag *kid  = get_kid(mcBags + curIndex, i);
        if (kid->id == parIndex)
            continue;

        while (get_last_extra_vertex(kid, b) != -1)
        {
            int u = get_last_extra_vertex(kid, b);
//            printf("\nreturning vertex %d from bag %d\n", u, get_kid(mcBags + curIndex, i)->id);

            assert(!mcReturned[u]);
            mcReturned[u] = true;
            int uIndex = get_vertex_index(kid, u);
            for (j=0; j<kid->verCnt; j++)
            {
                if(j==uIndex || !mcReturned[*get_vertex(kid, j)]) //TODO: double check
                    continue;
                else
                {
                    mcDiscSum[u] += kid->edges[uIndex][j]->delta * (kid->edges[uIndex][j]->reward
                                                                    + mcLambda * mcDiscSum[*get_vertex(kid, j)]);

//                    if (u==18)
//                        printf("updating y_%d += %f * ( %f + %f * Discsum[%d]=%f)\n", u, kid->edges[uIndex][j]->delta,
//                           kid->edges[uIndex][j]->reward, mcLambda, *get_vertex(kid, j), mcDiscSum[*get_vertex(kid, j)]);
                }
            }
        }
        return_vertices(kid->id, curIndex);
    }

}


double solve_mc(bag bags[], int bagsNum, vector e, int totV, int totE, double lambda, double res[])
{
    mcOneHat = totV; // The vertex 1^ that we add to every bag (here we make it the n-th vertex)
    int i, j;

    // Adding the new edges connecting 1^ to others and itself (with zero delta and reward to others)
    int st = totE;
    for (i=0; i<totV; i++)
    {
        edge *ej1 = malloc(sizeof(edge));
        ej1->v = mcOneHat; ej1->u = i;
        ej1->delta = 0.; ej1->reward = 0.;
        vector_add(&e, ej1);

        edge *ej2 = malloc(sizeof(edge));
        ej2->v = i; ej2->u = mcOneHat;
        ej2->delta = 0.; ej2->reward = 0.;
        vector_add(&e, ej2);
    }
    edge *ej = malloc(sizeof(edge));
    ej->v = mcOneHat; ej->u = mcOneHat;
    ej->delta = 1.; ej->reward = 1 - lambda;
    vector_add(&e, ej);

    // add a new bag that contains only 1^ and will be the root bag
    bag_init(bags + bagsNum, bagsNum);
    int *tmp = malloc(sizeof(int));
    *tmp = mcOneHat;
    bag_add_vertex(bags + bagsNum, tmp);
    bag_add_edge(bags + bagsNum, ej);

    // Adding the vertex 1^ to every bag
    for (i=0; i<bagsNum; i++)
    {
        bag_add_vertex(bags + i, tmp); // TODO: again new tmp?
        bag_add_edge(bags + i, ej);
        for (j=0; j<(bags+i)->verCnt-1; j++)
        {
            int u = *get_vertex(bags+i, j);
            bag_add_edge(bags + i, vector_get(&e, st + 2 * u));
            bag_add_edge(bags + i, vector_get(&e, st + 2 * u + 1));
        }
        mcBags[i] = bags[i];
    }
    mcBags[bagsNum] = bags[bagsNum];

    // Add the bag containing only 1^ to the tree decomposition
    // (by connecting it to bag 0)
    bag_add_kid(mcBags, mcBags + bagsNum);
    bag_add_kid(mcBags + bagsNum, mcBags);

    mcBagsNum = bagsNum + 1;
    mcE = e;
    mcTotV = totV + 1;
    mcTotE = vector_total(&e);
    mcLambda = lambda;

    memset(mcDiscSum, 0, sizeof(mcDiscSum));
    memset(mcRemoved, 0, sizeof(mcRemoved));
    memset(mcReturned, 0, sizeof(mcReturned));

    clock_t begin = clock();

    remove_vertices(mcBagsNum - 1, -1);
//    printf("\n-------------------------------\n\n");

    mcDiscSum[mcOneHat] = 1.;
    mcReturned[mcOneHat] = 1;
    return_vertices(mcBagsNum - 1, -1);
//    printf("\n-------------------------------\n\n");

    clock_t end = clock();

    for (i=0; i<totV; i++)
        res[i] = mcDiscSum[i];

    return (double) (end - begin) / CLOCKS_PER_SEC;
}

#endif //RMC_TREEWIDTH_CODE_MC_DISCSUM_SOLVE_H
