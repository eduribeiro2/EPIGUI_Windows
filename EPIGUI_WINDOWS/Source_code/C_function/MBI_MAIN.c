#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <time.h>
#include <string.h>

int n_individuals, final_time, n_sim, n_edges;
double dt;

int sum, n_random, n_compartments, n_characteristics;
double mu;

int main(int argc, char *argv[]);

int *ODE_inital, *rede_aux, **L, **ODE_FINAL, *n_output, ***input_cont, ***input_disc;
double **POP, *parameters_cont, *parameters_disc, *parameters_rate;
char **output_type;

void initial_condition (int stage, int value)
{
    sum=0;
    while (sum<value)
    {
        n_random=rand()%n_individuals;
        if(POP[n_random][0]==0.0)
        {
            POP[n_random][0]=(double)stage;
            sum++;
        }
    }
}

void c1 ()
{
    int i;

    for(i=0;i<n_individuals;i++) POP[i][0]=0.0;

    for(i=1;i<n_compartments;i++)  initial_condition(i, ODE_inital[i]);
}

void c2 (int j)
{
    int i;
    for(i=0;i<n_individuals;i++) POP[i][j]=0.;
}

void c3 (int j, int restriction, double parameter)
{
    int i;

    double z;
    if (restriction==1000)
    {
        for(i=0;i<n_individuals;i++)
        {
            z=rand()/(RAND_MAX+1.);
            POP[i][j]=-parameter*log(z);
        }
    }
    else
    {
        for(i=0;i<n_individuals;i++)
        {
            if(POP[i][0]==(double) restriction)
            {
                z=rand()/(RAND_MAX+1.);
                POP[i][j]=-parameter*log(z);
            }
        }
    }
}

