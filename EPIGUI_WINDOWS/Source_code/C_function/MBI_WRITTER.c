#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <time.h>
#include <string.h>

int n_compartments,n_parameters;
int aux1=0,aux2=0,aux3=0,pos=0,aux_name1,aux_name2,aux_name3, sum=0;

int aux_changed1,aux_final1, aux_final2, stopped;
double dt,aux_changed2;

double *values_parameters;   
int *aux_changed_pos,*ODE_inital;   

int *n_outputs;
char **name_compartment, **name_parameter, ***equation;
char name_par_aux1[10][10],name_par_aux2[10][10],name_par_aux3[10][10];

void continuous_variables()
{
    int i;
    if(sum==1)
    {
        strcpy(name_par_aux1[aux1],name_parameter[pos]);
        aux_name1=0;
        if (aux1==0)
        {
            aux1++;
            aux_name1=1;
        }
        else    for(i=0;i<aux1;i++) if (strcmp (name_parameter[pos], name_par_aux1[i]) == 0)    aux_name1++;
        if (aux_name1==0)   aux1++;
        memset(name_par_aux1[aux1],0,10);
    }
}

void discrete_variables()
{
    int i;
    if(sum==2)
    {
        strcpy(name_par_aux2[aux2],name_parameter[pos]);
        aux_name2=0;
        if (aux2==0)
        {
            aux2++;
            aux_name2=1;
        }
        else    for(i=0;i<aux2;i++) if (strcmp (name_parameter[pos], name_par_aux2[i]) == 0)    aux_name2++;
        if (aux_name2==0)   aux2++;

        memset(name_par_aux2[aux2],0,10);
    }
}

