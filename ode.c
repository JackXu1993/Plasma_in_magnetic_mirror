#include "stdlib.h"
#include "stdio.h"

void rkt1(t,y,n,h,k,z,f)
void (*f) ();
int n, k;
double t,h,y[],z[];
{
    int i,j,l;
    double a[4], tt, *b, *d ;
    b=malloc(n*sizeof(double));
    d=malloc(n*sizeof(double));
    a[0]=h/2.0; a[1]=a[0];
    a[2]=h; a[3]=h;
    for ( i = 0; i <= n-1 ; i++)  z[i*k]=y[i];
    for ( l = 1; l <= k-1 ; l++)
    {
        (*f)(t,y,n,d);
        for ( i = 0; i <= n-1 ; i++)  b[i]=y[i];
        for ( j = 0; j <= 2; j++)
        {
            for ( i = 0; i <= n-1 ; i++)
            {
                y[i]=z[i*k+l-1]+a[j]*d[i];
                b[i]=b[i]+a[j+1]*d[i]/3.0;
            }
            tt=t+a[j];
            (*f)(tt,y,n,d);
        }
            for ( i = 0; i <= n-1 ; i++)
                y[i]=b[i]+h*d[i]/6.0;
            for ( i = 0; i <= n-1 ; i++)
                z[i*k+l]=y[i];
            t=t+h;
        }
    free(b); free(d);
    return;
}

void rkt1f(t,y,n,d)
int n;
double t, y[],d[];
{
    t=t; n=n;
    d[0]=-3*(y[0]-y[1]);
    d[1]=-y[0]*y[2]+26.5*y[0]-y[1];
    d[2]=y[0]*y[1]-y[2];
    return;
}

int main()
{
    int i,j,n=3,m=600;
    void rkt1f(double,double [],int, double []);
    double t,h,y[n],z[n][m+1];
    y[0]=0.0; y[1]=1.0; y[2]=0.0;
    t=0.0; h=0.01;
    FILE* fp;

    fp = fopen("d.dat", "w");
    if (!fp)
    {
        perror("cannot open file");
        //exit(-1);
    }

    rkt1(t,y,n,h,m+1,z,rkt1f);
    printf("\n");
    for ( i = 0; i <=m ; i++)
    {
        t=i*h;
//        printf("t=%5.2f\n", t);
        for ( j = 0; j <= n-1 ; j++)
        {
            fprintf(fp,"%13.5e %13.5e  " ,t ,z[j][i]);
        }
        fprintf(fp,"\n");
    }
}
