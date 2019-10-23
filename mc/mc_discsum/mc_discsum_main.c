//
// Created by kiarash on 10/22/19.
//

#include <dirent.h>
#include <string.h>
#include "../../utils/vector.h"
#include "mc_discsum_solve.h"

bag bags[MAXN];
vector e;
int totV, totE, bagsNum;
float discSum[MAXN], lambda;

void input(char file_path[])
{
    FILE *fp = fopen(file_path, "r");

    int i, j;

    //read mcTotV
    fscanf(fp, "%d%d%f", &totV, &totE, &lambda);

    vector_init(&e);
    //get edges
    for (i = 0; i < totE; i++)
    {
        edge *ej = malloc(sizeof(edge));
        fscanf(fp, "%d%d%f%f", &(ej->v), &(ej->u), &(ej->delta), &(ej->reward));
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

    char dir_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/benchmarks/dacapobenchmark/outputs/mc_discsum/asm-3.1/";
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dir_path);
    if (dr == NULL)
    {
        printf("Could not open current directory");
        return 0;
    }

    float time_spent = 0.;
    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_name[0] == '.')
            continue;
//        printf("%s\n", de->d_name);
//        char file_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/mc/mc_discsum/example3.txt";
        char file_path[256];
        strcpy(file_path, dir_path);
        strcat(file_path, de->d_name);

        memset(discSum, 0, sizeof(discSum));

        input(file_path);
        time_spent += solve_mc(bags, bagsNum, e, totV, totE, lambda, discSum);

//        int i;
//        for (i = 0; i <= totV; i++)
//            printf("Expected discounted sum of vertex %d: %f\n", i, discSum[i]);
    }
    printf("\nTime spent: %f seconds\n", time_spent);

    return 0;
}