int main()
{

    FILE *entrada,*arq_exit;
    entrada = fopen("IBM.dat","r");
    arq_exit = fopen("INPUT_IBM.dat","w");
    int i,j,k,l,m;

    fscanf(entrada,"%lf",&dt); 

    fscanf(entrada,"%d",&n_compartments);                                         //
    fprintf(arq_exit,"%d\n",n_compartments);                                         //
    name_compartment = (char**)  malloc (n_compartments * sizeof(char*));         //
    for(i=0;i<n_compartments;i++)                                                 //  NOME DOS COMPARTIMENTOS
    {                                                                               //
        name_compartment[i] = (char*)  malloc (10 * sizeof(char));                  //
        fscanf(entrada,"%s",name_compartment[i]);                                   //
    }                                                                               //

    fscanf(entrada,"%d",&n_parameters);                                             //
    name_parameter = (char**)  malloc (n_parameters * sizeof(char*));               //
    for(i=0;i<n_parameters;i++)                                                     //
    {                                                                               //  NOME DOS PARAMETROS
        name_parameter[i] = (char*)  malloc (10 * sizeof(char));                    //
        fscanf(entrada,"%s",name_parameter[i]);                                     //
    }                                                                               //

    int strings[n_compartments];
    n_outputs         = (int*)    malloc (n_compartments * sizeof(int));           //
    equation         = (char***) malloc (n_compartments * sizeof(char**));        //
    for(i=0;i<n_compartments;i++)                                                 //  
    {                                                                               //
        strings[i] = 0;                                                             //
        n_outputs[i] = 0;                                                            //         
        equation[i]  = (char**)  malloc (50 * sizeof(char*));                       //      
        for(j=0;j<50;j++) equation[i][j] = (char*)  malloc (10 * sizeof(char));     //   NUMERO DE SAIDAS
        for(j=0;j<50;j++)                                                           //          E
        {                                                                           //       EQUACAO
            strings[i]++;                                                           //        
            fscanf(entrada,"%s",equation[i][j]);                                    //
            if (strcmp (equation[i][j], ";") == 0)  break;                          //
        }                                                                           //
    }                                                                               //

    for(i=0;i<10;i++)
    {
        memset(name_par_aux1[i],0,10);
        memset(name_par_aux2[i],0,10);
        memset(name_par_aux3[i],0,10);
    }

    for(i=0;i<n_compartments;i++)
    {
        for(j=0;j<strings[i];j++)
        {
            if (strcmp (equation[i][j], "-") == 0 || strcmp (equation[i][j], "+") == 0 || strcmp (equation[i][j], ";") == 0 )
            {
                if (strcmp (equation[i][j], "-") == 0)
                {
                    if (j==0) n_outputs[i]++;
                    else
                    {
                        if(strcmp (equation[i][j-1], "(1") != 0 && strcmp (equation[i][j-1], "1") != 0 ) n_outputs[i]++;
                        else
                        {
                            strcpy(name_par_aux3[aux3],equation[i][j+1]);
                            aux_name3=0;
                            if (aux3==0)
                            {
                                aux3++;
                                aux_name3=1;
                            }
                            else    for(k=0;k<aux3;k++) if (strcmp (equation[i][j+1], name_par_aux3[k]) == 0)    aux_name3++;
                            if (aux_name3==0)   aux3++;

                            memset(name_par_aux3[aux3],0,10);
                        }
                    }
                }  
                continuous_variables();
                discrete_variables();
                sum=0;
            }
            else
            {
                for(k=0;k<n_parameters;k++)
                {
                    if(strcmp (equation[i][j],name_parameter[k]) == 0)
                    {
                    pos=k;
                    break;
                    }
                }

                for(k=0;k<n_compartments;k++)
                {
                    if(strcmp (equation[i][j],name_compartment[k]) == 0)
                    {
                    sum++;
                    break;
                    }
                }
            }
        }
    }
    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
    values_parameters = (double*)  malloc (n_parameters * sizeof(double));
    aux_changed_pos = (int*)  malloc (n_parameters * sizeof(int));
    ODE_inital = (int*)  malloc (n_compartments * sizeof(int));

    for(i=0;i<n_parameters;i++)     fscanf(entrada,"%lf",&values_parameters[i]);     //

    for(i=0;i<n_parameters;i++) aux_changed_pos[i]=i;

    for(i=0;i<aux1;i++)
    {
        for(j=0;j<n_parameters;j++)
        if(strcmp (name_par_aux1[i],name_parameter[j]) == 0)
        {
            aux_changed1 = aux_changed_pos[i];
            aux_changed_pos[i] = aux_changed_pos[j];
            aux_changed_pos[j] = aux_changed1;

            aux_changed2 = values_parameters[i];
            values_parameters[i] = values_parameters[j];
            values_parameters[j] = aux_changed2;
        }
    }

    for(i=0;i<aux2;i++)
    {
        for(j=0;j<n_parameters;j++)
        if(strcmp (name_par_aux2[i],name_parameter[j]) == 0)
        {
            for(k=0;k<n_parameters;k++)
            {
                if(j==aux_changed_pos[k])
                {
                    aux_changed1 = aux_changed_pos[aux1+i];
                    aux_changed_pos[aux1+i] = aux_changed_pos[k];
                    aux_changed_pos[k] = aux_changed1;

                    aux_changed2 = values_parameters[aux1+i];
                    values_parameters[aux1+i] = values_parameters[k];
                    values_parameters[k] = aux_changed2;
                    break;
                }
            }
        }
    }

    for(i=0;i<aux3;i++)
    {
        for(j=0;j<n_parameters;j++)
        if(strcmp (name_par_aux3[i],name_parameter[j]) == 0)
        {
           for(k=0;k<n_parameters;k++)
            {
                if(j==aux_changed_pos[k])
                {
                    aux_changed1 = aux_changed_pos[aux1+aux2+i];
                    aux_changed_pos[aux1+aux2+i] = aux_changed_pos[k];
                    aux_changed_pos[k] = aux_changed1;

                    aux_changed2 = values_parameters[aux1+aux2+i];
                    values_parameters[aux1+aux2+i] = values_parameters[k];
                    values_parameters[k] = aux_changed2;
                    break;
                }
            }
        }
    }

    double parameters_cont[aux1],parameters_disc[aux2],parameters_rate[aux3];

    fprintf(arq_exit,"%d\n",aux1);
    for(i=0;i<aux1;i++)
    {
        parameters_cont[i]=values_parameters[i];
        fprintf(arq_exit,"%lf\t",1./parameters_cont[i]);
    }

    fprintf(arq_exit,"\n%d\n",aux2);
    for(i=0;i<aux2;i++)
    {
        parameters_disc[i]=values_parameters[i+aux1];
        fprintf(arq_exit,"%lf\t",dt*parameters_disc[i]);
    }
    
    fprintf(arq_exit,"\n%d\n",aux3);
    for(i=0;i<aux3;i++)
    {
        parameters_rate[i]=values_parameters[i+aux1+aux2];
        fprintf(arq_exit,"%lf\t",parameters_rate[i]);
    }
    
    fprintf(arq_exit,"\n");
    for(i=0;i<n_compartments;i++)
    {
        fscanf(entrada,"%d",&ODE_inital[i]);
        fprintf(arq_exit,"%d\t",ODE_inital[i]);
    }
    fprintf(arq_exit,"\n");

    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------

    int input_disc[n_compartments][10][6],input_cont[n_compartments][10][5];

    for(i=0;i<n_compartments;i++)
    {
        for(j=0;j<10;j++)
        {
            for(k=0;k<5;k++)
            {
                input_disc[i][j][k]=-1000;
                input_cont[i][j][k]=-1000;
            }
            input_disc[i][j][5]=-1000;
        }
    }

    for(i=0;i<n_compartments;i++)
    {
        sum=0;
        fprintf(arq_exit,"%d\t",n_outputs[i]);
        while(sum<n_outputs[i])
        {
            for(j=0;j<50;j++)
            {
                aux_final1 = aux_final2 = 0;
                if (strcmp (equation[i][j], "-") == 0)
                {
                    for(k=0;k<aux1;k++)
                    {
                        if (strcmp (equation[i][j+1],name_par_aux1[k]) == 0)
                        {
                            input_cont[i][sum][3]=k;                                  //parametro cont
                            aux_final1 = 1;
                        }
                    }

                    for(k=0;k<aux2;k++)
                    {
                        if (strcmp (equation[i][j+1],name_par_aux2[k]) == 0)
                        {
                            input_disc[i][sum][4]=k;                                              //parametro disc
                            aux_final2 = 1;
                            for(l=2;l<=3;l++)
                            {
                                stopped=0;
                                for(m=0;m<n_compartments;m++)
                                {
                                    if (strcmp (equation[i][j+l],name_compartment[m]) == 0 && i!=m)
                                    {
                                        stopped = 1;
                                        input_disc[i][sum][0]=m;                                  //contato
                                        break;
                                    }
                                }
                                if(stopped==1)   break;
                            }
                        }
                    }

                    stopped=0;
                    for(k=0;k<n_compartments;k++)
                    {
                        if(i!=k)
                        {
                            for(l=0;l<50;l++)
                            {
                                if (strcmp (equation[k][l], equation[i][j+1]) == 0 && strcmp (equation[k][l+1], equation[i][j+2]) == 0 && aux_final1 == 1)
                                {
                                    input_cont[i][sum][0]=i;
                                    if(l==0)
                                    {
                                        input_cont[i][sum][1]=k; 
                                        input_cont[i][sum][2]=1000; 
                                        input_cont[i][sum][4]=1000;
                                        fprintf(arq_exit,"c\t");
                                        for(m=0;m<4;m++)    fprintf(arq_exit,"%d\t",input_cont[i][sum][m]);
                                        fprintf(arq_exit,"%d\n",input_cont[i][sum][4]);
                                        sum++;
                                        break;
                                    }
                                    else
                                    {
                                        if (strcmp (equation[k][l-1], "+") == 0)                                //destino unico
                                        {
                                            input_cont[i][sum][1]=k; 
                                            input_cont[i][sum][2]=1000; 
                                            input_cont[i][sum][4]=1000;
                                            fprintf(arq_exit,"c\t");
                                            for(m=0;m<4;m++)    fprintf(arq_exit,"%d\t",input_cont[i][sum][m]);
                                            fprintf(arq_exit,"%d\n",input_cont[i][sum][4]);
                                            sum++;
                                            break;
                                        }    
                                        else
                                        {
                                            if(l==1)
                                            {
                                                for(m=0;m<aux3;m++)
                                                {
                                                    if(strcmp (equation[k][l-1], name_par_aux3[m]) == 0 )
                                                    {
                                                        input_cont[i][sum][1]=k;
                                                        input_cont[i][sum][4]=m; 
                                                        stopped++;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                for(m=0;m<aux3;m++)
                                                {
                                                    if(strcmp (equation[k][l-1], name_par_aux3[m]) == 0 )
                                                    {
                                                        input_cont[i][sum][1]=k;
                                                        input_cont[i][sum][4]=m; 
                                                        stopped++;
                                                    }
                                                    else if (strcmp (equation[k][l-2], name_par_aux3[m]) == 0 )
                                                    {
                                                        input_cont[i][sum][2]=k;
                                                        input_cont[i][sum][4]=m; 
                                                        stopped++;
                                                    }
                                                }
                                            }
                                            if(stopped == 2)
                                            {
                                                fprintf(arq_exit,"c\t");
                                                for(m=0;m<4;m++)    fprintf(arq_exit,"%d\t",input_cont[i][sum][m]);
                                                fprintf(arq_exit,"%d\n",input_cont[i][sum][4]);
                                                sum++;
                                                break;
                                            }
                                        }
                                    }
                                }
                                
                                if (strcmp (equation[k][l], equation[i][j+1]) == 0 && strcmp (equation[k][l+1], equation[i][j+2]) == 0 && strcmp (equation[k][l+2], equation[i][j+3]) == 0 && aux_final2 == 1)
                                {
                                    input_disc[i][sum][1]=i;                                              //origem
                                    if (l==0)
                                    {
                                        input_disc[i][sum][2]=k;       //destino unico
                                        input_disc[i][sum][3]=1000;    //destino unico
                                        input_disc[i][sum][5]=1000;    //destino unico
                                        fprintf(arq_exit,"d\t");
                                        for(m=0;m<5;m++)    fprintf(arq_exit,"%d\t",input_disc[i][sum][m]);
                                        fprintf(arq_exit,"%d\n",input_disc[i][sum][5]);
                                        sum++;
                                        break;
                                    }
                                    else
                                    {
                                        if (strcmp (equation[k][l-1], "+") == 0)
                                        {
                                            input_disc[i][sum][2]=k;  //destino unico
                                            input_disc[i][sum][3]=1000;  //destino unico
                                            input_disc[i][sum][5]=1000;  //destino unico
                                            fprintf(arq_exit,"d\t");
                                            for(m=0;m<5;m++)    fprintf(arq_exit,"%d\t",input_disc[i][sum][m]);
                                            fprintf(arq_exit,"%d\n",input_disc[i][sum][5]);
                                            sum++;
                                            break;
                                        }    
                                        else
                                        {
                                            if(l==1)
                                            {
                                                for(m=0;m<aux3;m++)
                                                {
                                                    if(strcmp (equation[k][l-1], name_par_aux3[m]) == 0 )
                                                    {
                                                        input_disc[i][sum][2]=k;
                                                        input_disc[i][sum][5]=m;
                                                        stopped++;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                for(m=0;m<aux3;m++)
                                                {
                                                    if(strcmp (equation[k][l-1], name_par_aux3[m]) == 0 )
                                                    {
                                                        input_disc[i][sum][2]=k;
                                                        input_disc[i][sum][5]=m;
                                                        stopped++;
                                                    }
                                                    else if (strcmp (equation[k][l-2], name_par_aux3[m]) == 0 )
                                                    {
                                                        input_disc[i][sum][3]=k; 
                                                        input_disc[i][sum][5]=m;
                                                        stopped++;
                                                    }
                                                }
                                            }
                                            if(stopped == 2)
                                            {
                                                fprintf(arq_exit,"d\t");
                                                for(m=0;m<5;m++)    fprintf(arq_exit,"%d\t",input_disc[i][sum][m]);
                                                fprintf(arq_exit,"%d\n",input_disc[i][sum][5]);
                                                sum++;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}