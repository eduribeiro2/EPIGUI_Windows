#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n_vertices;
double p;

void main(int argc, char *argv[])
{
    n_vertices = atoi(argv[1]);             //number of vertices
    p=atof(argv[2]);                        //parameter p of the random network of Erdos and Rényi

    int i,j;
    double prob;
    srand((unsigned)time(NULL));

    FILE *arq_exit=fopen("network.dat","w");   //network writing

    for(i=0;i<n_vertices-1;i++)
    {
        for(j=i+1;j<n_vertices;j++)
        {
            prob=rand()/(RAND_MAX+1.0);
            if(prob<p)  fprintf(arq_exit,"%d\t%d\n",i,j);
        }
    }
}