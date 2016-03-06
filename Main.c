#include <stdio.h>
<<<<<<< HEAD
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "myThread.h"


void trabajo(int cantidadTrabajo) {
=======
#include <math.h>
>>>>>>> 02e5a5dab7ed735d30039b8882d99d11a86982a8

/**
* Aproxima arcsen(x) mediante Serie de Taylor
*  @arg x Variable dependiente de la función
*  @arg n Cantidad de términos de la serie
*/
double arcsin(double x, int n)
{
    double fact,sum;
    fact=sum=1.0;
    int i;
    for(i=1;i<=n;i++)
    {
	fact *= (2 * (double)i - 1)/(2 * (double)i );
	double po = pow(x,(2*(double)i+1));
	double factwo = po / (2*(double)i+1);
	sum += fact * (pow(x,(2*(double)i+1)) / (2*(double)i+1));
    }
    return sum;
}

<<<<<<< HEAD

int main (int argc, char*argv[]){


	return 0;
	
=======
int main(void)
{
    int n;
    printf("Cantidad de unidades de trabajo: ");
    scanf("%d",&n);
    printf("2 arcsin(1) = %lf\n",2*arcsin(1.0,n*50));
>>>>>>> 02e5a5dab7ed735d30039b8882d99d11a86982a8
}
