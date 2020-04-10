//
// Created by kiarash on 10/20/19.
//

/* Input:
 * The only difference with MC is that we fist read choose and prob vertices,
 * and there is a flag for each edge determining whether it is a choose edge or not ("or not" != prob)
 * Choose edges have 0 prob in input (just to be consistent with prob edges)
 */

#include <dirent.h>
#include <string.h>
#include "../../commons/vector.h"
#include "../../mc_hitprob/ours/mc_hitprob_solve.h"
#include "../../commons/gaussian_elim.h"


int totChooseV, totProbV, totV, totE, target, bagsNum;
int isChoose[MAXN], policy[MAXN]; // policy[u] indicates the "index" of the edge to choose among choices[u]
double hitPr[MAXN];


void input(char file_path[], bag* bags, vector *e, vector *choices, vector *chooseVertices)
{
    FILE *fp = fopen(file_path, "r");

    int i, j;

    //read mdp vertices num
    fscanf(fp, "%d%d%d%d", &totChooseV, &totProbV, &totE, &target);

    totV = totChooseV + totProbV;
    vector_init(chooseVertices);

    // read choose vertices
    for (i=0; i<totChooseV; i++)
    {
        int *v = malloc(sizeof(int));
        fscanf(fp, "%d", v);
        isChoose[*v] = true;
        vector_add(chooseVertices, v);
        choices[*v] = *(vector *) malloc(sizeof(vector));
        vector_init(choices + *v);
    }
    // read prob vertices (unneccessary here)
    for (i=0; i<totProbV; i++)
    {
        int v; fscanf(fp, "%d", &v);
    }


    vector_init(e);
    //get edges
    for (i = 0; i < totE; i++)
    {
        edge *ej = malloc(sizeof(edge)); int isChooseEdge;
        fscanf(fp, "%d%d%lf%d", &(ej->v), &(ej->u), &(ej->delta), &isChooseEdge);
        if (ej->v == target)
            ej->delta = 0.0; //remove edges leaving target -- causes no change in hitting probs
        if (isChooseEdge && ej->v != target)
            vector_add(choices + ej->v, ej); // add ej to choices of v if it is a choose-vertex

        vector_add(e, ej);
    }

    // read vertices and edges within bags
    fscanf(fp, "%d", &bagsNum);
    for (i = 0; i < bagsNum; i++)
    {
        bags[i] = *(bag *) malloc(sizeof(bag));
        bag_init(bags + i, i);
        int V, E;
        fscanf(fp, "%d%d", &V, &E);

        // read vertices within bag i
        for (j = 0; j < V; j++)
        {
            int *x = malloc(sizeof(int));
            fscanf(fp, "%d", x);
            bag_add_vertex(bags + i, x);
        }

        // read edges within bag i
        for (j = 0; j < E; j++)
        {
            int ind;
            fscanf(fp, "%d", &ind);
            edge *ej = vector_get(e, ind);
            bag_add_edge(bags + i, ej);
        }
    }

    //read the tree edges between mcBags
    // assumptions: root is bag 0 and contains the mcTarget
    // Not anymore: and edges are from parent(v) to child(u)
    for (i = 0; i + 1 < bagsNum; i++)
    {
        int *v = malloc(sizeof(int)), *u = malloc(sizeof(int));
        fscanf(fp, "%d%d", v, u);
        bag_add_kid(bags + *v, bags + *u);
        bag_add_kid(bags + *u, bags + *v);
    }

    fclose(fp);
}


int main(int argc, char* argv[])
{

    double time_spent = 0., tot_iters=0., num_mdps=0;
    int done, i, j;

    do
    {
        done = 1;

        bag *bags = malloc(MAXN * sizeof(bag));
        vector *e = malloc(sizeof(vector));
        vector *choices = malloc(MAXN * sizeof(vector));
        vector *chooseVertices = malloc(sizeof(vector));

        input(argv[1], bags, e, choices, chooseVertices); // re-new everything

        clock_t begin = clock();
        // affect the current policy and prepare the Markov chain
        for (i = 0; i < vector_total(chooseVertices); i++)
        {
            int *v = vector_get(chooseVertices, i);
            if (vector_total(choices + *v) == 0)
                continue;
            edge *choice = vector_get(choices + *v, policy[*v]);
            choice->delta = 1.;
        }

        memset(hitPr, 0, sizeof(hitPr));
        double tmp = solve_mc(bags, bagsNum, *e, totV, totE, target, hitPr);

        for (i = 0; i < vector_total(chooseVertices); i++)
        {
            // de-affect the old policy
            int *v = vector_get(chooseVertices, i);
            if (vector_total(choices + *v) == 0)
                continue;
            edge *choice = vector_get(choices + *v, policy[*v]);
            choice->delta = 0.;

            // update the policy according to computed Hit Probs
            int newChoiceID = policy[*v];
            double maxProb = hitPr[choice->u];
            for (j = 0; j < vector_total(choices + *v); j++)
            {
                edge *cur = vector_get(choices + *v, j);
                if (hitPr[cur->u] - maxProb > EPSILON)
                {
                    maxProb = hitPr[cur->u];
                    newChoiceID = j;
                }
            }
            assert(newChoiceID != -1);
            if (newChoiceID != policy[*v])
                done = 0;
            policy[*v] = newChoiceID;
        }

        clock_t end = clock();
        time_spent += (double) (end - begin) / CLOCKS_PER_SEC;

    } while (!done);


    printf("%f\n", time_spent);

    return 0;
}
