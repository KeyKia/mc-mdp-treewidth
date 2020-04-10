//
// Created by kiarash on 10/19/19.
//

#ifndef RMC_TREEWIDTH_CODE_MC_HITPROB_SOLVE_H
#define RMC_TREEWIDTH_CODE_MC_HITPROB_SOLVE_H

// Input assumptions:
/* Root is bag 0 and contains ONLY the mcTarget
 * Not anymore: Edges of the tdc are from parent (v) to child (u)
 * All possible edges of each bag are given in edges (even with 0 delta)
 * (and even all the loops - which might have 0 delta)
 * All edges are given in both directions (even with 0 delta)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "../../commons/vector.h"
#include "../../commons/mc_treedec.h"

vector mcE; //contains all possible edges of each bag (even with 0 delta)
bag mcBags[MAXN]; //the bags
int mcTotV, mcTotE, mcTarget, mcBagsNum; // vertices, edges, and mcTarget in the Markov chain
double mcHitPr[MAXN];

void remove_vertices(int curIndex, int parIndex)
{
    /*
     * removes all vertices one by one, while keeping the Hitting
     * Probabilities of the remaining part unchanged. After all vertices are removed,
     * return_vertices() returns them one by one and computes the HitProbs.
    */

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
//            printf("\nremoving: %d\n", u);
            assert(!mcRemoved[u]);
            mcRemoved[u] = true;

            int uIndex = get_vertex_index(b, u);
            if (b->edges[uIndex][uIndex]->delta == 1.)
                continue;

            /* removes vertex u and updates the delta of edges among
             * its neighbors.
             */
            double f = 1. / (1. - b->edges[uIndex][uIndex]->delta);
            b->edges[uIndex][uIndex]->delta = 0.;

            for (i = 0; i < b->verCnt; i++)
                b->edges[uIndex][i]->delta *= f;

            for (i = 0; i < b->verCnt; i++)
                for (j = 0; j < b->verCnt; j++)
                {
                    if (mcRemoved[*get_vertex(b, i)] || mcRemoved[*get_vertex(b, j)])
                        continue;
                    edge *in = b->edges[i][uIndex];
                    edge *out = b->edges[uIndex][j];
                    edge *connection = b->edges[i][j];
                    connection->delta += in->delta * out->delta;
                }
        }
    }

}


void return_vertices(int curIndex, int parIndex)
{
    /*
     * Starting from root which has only two vertices (including
     * mcTarget), returns all vertices in each bag in the reversed
     * order they were mcRemoved in remove_vertices. While returning computes
     * the correct hitting probability of the returned vertex
     */
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
//            printf("returning vertex %d from bag %d\n", u, get_kid(mcBags + curIndex, i)->id);

            assert(!mcReturned[u]);
            mcReturned[u] = true;
            int uIndex = get_vertex_index(kid, u);
            if (kid->edges[uIndex][uIndex]->delta == 1.)
            {
                mcHitPr[u] = 0.;
                continue;
            }
            for(j=0;j<kid->verCnt;j++)
            {
                if(j==uIndex)
                    continue;
                else
                    mcHitPr[u] += kid->edges[uIndex][j]->delta * mcHitPr[*get_vertex(kid,j)];
            }
        }
        return_vertices(kid->id, curIndex);
    }

}


double solve_mc(bag bags[], int bagsNum, vector e, int totV, int totE, int target, double res[])
{
    mcBagsNum = bagsNum;
    int i;
    for (i=0; i<mcBagsNum; i++)
        mcBags[i] = bags[i];
    mcE = e;
    mcTotV = totV; mcTotE=totE; mcTarget = target;

    memset(mcHitPr, 0, sizeof(mcHitPr));
    memset(mcRemoved, 0, sizeof(mcRemoved));
    memset(mcReturned, 0, sizeof(mcReturned));

    clock_t begin = clock();

    remove_vertices(0, -1);

    mcHitPr[mcTarget] = 1.;
    return_vertices(0, -1);

    clock_t end = clock();

    for (i=0; i<totV; i++)
        res[i] = mcHitPr[i];

    return (double) (end - begin) / CLOCKS_PER_SEC;
}

#endif //RMC_TREEWIDTH_CODE_MC_HITPROB_SOLVE_H