void characteristics(char letter)
{
    int i,j,k,l;

    c1();

    if (letter=='Y')
    {
        c2(1);
        c3(2,1000, mu);
        
        for(i=3;i<n_characteristics;i=i+2)
        {
            for(j=0;j<n_compartments;j++)
            {
                for(k=0;k<n_output[j];k++)
                {
                    if(input_cont[j][k][3]==(int)(i-3)/2)
                    {
                        c2(i);
                        c3(i+1,j,parameters_cont[(int)(i-3)/2]);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        for(i=1;i<n_characteristics;i=i+2)
        {
            for(j=0;j<n_compartments;j++)
            {
                for(k=0;k<10;k++)
                {
                    if(input_cont[j][k][3]==(int)(i-1)/2)
                    {
                        c2(i);
                        c3(i+1,j,parameters_cont[(int)(i-1)/2]);
                        break;
                    }
                }
            }
        }
    }
}

int sum_compartment (int j)
{
    int k;
    sum=0;
    for(k=0;k<n_individuals;k++)   if (POP[k][0]==(double)j) sum++;
    return sum;
}

void print_MA()
{
    int i,j;
    double t=dt;
    FILE *arq_exit;
    arq_exit= fopen("IBM_result.dat","w");

    fprintf(arq_exit,"%lf\t",(double)0);
    for (j=0;j<n_compartments;j++) fprintf(arq_exit,"%lf\t",(double)ODE_inital[j]);
    fprintf(arq_exit,"\n");

    for (i=0;i<final_time;i++)
    {
        fprintf(arq_exit,"%lf\t",t);
        for (j=0;j<n_compartments;j++) fprintf(arq_exit,"%lf\t",(double)ODE_FINAL[i][j]/(n_sim));
        fprintf(arq_exit,"\n");
        t=t+dt;
    }
    fclose(arq_exit);
}

void MBI_without()
{
    int i,j,k,l,m,aux_contact;
    double prob,z;

    for (i=0;i<n_individuals;i++)    for(j=0;j<n_characteristics;j++)    POP[i][j]=0.0; 

    characteristics('N');

    for(i=0;i<final_time;i++)
    {
        for(j=0;j<n_individuals;j++)
        {
            for (k=0;k<n_compartments;k++)
            { 
                if(POP[j][0]==k)
                {
                    for(l=0;l<n_output[k];l++)
                    {
                        if(output_type[k][l] == 'c')      //output type = c-continuous d-discreet
                        {
                            if(POP[j][2*input_cont[k][l][3]+1]>POP[j][2*input_cont[k][l][3]+2])
                            {
                                if(input_cont[k][l][2]==1000)
                                {
                                    POP[j][0]=input_cont[k][l][1];
                                    if(n_output[input_cont[k][l][1]]!=0)
                                    {
                                        for(m=0;m<n_output[input_cont[k][l][1]];m++)
                                        {
                                            if (input_cont[input_cont[k][l][1]][m][0] != -1000) 
                                            {
                                                z=rand()/(RAND_MAX+1.);
                                                POP[j][2*input_cont[input_cont[k][l][1]][m][3]+1]=0.0;
                                                POP[j][2*input_cont[input_cont[k][l][1]][m][3]+2]=-parameters_cont[input_cont[input_cont[k][l][1]][m][3]]*log(z);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    prob=rand()/(RAND_MAX+1.);
                                    if(prob<parameters_rate[input_cont[k][l][4]])
                                    {
                                        POP[j][0]=input_cont[k][l][1];
                                        if(n_output[input_cont[k][l][1]]!=0)
                                        {
                                            for(m=0;m<n_output[input_cont[k][l][1]];m++)
                                            {
                                                if (input_cont[input_cont[k][l][1]][m][0] != -1000) 
                                                {
                                                    z=rand()/(RAND_MAX+1.);
                                                    POP[j][2*input_cont[input_cont[k][l][1]][m][3]+1]=0.0;
                                                    POP[j][2*input_cont[input_cont[k][l][1]][m][3]+2]=-parameters_cont[input_cont[input_cont[k][l][1]][m][3]]*log(z);
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        POP[j][0]=input_cont[k][l][2];
                                        if(n_output[input_cont[k][l][2]]!=0)
                                        {
                                            for(m=0;m<n_output[input_cont[k][l][2]];m++)
                                            {
                                                if (input_cont[input_cont[k][l][2]][m][0] != -1000) 
                                                {
                                                    z=rand()/(RAND_MAX+1.);
                                                    POP[j][2*input_cont[input_cont[k][l][2]][m][3]+1]=0.0;
                                                    POP[j][2*input_cont[input_cont[k][l][2]][m][3]+2]=-parameters_cont[input_cont[input_cont[k][l][2]][m][3]]*log(z);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                POP[j][2*input_cont[k][l][3]+1]+=dt;
                            }
                            
                        }
                        if(output_type[k][l] == 'd')
                        {
                            for (m=0;m<1000;m++)
                            {
                                n_random=rand()%n_individuals;
                                if(n_random!=j) break;
                            }
                            aux_contact=contact(n_random,j);

                            if(POP[n_random][0]*aux_contact==input_disc[k][l][0])
                            {
                                prob=rand()/(RAND_MAX+1.);
                                if(prob<parameters_disc[input_disc[k][l][4]])
                                {
                                    if(input_disc[k][l][3] == 1000)
                                    {
                                        POP[j][0]=input_disc[k][l][2];
                                        if(n_output[input_disc[k][l][2]]!=0)
                                        {
                                            for(m=0;m<n_output[input_disc[k][l][2]];m++)
                                            {
                                                if (input_cont[input_disc[k][l][2]][m][0] != -1000) 
                                                {
                                                    z=rand()/(RAND_MAX+1.);
                                                    POP[j][2*input_cont[input_disc[k][l][2]][m][3]+1]=0.0;
                                                    POP[j][2*input_cont[input_disc[k][l][2]][m][3]+2]=-parameters_cont[input_cont[input_disc[k][l][2]][m][3]]*log(z);
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        prob=rand()/(RAND_MAX+1.);
                                        if(prob<parameters_rate[input_disc[k][l][5]])
                                        {
                                            POP[j][0]=input_disc[k][l][2];
                                            if(n_output[input_disc[k][l][2]]!=0)
                                            {
                                                for(m=0;m<n_output[input_disc[k][l][2]];m++)
                                                {
                                                    if (input_cont[input_disc[k][l][2]][m][0] != -1000) 
                                                    {
                                                        z=rand()/(RAND_MAX+1.);
                                                        POP[j][2*input_cont[input_disc[k][l][2]][m][3]+1]=0.0;
                                                        POP[j][2*input_cont[input_disc[k][l][2]][m][3]+2]=-parameters_cont[input_cont[input_disc[k][l][2]][m][3]]*log(z);
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            POP[j][0]=input_disc[k][l][3];
                                            if(n_output[input_disc[k][l][3]]!=0)
                                            {
                                                for(m=0;m<n_output[input_disc[k][l][3]];m++)
                                                {
                                                    if (input_cont[input_disc[k][l][3]][m][0] != -1000) 
                                                    {
                                                        z=rand()/(RAND_MAX+1.);
                                                        POP[j][2*input_cont[input_disc[k][l][3]][m][3]+1]=0.0;
                                                        POP[j][2*input_cont[input_disc[k][l][3]][m][3]+2]=-parameters_cont[input_cont[input_disc[k][l][3]][m][3]]*log(z);
                                                    }
                                                }
                                            }
                                        }
                                        
                                    }
                                }
                            }
                        }
                        if(POP[j][0]!=k)    break;//changed state  
                    }
                    break;
                }
            }
        }
        
        for(j=0;j<n_compartments;j++) ODE_FINAL[i][j]+=sum_compartment(j);
    }
}

void MBI_within()
{
    int i,j,k,l,m,aux,aux_contact;
    double prob,z;

    for (i=0;i<n_individuals;i++)    for(j=0;j<n_characteristics;j++)    POP[i][j]=0.0; 

    characteristics('Y');

    for(i=0;i<final_time;i++)
    {
        for(j=0;j<n_individuals;j++)
        {
            if(POP[j][1]>POP[j][2])
            {
                z=rand()/(RAND_MAX+1.);
                POP[j][0]=0.0;
                POP[j][1]=0.0;
                POP[j][2]= -mu*log(z);
            }
            else
            {
                for (k=0;k<n_compartments;k++)
                { 
                    if(POP[j][0]==k)
                    {
                        for(l=0;l<n_output[k];l++)
                        {
                            if(output_type[k][l] == 'c')      //output type = c-continuous d-discreet
                            {
                                if(POP[j][2*input_cont[k][l][3]+3]>POP[j][2*input_cont[k][l][3]+4])
                                {
                                    if(input_cont[k][l][2]==1000)
                                    {
                                        POP[j][0]=input_cont[k][l][1];
                                        if(n_output[input_cont[k][l][1]]!=0)
                                        {
                                            for(m=0;m<n_output[input_cont[k][l][1]];m++)
                                            {
                                                if (input_cont[input_cont[k][l][1]][m][0] != -1000) 
                                                {
                                                    z=rand()/(RAND_MAX+1.);
                                                    POP[j][2*input_cont[input_cont[k][l][1]][m][3]+3]=0.0;
                                                    POP[j][2*input_cont[input_cont[k][l][1]][m][3]+4]=-parameters_cont[input_cont[input_cont[k][l][1]][m][3]]*log(z);
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        prob=rand()/(RAND_MAX+1.);
                                        if(prob<parameters_rate[input_cont[k][l][4]])
                                        {
                                            POP[j][0]=input_cont[k][l][1];
                                            if(n_output[input_cont[k][l][1]]!=0)
                                            {
                                                for(m=0;m<n_output[input_cont[k][l][1]];m++)
                                                {
                                                    if (input_cont[input_cont[k][l][1]][m][0] != -1000) 
                                                    {
                                                        z=rand()/(RAND_MAX+1.);
                                                        POP[j][2*input_cont[input_cont[k][l][1]][m][3]+3]=0.0;
                                                        POP[j][2*input_cont[input_cont[k][l][1]][m][3]+4]=-parameters_cont[input_cont[input_cont[k][l][1]][m][3]]*log(z);
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            POP[j][0]=input_cont[k][l][2];
                                            if(n_output[input_cont[k][l][2]]!=0)
                                            {
                                                for(m=0;m<n_output[input_cont[k][l][2]];m++)
                                                {
                                                    if (input_cont[input_cont[k][l][2]][m][0] != -1000) 
                                                    {
                                                        z=rand()/(RAND_MAX+1.);
                                                        POP[j][2*input_cont[input_cont[k][l][2]][m][3]+3]=0.0;
                                                        POP[j][2*input_cont[input_cont[k][l][2]][m][3]+4]=-parameters_cont[input_cont[input_cont[k][l][2]][m][3]]*log(z);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    POP[j][2*input_cont[k][l][3]+3]+=dt;
                                }
                                
                            }
                            if(output_type[k][l] == 'd')
                            {
                                for (m=0;m<1000;m++)
                                {
                                    n_random=rand()%n_individuals;
                                    if(n_random!=j) break;
                                }
                                aux_contact=contact(n_random,j);

                                if(POP[n_random][0]*aux_contact==input_disc[k][l][0])
                                {
                                    prob=rand()/(RAND_MAX+1.);
                                    if(prob<parameters_disc[input_disc[k][l][4]])
                                    {
                                        if(input_disc[k][l][3] == 1000)
                                        {
                                            POP[j][0]=input_disc[k][l][2];
                                            if(n_output[input_disc[k][l][2]]!=0)
                                            {
                                                for(m=0;m<n_output[input_disc[k][l][2]];m++)
                                                {
                                                    if (input_cont[input_disc[k][l][2]][m][0] != -1000) 
                                                    {
                                                        z=rand()/(RAND_MAX+1.);
                                                        POP[j][2*input_cont[input_disc[k][l][2]][m][3]+3]=0.0;
                                                        POP[j][2*input_cont[input_disc[k][l][2]][m][3]+4]=-parameters_cont[input_cont[input_disc[k][l][2]][m][3]]*log(z);
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            prob=rand()/(RAND_MAX+1.);
                                            if(prob<parameters_rate[input_disc[k][l][5]])
                                            {
                                                POP[j][0]=input_disc[k][l][2];
                                                if(n_output[input_disc[k][l][2]]!=0)
                                                {
                                                    for(m=0;m<n_output[input_disc[k][l][2]];m++)
                                                    {
                                                        if (input_cont[input_disc[k][l][2]][m][0] != -1000) 
                                                        {
                                                            z=rand()/(RAND_MAX+1.);
                                                            POP[j][2*input_cont[input_disc[k][l][2]][m][3]+3]=0.0;
                                                            POP[j][2*input_cont[input_disc[k][l][2]][m][3]+4]=-parameters_cont[input_cont[input_disc[k][l][2]][m][3]]*log(z);
                                                        }
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                POP[j][0]=input_disc[k][l][3];
                                                if(n_output[input_disc[k][l][3]]!=0)
                                                {
                                                    for(m=0;m<n_output[input_disc[k][l][3]];m++)
                                                    {
                                                        if (input_cont[input_disc[k][l][3]][m][0] != -1000) 
                                                        {
                                                            z=rand()/(RAND_MAX+1.);
                                                            POP[j][2*input_cont[input_disc[k][l][3]][m][3]+3]=0.0;
                                                            POP[j][2*input_cont[input_disc[k][l][3]][m][3]+4]=-parameters_cont[input_cont[input_disc[k][l][3]][m][3]]*log(z);
                                                        }
                                                    }
                                                }
                                            }
                                            
                                        }
                                    }
                                }
                            }
                            if(POP[j][0]!=k)    break;//changed state   
                        }
                        break;
                    }
                }
                POP[j][1]+=dt;
            }
        }
        for(j=0;j<n_compartments;j++) ODE_FINAL[i][j]+=sum_compartment(j);
    }
}

void reading()
{
    int i;
    char chr;
    FILE *input_file;
    input_file=fopen("network.dat","r");

    chr = getc(input_file);
    while (chr != EOF)
    {
        if (chr == '\n')    n_edges++;
        chr = getc(input_file);
    }
    fclose(input_file);

    L = (int**) malloc(n_edges * sizeof(int*));
    input_file = fopen("network.dat", "r");

    for(i=0;i<n_edges;i++)
    {
        L[i] = (int*) malloc(2 * sizeof(int));
        fscanf(input_file,"%d",&L[i][0]);
        fscanf(input_file,"%d",&L[i][1]);
    }
    fclose(input_file);
    summation();
}

void summation()
{
    int i,aux=0;
    for(i=0;i<n_individuals;i++) rede_aux[i]=0;

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

int contact(int source, int target)
{
    int i,min,max,aux=0;
    if (source<target)
    {
        min=source;
        max=target;
    }
    else
    {
        min=target;
        max=source;
    }

    if (min==0)
    {
        for (i=0;i<rede_aux[0];i++)
        {
            if (L[i][1]==max)
            {
                aux=1;
                break;
            }
        }
    }
    else
    {
        for (i=rede_aux[min-1];i<rede_aux[min];i++)
        {
            if (L[i][1]==max)
            {
                aux=1;
                break;
            }
        }
    }

    return aux;
}

int main(int argc, char *argv[])
{
    n_sim=atoi(argv[1]);
    dt = atof(argv[2]);
    final_time = (int)(atoi(argv[3])/dt);
    n_individuals = atoi(argv[4]);

    rede_aux    = (int*)    malloc (n_individuals * sizeof(int));

    int i,j,k;
    int n_parameters_cont, n_parameters_disc, n_parameters_rate;
    char dummy,letter[2];
    srand(time(NULL));

    FILE *input_file;
    input_file = fopen("INPUT_IBM.dat","r");
    fscanf(input_file,"%d",&n_compartments);

    ODE_inital  = (int*)    malloc (n_compartments * sizeof(int));
    n_output    = (int*)    malloc (n_compartments * sizeof(int));

    ODE_FINAL   = (int**)   malloc (final_time * sizeof(int*));
    for (i=0;i<final_time;i++) ODE_FINAL[i]=(int*) malloc (n_compartments * sizeof(int));
    
    output_type  = (char**) malloc (n_compartments * sizeof(char*));
    input_cont  = (int***)  malloc (n_compartments * sizeof(int**));
    input_disc  = (int***)  malloc (n_compartments * sizeof(int**));
    for (i=0;i<n_compartments;i++)
    {
        output_type[i] = (char*)  malloc (10 * sizeof(char));
        input_cont[i] = (int**)   malloc (10 * sizeof(int*));
        input_disc[i] = (int**)   malloc (10 * sizeof(int*));
        for (j=0;j<10;j++)
        {
            input_cont[i][j] = (int*)   malloc (5 * sizeof(int));
            input_disc[i][j] = (int*)   malloc (6 * sizeof(int));
        }
    }

    fscanf(input_file,"%d",&n_parameters_cont);
    parameters_cont = (double*)         malloc (n_parameters_cont * sizeof(double));
    for(i=0;i<n_parameters_cont;i++)    fscanf(input_file,"%lf",&parameters_cont[i]);

    fscanf(input_file,"%d",&n_parameters_disc);
    parameters_disc = (double*)         malloc (n_parameters_disc * sizeof(double));
    for(i=0;i<n_parameters_disc;i++)    fscanf(input_file,"%lf",&parameters_disc[i]);

    fscanf(input_file,"%d",&n_parameters_rate);
    parameters_rate = (double*)         malloc (n_parameters_rate * sizeof(double));
    for(i=0;i<n_parameters_rate;i++)    fscanf(input_file,"%lf",&parameters_rate[i]);

    for(i=0;i<n_compartments;i++)     fscanf(input_file,"%d",&ODE_inital[i]);

    for(i=0;i<n_compartments;i++)
    {
        for(j=0;j<10;j++)
        {
            for(k=0;k<5;k++) input_disc[i][j][k]=input_cont[i][j][k]=-1000;
            input_disc[i][j][5]=-1000;
        }
    }  

    for(i=0;i<n_compartments;i++)
    {
        fscanf(input_file,"%d",&n_output[i]);
        for(j=0;j<n_output[i];j++)
        {
            fscanf(input_file,"%c",&dummy);
            fscanf(input_file,"%c",&output_type[i][j]);
            if (output_type[i][j] == 'd')
            {
                for(k=0;k<6;k++)
                {
                    fscanf(input_file,"%d",&input_disc[i][j][k]);  
                }    
            }
            else if (output_type[i][j] == 'c')
            {
                for(k=0;k<5;k++)
                {
                    fscanf(input_file,"%d",&input_cont[i][j][k]); 
                }   
            }
        }
    }

    for(i=0;i<final_time;i++)   for(j=0;j<n_compartments;j++) ODE_FINAL[i][j]=0;

    strcpy(letter, argv[5]);
    reading();
    
    if(strcmp (letter, "N") == 0 )
    {
        n_characteristics=(2*n_parameters_cont+1);

        POP = (double**) malloc (n_individuals * sizeof(double*));
        for (i=0;i<n_individuals;i++)    POP[i]=(double*) malloc (n_characteristics * sizeof(double));

        for(i=0;i<n_sim;i++) MBI_without();
    }
    else
    {
        mu=(double)(1./atof(argv[6]));
        n_characteristics=(2*n_parameters_cont+3);

        POP = (double**) malloc (n_individuals * sizeof(double*));
        for (i=0;i<n_individuals;i++)    POP[i]=(double*) malloc (n_characteristics * sizeof(double));

        for(i=0;i<n_sim;i++) MBI_within();
    }
    FILE *arq_exit;
    arq_exit = fopen("color_graph.dat","w");
    for(i=0;i<n_individuals;i++)   fprintf(arq_exit,"%d %d\n",i,(int)POP[i][0]);
    fclose(arq_exit);

    print_MA();
    return 0;
}