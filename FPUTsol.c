/*Sergio Gutierrez 201326669
/*Jose Acosta 201212570 
/*Universidad de Los Andes
/*Marzo 9 2017
*/ 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "omp.h"
#include <string.h>
#define PI 3.14159265358979323846


/* varaibles inciales dadas */
int N=64,B=1.0;

/*Defino funicion que se van a usar*/
void init(double *array);
void force(double *array, double *f);
void leapfrog(double *x_n, double *v_n,double *v_half,double *f, int n,double t);
double Qk(double *x_n,int N, int k);
double Ek(double *x_n,double *v_n,double Q,double Qp,double wk,int N, int k);
double wk(int N,int k);

/* Funcion que me defini mis condiciones iniciales */
void init (double *array)
{ 
	int i;
	
	for(i=0 ; i<N; i++)
	{
		array[i]=sin((PI*i)/(N-1));
	}
	
}

/*Fucion que calcula el desplazamiento (aceleracion) aplicado a cada nodo*/ 
void force(double *array, double *f)
{	int i;
	/*calculo el despasamiento para cada punto del vector de entrada*/ 
	
	for (i  = 1 ; i<N-1 ; i++)
	{
	f[i] = array[i+1] -2*array[i]+ array[i-1]+B*(pow(array[i+1]-array[i],2)-pow(array[i]-array[i-1],2));
	}

}
/*Fucion que me da un paso en el timepo t me actualisa las vriables de posicion velocidad */
void leapfrog(double *x_n, double *v_n,double *v_half,double *f,int n,double t)
{ 
	int i;
	/*actualiso aceleracion calculi velocidad nueva y posicion*/
	force(x_n,f);
	#pragma omp parallel for shared(v_half,x_n,v_n)
	for(i=1 ; i<n-1; i++)
	{
		v_half[i] = v_n[i] + 0.5*t*f[i];
		x_n[i] =x_n[i]+t*v_half[i];
	}
	/*con nueva posicion  actual calculo la velocidad en el punto*/
	force(x_n,f);
	#pragma omp parallel for shared(v_half,x_n,v_n)
	for(i=1 ; i<n-1; i++)
	{
		v_n[i] = v_half[i] + 0.5*t*f[i];
	}
}

/* Fucion que me da la relacion de los Q que se encuentran en la guia*/
double Qk(double *x_n,int N, int k)
{	
	double Q;
	int i;
	
	for(i=0;i<N;i++)
	{
		Q+=x_n[i]*sin((PI*k*i)/(N));	
	}
	return pow(2,.5)*pow(N+1,-.5)*Q ;
}
/* Con los Q puedo hace una funcion que me calcule la energia para un instante en el tiempo*/
double Ek(double *x_n,double *v_n,double Q,double Qp,double wk,int N, int k)
{	
	Q=Qk(x_n,N,k);
	Qp=Qk(v_n,N,k);
	return .5*(pow(Q,2)+wk*pow(Qp,2));
}
/* calculo las frecuancias seguna la teoria para cada momento*/
double wk(int N,int k)
{
	double wk;
	wk=2*sin((k*PI)/(2*N+2));
	return wk;
}

int main(int argc , char** argv)
{
	/* Defino todas las variable que nesecito para resolver el problema*/
	
	omp_set_num_threads(atoi(argv[1]));
	double E,Q,Qp,w;
	int j,i,c,proc;
	double *x_n= malloc((N)*sizeof(double));
	double *v_n= malloc((N)*sizeof(double));
	double *v_half=malloc((N)*sizeof(double));
	double *f=malloc((N)*sizeof(double));	
	double t= 5E-3 , T=5*pow(N,2.2);
	clock_t start = clock();
	/* inicialiso las posiciones*/
	init(x_n);
	
	/*Reccorro todos los intervalos temporales actualisando posicion y velocidad punto a punto*/
	for(i=0; i<T/t;i++)
	{
		leapfrog(x_n,v_n,v_half,f,N,t);
		/*Genero un contador que me deje imprimir las energias cada 1000 paso equidistantes*/
		if(c>T/(t*pow(10,3)))
		{
			/*si estoy en un paso 1000 mas adelante imprimo las energias para los modos 1,2,3*/
			for(j=1;j<=3;j++)
			{
				w=pow(wk(N,j),2);			
				E=Ek(x_n,v_n,Q,Qp,w,N,j);	
				printf("%f  ", E);
	
			}
			printf("\n");
			c=0;
		}	
	c++;
	}
clock_t finish = clock();
clock_t totaltime=(double)(finish - start) / CLOCKS_PER_SEC;


char nombre[15]="tiempo";
char numproc[2];
sprintf(numproc, "%d", atoi(argv[1]));
strcat(nombre,numproc);
strcat(nombre,".dat");

FILE *ePtr;
if((ePtr=fopen(nombre,"w"))==NULL)
	puts("No se pudo crear el archivo");
else
{		
 fprintf(ePtr,"%ld",totaltime);
		
}

return 0;
}


