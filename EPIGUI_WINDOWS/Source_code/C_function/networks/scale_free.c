#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n_vertices, n_0, n_edges;
int **L, *GC;

void edge_organizer ()
{
    int i,j,aux1[2],aux;
    for(i=0;i<n_edges;i++)
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
        for(j=aux;j<n_edges;j++)
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
    n_vertices = atoi(argv[1]);                 //Number of vertices
    n_0=atoi(argv[2]);                          //Parameter n_0 of the scale-free network of Barabási and Albert

    int i,j,k,aux,aux2,n_random,v_aux[n_vertices];
    double prob;
    srand((unsigned)time(NULL));

    n_edges = (int)((n_vertices-n_0)*n_0);      //Number of edges
    GC=(int *)malloc(n_vertices*sizeof(int));   //Vector of vertex connectivity degrees
    
    L=(int **)malloc(n_0*sizeof(int*));         //List containing vertex pairs
    for(i=0;i<n_0;i++) L[i]=(int *)malloc(2*sizeof(int));

    FILE *arq_exit;
    arq_exit=fopen("network.dat","w");          //network writing

    for(i=0;i<n_0;i++)
    {
        L[i][0]=i;
        L[i][1]=n_0;
        GC[i]++;
        GC[n_0]++;
    }
      
    aux=n_0;
    for(i=n_0+1;i<n_vertices;i++)
    {
        for(j=0;j<i;j++) v_aux[j]=0;
        aux2=0;
        while(aux2<n_0)
        {
            n_random=rand()%i;
            prob=rand()/(RAND_MAX+1.0);

            if(prob<(double) GC[n_random]/(2*n_0*(i-n_0)) && v_aux[n_random]==0)        //p(k_i)= GC[i]/(sum_j GC[j])
            {
                aux++;
                L = (int **)realloc(L, aux*sizeof(int *));
    	        for(k=aux-1; k<aux; k++) L[k]=(int *)malloc(2*sizeof(int));
                v_aux[n_random]=1;

                L[aux-1][0]=n_random;
                L[aux-1][1]=i;
                GC[n_random]++;
                GC[i]++;
                aux2++;
            }
        }
    }

    edge_organizer ();
    for(i=0;i<aux;i++)  fprintf(arq_exit,"%d\t%d\n",L[i][0],L[i][1]);

    for(i=0;i<aux;i++)  free(L[i]);
    free(L);
}