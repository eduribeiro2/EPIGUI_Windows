#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INFINITY 1e3

int n_vertices, n_edges, n_columns, bigger, pos;
int **L, *GC, *rede_aux, *E, **D, *vector_aux;
double *CC;

void real_networks(char filechar[100])
{
    FILE *fileptr;
    char chr;
    int i,j,smaller=100000;

    fileptr = fopen(filechar, "r");

    n_vertices = 0;
    n_edges = 0;
    n_columns = 0;

    chr = getc(fileptr);
    while (chr != EOF)
    {
        if (chr == ' ' || chr == '\t' || chr == '\n')
        {
            n_columns++;
        }

        if (chr == '\n')
        {
            n_edges++;
        }
        chr = getc(fileptr);
    }
    n_columns = n_columns/n_edges; 
    fclose(fileptr);

    L = (int**) malloc(n_edges * sizeof(int*));
    fileptr = fopen(filechar, "r");
    for(i=0;i<n_edges;i++)
    {
        L[i] = (int*) malloc(n_columns * sizeof(int));
        for(j=0;j<n_columns;j++)    fscanf(fileptr,"%d",&L[i][j]);

        if (L[i][0] < smaller)
        {
            smaller = L[i][0];
        }
        if (L[i][1] < smaller)
        {
            smaller = L[i][1];
        }
        if (L[i][0]>n_vertices)
        {
            n_vertices = L[i][0];
        }
        if (L[i][1]>n_vertices)
        {
            n_vertices = L[i][1];
        }
    }

    if(smaller == 0)  n_vertices+=1;
    else   
    {
        for(i=0;i<n_edges;i++)
        {
            L[i][0]-=smaller;
            L[i][1]-=smaller;
        } 
        n_vertices = n_vertices-smaller+1;
    }

    fclose(fileptr);

    vector_aux = (int*)                          malloc(n_vertices * sizeof(int)); 
    GC = (int*)                                 malloc(n_vertices * sizeof(int)); 
    rede_aux = (int*)                           malloc(n_vertices * sizeof(int)); 
    E = (int*)                                  malloc(n_vertices * sizeof(int)); 
    CC = (double*)                              malloc(n_vertices * sizeof(double)); 
    D = (int**)                                 malloc(n_vertices * sizeof(int*));
    for(i=0;i<n_vertices;i++)   D[i] = (int*)   malloc(n_vertices * sizeof(int));
}

void degree()
{
    int i;

    for (i=0;i<n_vertices;i++)  GC[i]=0;

    for (i=0;i<n_edges;i++)
    {
        GC[L[i][0]]++;
        GC[L[i][1]]++;
    }
}

void organizer()
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

void summation()
{
    organizer();
    int i,aux=0;
    for(i=0;i<n_vertices;i++) rede_aux[i]=0;

    for(i=0;i<n_edges;i++)
    {
        if (L[i][0]==aux)
        {
            rede_aux[aux]++;
        }
        else
        {
            aux++;
            rede_aux[aux]=rede_aux[aux-1];
            i--;
        }
    }
}

double clustering_coef ()
{
    int i, j, k, l;
    double sum;

    for(i=0;i<n_vertices;i++) E[i]=0;

    for(i=0;i<n_vertices;i++)
    {
        if(i==0)
        {
            for(j=0;j<rede_aux[0];j++)
            {
                for(k=rede_aux[L[j][1]-1];k<rede_aux[L[j][1]];k++)
                {
                    for (l=0;l<rede_aux[0];l++)
                    {
                        if(L[k][1]==L[l][1]) 
                        {
                            E[0]++;
                            E[L[j][1]]++;
                            E[L[l][1]]++;
                            break;
                        }
                    }
                }
            }
        }
        
        else
        {
            for(j=rede_aux[i-1];j<rede_aux[i];j++)
            {
                for(k=rede_aux[L[j][1]-1];k<rede_aux[L[j][1]];k++)
                {
                    for (l=rede_aux[i-1];l<rede_aux[i];l++)
                    {
                        if(L[k][1]==L[l][1]) 
                        {
                            E[i]++;
                            E[L[j][1]]++;
                            E[L[l][1]]++;
                            break;
                        }
                    }
                }
            }
        }
    }

    sum=0.0;
    for (i=0;i<n_vertices;i++)
    {
        if (GC[i]!=1 && GC[i]!=0)   CC[i]=(double)2.0*E[i]/(GC[i]*(GC[i]-1));
        else                        CC[i]=0.0;
        if(vector_aux[i]==pos)   sum+=CC[i];
    }

    return sum/bigger;
}

