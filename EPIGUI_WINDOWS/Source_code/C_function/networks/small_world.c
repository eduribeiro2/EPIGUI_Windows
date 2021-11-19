#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n_vertices, n_0, n_rows;
double p;

int **L;

void edge_organizer ()
{
    int i,j,aux1[2],aux;
    for(i=0;i<n_rows;i++)
    {
        if(L[i][0]>L[i][1])
        {
            aux=L[i][0];
            L[i][0]=L[i][1];
            L[i][1]=aux;
        }
    }

    aux=0;
    for(i=0;i<n_vertices;i++)
    {
        for(j=aux;j<n_rows;j++)
        {
            if(L[j][0]==i)
            {
                aux1[0]=L[aux][0];
                aux1[1]=L[aux][1];
                L[aux][0]=L[j][0];
                L[aux][1]=L[j][1];
                L[j][0]=aux1[0];
                L[j][1]=aux1[1];
                aux++;
            }
        }
    }
}

void main(int argc, char *argv[])
{
    n_vertices = atoi(argv[1]);             //Number of vertices
    p = atof(argv[2]);                      //reconnection probability
    n_0 = atoi(argv[3]);                    //number of n_0 nearest neighbors

    int i,j,aux=0,aux2,n_random;
    double prob;
    srand((unsigned)time(NULL));

    n_rows =(int) (n_0*n_vertices);

    L=(int **)malloc(n_rows*sizeof(int*));
    for(i=0;i<n_rows;i++) L[i]=(int *)malloc(2*sizeof(int));
 
    FILE *arq_exit=fopen("network.dat","w");
    // building a regular network
    for(i=1;i<=n_0;i++)
    {
        for(j=0;j<n_vertices;j++)
        {
            L[aux][0]=j;
            L[aux][1]=(i+j)%n_vertices;
            aux++;
        }
    }

    //Edge reconnection process
    for(i=0;i<n_0*n_vertices;i++)
    {
        prob=rand()/(RAND_MAX+1.0);
        if(prob<p)
        {
            aux=1;
            while (aux==1)
            {
                aux=0;
                for(j=0;j<1e10;j++)
                {
                    n_random=rand()%n_vertices;
                    if(n_random!=L[i][0]) break;
                }

                for(j=0;j<n_0*n_vertices;j++)
                {
                    if((L[j][0]==L[i][0] && L[j][1]==n_random) || (L[j][0]==n_random && L[j][1]==L[i][0]))
                    {
                        aux=1;
                        break;
                    }
                }
            }
            L[i][1]=n_random;
        }
    }

    edge_organizer ();
    for(i=0;i<n_0*n_vertices;i++)   fprintf(arq_exit,"%d\t%d\n",L[i][0],L[i][1]);
}