//
// Created by kiarash on 10/23/19.
//

/* Input:
 * The only difference with MC is that we fist read choose and prob vertices,
 * and there is a flag for each edge determining whether it is a choose edge or not ("or not" != prob)
 * Choose edges have 0 prob in input (just to be consistent with prob edges)
 */

#include <dirent.h>
#include <string.h>
#include "../../utils/vector.h"
#include "../../mc/mc_discsum/mc_discsum_solve.h"
#include "../../gaussian_elimination/gaussian_elim.h"

//bag bags[MAXN];
//vector e;
//vector choices[MAXN]; //choices[u] contains the edges towards successors of choose-vertex u
//vector chooseVertices;
int totChooseV, totProbV, totV, totE, bagsNum;
int isChoose[MAXN], policy[MAXN]; // policy[u] indicates the "index" of the edge to choose among choices[u]
double discSum[MAXN], lambda;

void input(char file_path[], bag* bags, vector *e, vector *choices, vector *choices_copy, vector *chooseVertices)
{
    FILE *fp = fopen(file_path, "r");

    int i, j;

    //read mdp vertices num
    fscanf(fp, "%d%d%d%lf", &totChooseV, &totProbV, &totE, &lambda);

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
        choices_copy[*v] = *(vector *) malloc(sizeof(vector));
        vector_init(choices_copy + *v);
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
        fscanf(fp, "%d%d%lf%lf%d", &(ej->v), &(ej->u), &(ej->delta), &(ej->reward), &isChooseEdge);
        if (isChooseEdge)
        {
            vector_add(choices + ej->v, ej); // add ej to choices of v if it is a choose-vertex

            edge *ej_copy = malloc(sizeof(edge));
            ej_copy->v = ej->v; ej_copy->u = ej->u;
            ej_copy->delta = ej->delta; ej_copy->reward = ej->reward;
            vector_add(choices_copy + ej_copy->v, ej_copy);
        }

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


int main()
{

    char dir_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/benchmarks/dacapobenchmark/outputs/mdp_discsum/eclipse/";
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dir_path);
    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
    }

    double time_spent = 0., tot_iters=0., num_mdps=0;
    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_name[0] == '.')
            continue;
//        printf("%s\n",de->d_name);
        num_mdps++;
//        char file_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/mdp/mdp_discsum/example.txt";
//        char file_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/benchmarks/dacapobenchmark/outputs/mdp_discsum/eclipse/parseActions.txt";
        char file_path[256];
        strcpy(file_path, dir_path);
        strcat(file_path, de->d_name);

        int i, j, iter = 0, done = 1;

        memset(discSum, 0, sizeof(discSum));
        memset(isChoose, 0, sizeof(isChoose));
        memset(policy, 0, sizeof(policy));

        do
        {
            done = 1;

            bag *bags = malloc(MAXN * sizeof(bag));
            vector *e = malloc(sizeof(vector));
            vector *choices = malloc(MAXN * sizeof(vector));
            vector *choices_copy = malloc(MAXN * sizeof(vector));
            vector *chooseVertices = malloc(sizeof(vector));

            input(file_path, bags, e, choices, choices_copy, chooseVertices); // read input here each time to re-new everything
            if (lambda>0.95)
                lambda = 0.9;

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

            memset(discSum, 0, sizeof(discSum));

//            double tmp = solve_mc(bags, bagsNum, *e, totV, totE, lambda, discSum);
            gaussian_solve_mc_discsum(*e, totV, totE, lambda, discSum);

            for (i = 0; i < vector_total(chooseVertices); i++)
            {
                int *v = vector_get(chooseVertices, i);
                if (vector_total(choices_copy + *v) == 0)
                    continue;

                // update the policy according to computed Hit discSums
                // we use choices_copy cause the rewards in choices has changed within solve_mc
                edge *choice = vector_get(choices_copy + *v, policy[*v]);
                int newChoiceID = policy[*v];
                double maxDiscSum = choice->reward + lambda * discSum[choice->u];

//                if (choice->reward + lambda * discSum[choice->u] != discSum[*v])
//                {
//                    printf("ridiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
//                    printf("%f != %f\n", choice->reward + lambda * discSum[choice->u], discSum[*v]);
//                    printf("discSum[%d] = %f\n",choice->u, discSum[choice->u]);
//                    print_edge(choice);
//                }

                for (j = 0; j < vector_total(choices_copy + *v); j++)
                {
                    edge *cur = vector_get(choices_copy + *v, j);
                    if (cur->reward + lambda * discSum[cur->u] - maxDiscSum > EPSILON)
                    {
                        maxDiscSum = cur->reward + lambda * discSum[cur->u];
                        newChoiceID = j;
                    }
                }
                assert(newChoiceID != -1);
                if (newChoiceID != policy[*v])
                {
                    done = 0;
//                    edge *last = vector_get(choices_copy + *v, policy[*v]);
//                    edge *current = vector_get(choices_copy + *v, newChoiceID);
//                    printf("Policy changed for vertex %d from %d to %d\n", *v, last->u, current->u);
//                    printf("last: %f, current: %f\n", last->reward + lambda * discSum[last->u],
//                            current->reward + lambda * discSum[current->u]);
//                    print_edge(last);
//                    print_edge(current);
                }

                policy[*v] = newChoiceID;
            }
            iter++;

            clock_t end = clock();
            time_spent += (double) (end - begin) / CLOCKS_PER_SEC;

//            for (i = 0; i < totV; i++)
//                printf("Expected Discounted Sum of vertex %d: %f\n", i, discSum[i]);
//
//            if (iter>10){
//                printf("-------------------------------%s\n", de->d_name);
//                done = 1;
//            }

        } while (!done);

        tot_iters += iter;
//        for (i = 0; i < totV; i++)
//            printf("Expected Discounted Sum of vertex %d: %f\n", i, discSum[i]);
    }

    printf("\nMean number of iterations: %f\n", tot_iters/num_mdps);
    printf("\nTime spent: %f seconds\n", time_spent);

    return 0;
}