void shortest_path(double *Dist, double *E, int *diameter)
{
    int i,j,k,sum2,aux=1,count,density1,density2;
    double sum1;

    bigger=-1e3;
    *diameter = 0;

    for(i=0;i<n_vertices;i++) for(j=i;j<n_vertices;j++)     D[i][j]=D[j][i]=INFINITY; 
    for(i=0;i<n_edges;i++)                                 D[L[i][0]][L[i][1]] = D[L[i][1]][L[i][0]] = 1;

    for(k=0;k<n_vertices;k++)
    {
        for(i=0;i<n_vertices-1;i++)  
        {
            for(j=i+1;j<n_vertices;j++)   if(D[i][j]>D[i][k]+D[k][j])     D[i][j] = D[j][i] = D[i][k] + D[k][j];
        }
    }

    for(i=0;i<n_vertices;i++)
    {
        D[i][i]=0;
        vector_aux[i]=0;
    } 

    for(i=0;i<n_vertices;i++)  
    {
        if(vector_aux[i]==0)
        {
            for(j=0;j<n_vertices;j++)
            if(D[i][j]!=INFINITY)
            {
                vector_aux[j]=aux;
            }
            aux++;
        }
    }

    for(i=1;i<aux;i++)
    {
        count=0;
        for(j=0;j<n_vertices;j++)   if(vector_aux[j]==i) count++;

        if (count > bigger)
        {
            bigger=count;
            pos=i;
        }
        if (count == bigger && pos!=i)
        {
            density1=density2=0;
            for(j=0;j<n_vertices;j++)
            {
                if(vector_aux[j]==pos) density1+=GC[j];
                if(vector_aux[j]==i) density2+=GC[j];
            }   
            if (density2>density1)
            {
                bigger=count;
                pos=i;
            }
        }
    }  

    sum1 = 0.0;
    sum2 = 0;
    for(i=0;i<n_vertices;i++)
    {
        if(vector_aux[i]==pos)
        {
            for(j=0;j<n_vertices;j++) 
            {
                if(vector_aux[j]==pos && i!=j)
                {
                    if (D[i][j]> *diameter)   *diameter=D[i][j];
                    sum1=sum1+1.0/D[i][j];
                    sum2=sum2+D[i][j];
                }
            }
        } 
    }   
    *E    = sum1/(bigger*(bigger-1.0));
    *Dist = sum2/(bigger*(bigger-1.0));
}

void main(int argc, char *argv[])
{
    int i,sum=0;
    char filechar[100];
    strcpy(filechar, argv[1]);

    FILE *arq_exit = fopen("measures.dat","w"); 

    double average_degree,average_cc;
    int diameter;

    double dist, efficiency;

    real_networks(filechar);
    summation();
    degree();
    shortest_path(&dist,&efficiency,&diameter);

    for (i=0;i<n_vertices;i++) if(vector_aux[i]==pos)   sum+=GC[i];
    average_degree = (double)sum/bigger;
    average_cc = clustering_coef();

    fprintf(arq_exit,"%lf %lf %lf %lf %lf %d",average_degree,average_degree/(bigger-1),average_cc,dist,efficiency,diameter);
    fclose(arq_exit);
}