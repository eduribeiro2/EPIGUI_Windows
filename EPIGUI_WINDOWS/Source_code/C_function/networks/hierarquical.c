#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n_vertices,bh,m,n_edges;
double lam,ksi;
int *path1,*path2,**L;

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
    n_vertices = atoi(argv[1]);         //Number of vertices
    bh = atoi(argv[2]);                 //tree branch factor
    m = atoi(argv[3]);                  //Number of additional edges
    lam=atof(argv[4]);                  //Parameter lambda of the hierarquical network of Dodds, Watts and Sabel
    ksi=atof(argv[5]);                  //Parameter ksi of the hierarquical network of Dodds, Watts and Sabel
    n_edges = n_vertices+m-1;

    FILE *arq_exit=fopen("network.dat","w");

    int i,j,z,rand1,rand2,edges,Dij,p3,p1,p2,aux,aux2,aux3;
    double xij,prob;
    z=(int)n_vertices/bh;
    L=(int **)malloc(n_edges*sizeof(int*));
    for(i=0;i<n_edges;i++) L[i]=(int *)malloc(2*sizeof(int));

    //construct a tree with n_vertices nodes and branch factor bh
    aux=0;
    for(i=0;i<z;i++)
    {
        if(i==z-1.)
        {
            for(j=1.+(i*bh);j<n_vertices;j++)
            {
                L[aux][0]=i;
                L[aux][1]=j;
                aux++;
            }
        }
        else
        {
            for(j=1.+(i*bh);j<=(i+1.)*bh;j++)
            {
                L[aux][0]=i;
                L[aux][1]=j;
                aux++;
            }
        }
    }

    edges=0;
    z=(int)log(n_vertices)/log(bh)+1;
    path1=(int *)malloc(z*sizeof(int));
    path2=(int *)malloc(z*sizeof(int));

    while (edges<m)
    {
        // pick two nodes at random
        for(i=0;i<100000;i++)
        {
            rand1=(int)rand()%n_vertices;
            rand2=(int)rand()%n_vertices;
            if(rand1>rand2)
            {
                aux2=rand1;
                rand1=rand2;
                rand2=aux2;
            }
            aux3=0; 
            for(j=0;j<aux;j++)  if(L[j][0]==rand1 && L[j][1]==rand2) aux3=1;
            if(rand1!=rand2 && aux3==0) break;
        }

        // find di,dj and Dij
        path1[0]=rand1;
        path2[0]=rand2;

        for (i=1;i<z;i++)
        {
            path1[i]=(int)path1[i-1]/bh;
            path2[i]=(int)path2[i-1]/bh;
        }

        Dij=0;
        for (i=0;i<z;i++)
        {
            Dij=Dij+1;
            if(path1[i]==1)break;
        }

        p1=0;
        p3=0;
        for (i=0;i<z;i++)
        {
            p2=0;
            for (j=0;j<z;j++)
            {
                p2=p2+1;
                if(path1[i]==path2[j])
                {
                    p1=p1+1;
                    break;
                }
            }
            p3=p3+1;
            if(p1==1) break;
        }

        //di+dj is the the distance from ind1 to ind2 along the backbone 
        //hierarchy the level of the highest common node on the path to the root
        Dij=Dij-p3; 
        xij=sqrt(pow(p3-1.,2)+pow(p2-1.,2)-2);
        prob=rand()/(RAND_MAX+1.);

        //connect ind1 and ind2 with prob. e^(-Dij/lam)e^(-xij/ksi))
        if (prob<exp(-Dij/lam)*exp(-xij/ksi))
        {
            L[aux][0]=rand1;
            L[aux][1]=rand2;
            aux++;
            edges=edges+1;
        }
    }

    edge_organizer();
    for(i=0;i<n_edges;i++)  fprintf(arq_exit,"%d\t%d\n",L[i][0],L[i][1]);

    for(i=0;i<n_edges;i++)  free(L[i]);
    free(L);

}
