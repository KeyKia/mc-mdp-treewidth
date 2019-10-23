//
// Created by kiarash on 9/24/19.
//

#include <dirent.h>
#include <string.h>
#include "../../utils/vector.h"
#include "mc_hitprob_solve.h"

bag bags[MAXN];
vector e;
int totV, totE, target, bagsNum;
float res[MAXN];

void input(char file_path[])
{
    FILE *fp = fopen(file_path, "r");

    int i, j;

    //read mcTotV
    fscanf(fp, "%d%d%d", &totV, &totE, &target);

    vector_init(&e);
    //get edges
    for (i = 0; i < totE; i++)
    {
        edge *ej = malloc(sizeof(edge));
        fscanf(fp, "%d%d%f", &(ej->v), &(ej->u), &(ej->delta));
        if (ej->v == target)
            ej->delta = 0.0; //remove edges leaving mcTarget -- causes no change in hitting probs
        vector_add(&e, ej);
    }

    // read vertices and edges within mcBags
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

    char dir_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/benchmarks/dacapobenchmark/outputs/asm-3.1_mcprob_format/";
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dir_path);
    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
    }

    float time_spent = 0.;
    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_name[0] == '.')
            continue;
        char file_path[256];
        strcpy(file_path, dir_path);
        strcat(file_path, de->d_name);

        memset(res, 0, sizeof(res));

        input(file_path);
        time_spent += solve_mc(bags, bagsNum, e, totV, totE, target, res);

//        int i;
//        for (i = 0; i < totV; i++)
//            printf("Hitting prob of vertex %d: %f\n", i, res[i]);
    }
    printf("\nTime spent: %f seconds\n", time_spent);

    return 0;
}