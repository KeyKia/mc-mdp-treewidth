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

    char dir_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/benchmarks/dacapobenchmark/outputs/mdp_discsum/";
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dir_path);
    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
    }
    double all_means[50]; char all_names[50][256]; int file_n=0, flag=0, end_this_shit=0;
    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_name[0] == 'j' && de->d_name[1]=='a' && de->d_name[2]=='f' && de->d_name[3]=='f' && de->d_name[4]=='l')
            break;
//        if (!flag)
//            continue;
        if (de->d_name[0] == '.')
            continue;
        char subdir_path[256];
        strcpy(subdir_path, dir_path);
        strcat(subdir_path, de->d_name);
        strcat(subdir_path, "/");

        struct dirent *sde;  // Pointer for directory entry
        DIR *sdr = opendir(subdir_path);
        if (sdr == NULL)
        {
            printf("Could not open benchmark within directory" );
            return 0;
        }

        double time_spent = 0., tot_iters=0., num_mdps=0;
        while ((sde = readdir(sdr)) != NULL)
        {
            if (sde->d_name[0] == '.')
                continue;
            char file_path[256];
            strcpy(file_path, subdir_path);
            strcat(file_path, sde->d_name);

            num_mdps++;
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

                if (lambda>0.9) //TODO: fix
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

                double tmp = solve_mc(bags, bagsNum, *e, totV, totE, lambda, discSum);
//                if (totV >= 1241)
//                    break;
//                gaussian_solve_mc_discsum(*e, totV, totE, lambda, discSum);

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

                    for (j = 0; j < vector_total(choices_copy + *v); j++)
                    {
                        edge *cur = vector_get(choices_copy + *v, j);
                        if (cur->reward + lambda * discSum[cur->u] - maxDiscSum > 1.5) //TODO: threshold
                        {
                            maxDiscSum = cur->reward + lambda * discSum[cur->u];
                            newChoiceID = j;
                        }
                    }
                    assert(newChoiceID != -1);
                    if (newChoiceID != policy[*v])
                        done = 0;

                    policy[*v] = newChoiceID;

                }
                iter++;

                clock_t end = clock();
                time_spent += (double) (end - begin) / CLOCKS_PER_SEC;

                if (iter > 100)
                    printf("ghaaatesh kon lamassaaaaab\n");

            } while (!done);

            tot_iters += iter;

        }
        all_means[file_n] = tot_iters/num_mdps;
        strcpy(all_names[file_n], de->d_name);
        file_n++;
        printf("| %s | %f |\n", de->d_name, time_spent);
    }

    printf("\n\nMean number of iterations:\n");
    int i;
    for (i=0; i<file_n; i++)
        printf("| %s | %f |\n", all_names[i], all_means[i]);

    return 0;
}