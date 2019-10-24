//
// Created by kiarash on 10/22/19.
//

#include <dirent.h>
#include <string.h>
#include "../../utils/vector.h"
#include "mc_discsum_solve.h"
#include "../../gaussian_elimination/gaussian_elim.h"

bag bags[MAXN];
vector e;
int totV, totE, bagsNum;
double discSum[MAXN], lambda;

void input(char file_path[])
{
    FILE *fp = fopen(file_path, "r");

    int i, j;

    //read mcTotV
    fscanf(fp, "%d%d%lf", &totV, &totE, &lambda);

    vector_init(&e);
    //get edges
    for (i = 0; i < totE; i++)
    {
        edge *ej = malloc(sizeof(edge));
        fscanf(fp, "%d%d%lf%lf", &(ej->v), &(ej->u), &(ej->delta), &(ej->reward));
        vector_add(&e, ej);
    }

    // read vertices and edges within bags
    fscanf(fp, "%d", &bagsNum);
    for (i = 0; i < bagsNum; i++)
    {
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
            edge *ej = vector_get(&e, ind);
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

    char dir_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/benchmarks/dacapobenchmark/outputs/mc_discsum/";
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dir_path);
    if (dr == NULL)
    {
        printf("Could not open current directory");
        return 0;
    }
    while ((de = readdir(dr)) != NULL)
    {
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
        double time_spent = 0.;
        while ((sde = readdir(sdr)) != NULL)
        {
            if (sde->d_name[0] == '.')
                continue;
            char file_path[256];
            strcpy(file_path, subdir_path);
            strcat(file_path, sde->d_name);

            memset(discSum, 0, sizeof(discSum));

            input(file_path);
//            time_spent += solve_mc(bags, bagsNum, e, totV, totE, lambda, discSum);
            time_spent += gaussian_solve_mc_discsum(e, totV, totE, lambda, discSum);

        }
        printf("| %s | %f |\n", de->d_name, time_spent);
    }

    return 0;
}

