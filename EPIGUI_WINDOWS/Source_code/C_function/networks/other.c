#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INFINITY 1e7;

int n_vertices, n_rows, n_columns;
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

void open_file(char filechar[100])
{
    FILE *fileptr;
    char chr;
    int i,j,aux,smaller =100000;
    fileptr = fopen(filechar, "r");

    n_vertices = 0;
    n_rows = 0;
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
            n_rows++;
        }
        chr = getc(fileptr);
    }
    n_columns = n_columns/n_rows; 
    fclose(fileptr);

    L = (int**) malloc(n_rows * sizeof(int*));
    fileptr = fopen(filechar, "r");
    for(i=0;i<n_rows;i++)
    {
        L[i] = (int*) malloc(n_columns * sizeof(int));
        for(j=0;j<n_columns;j++)    fscanf(fileptr,"%d",&L[i][j]);

        if (L[i][0] < smaller )
        {
            smaller  = L[i][0];
        }
        if (L[i][1] < smaller )
        {
            smaller  = L[i][1];
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

    if(smaller  == 0)  n_vertices+=1;
    else   
    {
        for(i=0;i<n_rows;i++)
        {
            L[i][0]-=smaller ;
            L[i][1]-=smaller ;
        } 
        n_vertices = n_vertices-smaller +1;
    }
    fclose(fileptr);
}

void main(int argc, char *argv[])
{
    char filechar[100];
    int i,j,aux;

    strcpy(filechar, argv[1]);
    open_file(filechar);
    edge_organizer();

    FILE *arq_exit;
    arq_exit = fopen("network.dat","w");

    fprintf(arq_exit,"%d\t%d\n",L[0][0],L[0][1]); 
    for(i=1;i<n_rows;i++)
    {
        aux=0;
        for(j=0;j<i;j++)
        {
           if (L[i][0]==L[j][0] && L[i][1]==L[j][1])
            {
                aux=1;
                break;
            }   
        }    
        if (aux==0) fprintf(arq_exit,"%d\t%d\n",L[i][0],L[i][1]);
    }
}
