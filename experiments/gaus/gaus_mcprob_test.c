//
// Created by kiarash on 10/20/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include "gaussian_elim.h"

int main ()
{
    char dir_path[256] = "/home/kiarash/Desktop/rmc/rmc-treewidth-code/benchmarks/dacapobenchmark/outputs/asm-3.1_mcprob_eqs/";
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(dir_path);
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
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
        FILE *fp = fopen(file_path, "r");

        int n;
        fscanf(fp, "%d", &n);

        float A[n][n];
        float B[n];

        int i, j;
        for (i=0; i<n; i++)
            for (j=0; j<n; j++)
                fscanf(fp, "%f", &A[i][j]);

        for (i=0; i<n; i++)
            fscanf(fp, "%f", &B[i]);

        fclose(fp);

        clock_t begin = clock();
        int has_solution = Gaussian_Elimination(&A[0][0], n, B);
        clock_t end = clock();

        if (!has_solution)
            printf("No solution: %s\n", de->d_name);
        else
        {
            ;
//            for (i=0; i<n; i++)
//                printf("Hitting prob of vertex %d: %f\n", i, B[i]);
        }

        time_spent += (double) (end - begin) / CLOCKS_PER_SEC;
    }

    closedir(dr);

    printf("\nTime spent: %f seconds\n", time_spent);

    return 0;
}