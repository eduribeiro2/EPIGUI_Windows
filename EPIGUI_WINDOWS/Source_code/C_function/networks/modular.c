#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n_vertices,cluster;
double r,p;

void main(int argc, char *argv[])
{
    n_vertices = atoi(argv[1]);             //Number of vertices
    p = atof(argv[2]);                      //overall probability of attachment
    r = atof(argv[3]);                      //proportion of links within modules
    cluster = atoi(argv[4]);                //number of clusters/modules

    FILE *arq_exit=fopen("network.dat","w");

    int i,j,module_i,module_j;
    double z,n_random;

    z=p*(n_vertices-1);                     // z=p(n-1) - Erdos-Renyi average degree

    for(i=0;i<n_vertices-1;i++)
    {
        for(j=i+1;j<n_vertices;j++)
        {
        module_i=(int)(cluster*i/n_vertices);       //the module to which i belongs to
        module_j=(int)(cluster*j/n_vertices);       //the module to which j belongs to
            if (module_i==module_j)
            {
                //prob of attachment within module
                n_random=rand()/(RAND_MAX+1.);
                if(n_random<=r*z/(n_vertices/cluster-1.))
                {
                    fprintf(arq_exit,"%d\t%d\n",i,j);
                } 
            }
            else
            {
                //prob of attachment across modules
                n_random=rand()/(RAND_MAX+1.);
                if(n_random<=z*(1-r)/(n_vertices-n_vertices/cluster))
                {
                    fprintf(arq_exit,"%d\t%d\n",i,j);
                } 
            }
        }
    }
}