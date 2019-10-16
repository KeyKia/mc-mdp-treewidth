//
// Created by kiarash on 9/24/19.
//

//TODO: input assumptions:
/* Root is bag 0 and contains exactly two vertices including the target
 * Not anymore: Edges of the tdc are from parent (v) to child (u)
 * All possible edges of each bag are given in edges (even with 0 prob)
 * (and even all the loops - which might have 0 prob)
 * All edges are given in both directions (even with 0 prob)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "vector.h"
#include "tree_decomposition.h"

vector e; //contains all possible edges of each bag (even with 0 prob)
bag bags[MAXN]; //the bags
int totV,totE,target; // vertices, edges, and target in the Markov chain

bool input()
{

    FILE* fp=fopen("/home/kiarash/Desktop/rmc/rmc-treewidth-code/benchmark/dacapobenchmark/outputs/eclipse_myformat/createInternalURLStreamHandler.txt","r");
    int i,j;

    //read totV
    fscanf(fp, "%d%d%d", &totV, &totE, &target);

    vector_init(&e);
    //get edges
    for(i=0;i<totE;i++)
    {
        edge *ej = malloc(sizeof(edge));
        fscanf(fp, "%d%d%f", &(ej->v), &(ej->u), &(ej->prob));
        if (ej->v==target)
            ej->prob=0.0; //remove edges leaving target -- causes no change in hitting probs
        vector_add(&e, ej);
    }

    // read vertices and edges within bags
    int n;
    fscanf(fp,"%d",&n);
    for(i=0;i<n;i++)
    {
        bag_init(bags+i,i);
        int V,E;
        fscanf(fp,"%d%d",&V,&E);
        if(i==0 && V!=2){
            printf("All hitting probabilities are zero (first/root bag has %d vertices\n", V);
            return false;
        }

        // read vertices within bag i
        for(j=0;j<V;j++)
        {
            int *x=malloc(sizeof(int));
            fscanf(fp,"%d",x);
            bag_add_vertex(bags+i,x);
        }

        // read edges within bag i
        for(j=0;j<E;j++)
        {
            int ind;
            fscanf(fp,"%d",&ind);
            edge *ej=vector_get(&e,ind);
            bag_add_edge(bags+i,ej);
        }
    }

    //read the tree edges between bags
    // assumptions: root is bag 0 and contains the target
    // Not anymore: and edges are from parent(v) to child(u)
    for(i=0;i+1<n;i++)
    {
        int *v=malloc(sizeof(int)),*u=malloc(sizeof(int));
        fscanf(fp,"%d%d",v,u);
        bag_add_kid(bags+*v,bags+*u);
        bag_add_kid(bags+*u,bags+*v);
    }
    fclose(fp);

    return true;
}


/*first removes all its kids without changing hitting
  probabilities of the remaining part
  then removes the current bag
*/
void dfs_up(int curIndex,int parIndex)
{
//    bag_print(bags+curIndex);
//    fflush(stdout);
    int i;
    for(i=0;i<bags[curIndex].kidCnt;i++)
        if(get_kid(bags+curIndex,i)-bags!=parIndex)
            dfs_up(get_kid(bags+curIndex,i)-bags,curIndex);
    if(parIndex!=-1)
    {
        while(get_first_extra_vertex(bags + curIndex, bags + parIndex)!=-1)
        {
            int v=get_first_extra_vertex(bags+curIndex,bags+parIndex);
            remove_vertex(bags+curIndex,v);
        }
    }

}

/*starting from root which has only two vertices (including
 * target), returns all vertices in each bag in the reversed
 * order they were removed in dfs_up. While returning computes
 * the correct hitting probability of the returned vertex
 */
void dfs_down(int curIndex,int parIndex)
{
    int i;
    for(i=0;i<bags[curIndex].kidCnt;i++)
    {

        bag *tmp=get_kid(bags+curIndex,i);
        int kidIndex=tmp->id;
        if(kidIndex==parIndex)
            continue;
        while(get_last_extra_vertex(get_kid(bags+curIndex,i),bags+curIndex)!=-1)
        {
            int v=get_last_extra_vertex(get_kid(bags+curIndex,i),bags+curIndex);
            printf("returning vertex %d from bag %d\n", v, get_kid(bags+curIndex,i)->id);
            return_vertex(get_kid(bags+curIndex,i),v);
        }

        dfs_down(kidIndex,curIndex);
    }

}


void solve()
{
    memset(res,0,sizeof(res));
    dfs_up(0,-1);
    printf("\ndfs up finished -- two-vertex root bag now:\n");
    bag_print(bags);
    init_res(bags,target);
    memset(removed,0,sizeof(removed));
    dfs_down(0,-1);
    printf("\ndfs_down finished.\n-------------------------------\n\n");
}

void show_results()
{
    int i;
    for(i=0;i<totV;i++)
        printf("Hitting prob of vertex %d: %f\n", i, res[i]);
}

int main()
{
    bool done = input();
    if (!done)
        return 0;
    clock_t begin = clock();
    solve();
    clock_t end = clock();
    show_results();
    float time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime spent: %f seconds\n", time_spent);

    return 0;
}
