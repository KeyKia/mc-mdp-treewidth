//
// Created by kiarash on 9/24/19.
//

#include "../utils/vector.h"
#include "mc_solve.h"

vector e; //contains all possible edges of each bag (even with 0 delta)
bag bags[MAXN]; //the bags
int totV, totE, target, bagsNum; // vertices, edges, and mcTarget in the Markov chain

void input()
{

    FILE *fp = fopen(
            "/home/kiarash/Desktop/rmc/rmc-treewidth-code/mc/example.txt",
            "r");
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
    input();
    float res[MAXN];

    clock_t begin = clock();
    solve_mc(bags, bagsNum, e, totV, totE, target, res);
    clock_t end = clock();

    float time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    int i;
    for (i = 0; i < totV; i++)
        printf("Hitting prob of vertex %d: %f\n", i, res[i]);
    printf("\nTime spent: %f seconds\n", time_spent);

    return 0;
}
