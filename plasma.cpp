#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <cstdio>
#define PI 3.1415926
#define A 0.0
#define B 2*PI
#define EPS 0.000001
#define n 6
#define m 6000
using namespace std;

/////////////////////////////////definition of functions////////////////////////////////
/////////////////////////////////definition of functions////////////////////////////////
double px,py,pz;
double y[n];
double d[n];
typedef double (*PT) ( double );
typedef void (*PF) ( double ,int , double[] );
double bx(double);
double by(double);
double bz(double);
void ode(double t ,int n , double d[] );
void sode(double t ,int n ,double h ,int k ,double z[] , PF f);
double integrate( double a ,double b ,double eps ,PT f ,double xx ,double yy ,double zz );

/////////////////////////////////Integration function////////////////////////////////
/////////////////////////////////Integration function////////////////////////////////
double integrate( double a ,double b ,double eps ,PT f ,double xx ,double yy ,double zz )
{
    int mm,i,j;
    double s,p,ep,h,aa,bb,w,xxxx,g;
    double t[5]={-0.9061798459,-0.5384693101,0.0,0.5384693101,0.9061798459};
    double c[5]={0.2369268851,0.4786286705,0.5688888889,0.4786286705,0.2369268851};
    mm=1;
    px=xx;
    py=yy;
    pz=zz;
    h=b-a; s=fabs(0.001*h);
    p=1.0e+35;ep=eps+1.0;
    while ((ep>=eps)&&(fabs(h)>s))
    {
        g=0.0;
        for (i=1;i<=mm;i++)
        {
            aa=a+(i-1.0)*h; bb=a+i*h;
            w=0.0;
            for (j=0;j<=4;j++)
            {
                xxxx=((bb-aa)*t[j]+(bb+aa))/2.0;
                w=w+f(xxxx)*c[j];
            }
            g=g+w;
        }
        g=g*h/2.0;
        ep=fabs(g-p)/(1.0+fabs(g));
        p=g; mm=mm+1; h=(b-a)/mm;
    }
    return (g);
}
/////////////////////////////////solve ode function//////////////////////////////////
/////////////////////////////////solve ode function//////////////////////////////////
void sode(double t ,int n ,double h ,int k ,double z[] ,PF f)
{
    int i,j,l;
    double a[4], tt, *b, *d ;
    b=(double*)malloc(n*sizeof(double));
    d=(double*)malloc(n*sizeof(double));
    a[0]=h/2.0; a[1]=a[0]; a[2]=h; a[3]=h;
    for ( i = 0; i <= n-1 ; i++)
    {
        z[i*k]=y[i];
    }
    for ( l = 1; l <= k-1 ; l++)
    {
        f(t,n,d);
        for ( i = 0; i <= n-1 ; i++)
        {
            b[i]=y[i];
        }
        for ( j = 0; j <= 2; j++)
        {
            for ( i = 0; i <= n-1 ; i++)
            {
                y[i]=z[i*k+l-1]+a[j]*d[i];
                b[i]=b[i]+a[j+1]*d[i]/3.0;
            }
            tt=t+a[j];
            f(tt,n,d);
        }
            for ( i = 0; i <= n-1 ; i++)
            {
                y[i]=b[i]+h*d[i]/6.0;
            }
            for ( i = 0; i <= n-1 ; i++)
            {
                z[i*k+l]=y[i];
            }
            t=t+h;
        }
    free(b); free(d);
    return;
}
/////////////////////////fuction of magnetic field/////////////////////////////
/////////////////////////fuction of magnetic field/////////////////////////////

double bx(double xxx)
{
    double xx;
    xx=(pz-1)*cos(xxx)/pow((px*px+py*py+(pz-1)*(pz-1)+1-2*px*cos(xxx)-2*py*sin(xxx)),1.5)+(pz+1)*cos(xxx)/pow((px*px+py*py+(pz+1)*(pz+1)+1-2*px*cos(xxx)-2*py*sin(xxx)),1.5);
    return (xx);
}

double by(double xxx)
{
    double yy;
    yy=(pz-1)*sin(xxx)/pow((px*px+py*py+(pz-1)*(pz-1)+1-2*px*cos(xxx)-2*py*sin(xxx)),1.5)+(pz+1)*sin(xxx)/pow((px*px+py*py+(pz+1)*(pz+1)+1-2*px*cos(xxx)-2*py*sin(xxx)),1.5);
    return (yy);
}

double bz(double xxx)
{
    double zz;
    zz=(1-px*cos(xxx)-py*sin(xxx))*pow((px*px+py*py+(pz-1)*(pz-1)+1-2*px*cos(xxx)-2*py*sin(xxx)),-1.5)+(1-px*cos(xxx)-py*sin(xxx))*pow((px*px+py*py+(pz+1)*(pz+1)+1-2*px*cos(xxx)-2*py*sin(xxx)),-1.5);
    return (zz);
}

/////////////////////////////////ode function//////////////////////////////////
/////////////////////////////////ode function//////////////////////////////////
void ode(double t ,int n ,double d[] )
{
    PT bxx,byy,bzz;
    t=t; n=n;
    bxx=bx;
    byy=by;
    bzz=bz;
    d[0]=-615479*(y[1]*integrate(A,B,EPS,bzz,y[3],y[4],y[5])-y[2]*integrate(A,B,EPS,byy,y[3],y[4],y[5]));
    d[1]=-615479*(y[2]*integrate(A,B,EPS,bxx,y[3],y[4],y[5])-y[0]*integrate(A,B,EPS,bzz,y[3],y[4],y[5]));
    d[2]=-615479*(y[0]*integrate(A,B,EPS,byy,y[3],y[4],y[5])-y[1]*integrate(A,B,EPS,bxx,y[3],y[4],y[5]));
    d[3]=y[0];
    d[4]=y[1];
    d[5]=y[2];
}
/////////////////////////////////////main function////////////////////////////////////
/////////////////////////////////////main function////////////////////////////////////
int main()
{
    int i,j,k;
    double t,h;
    double z[n][m+1];
    PF rr;
    rr=ode;
/////////////////////initial values and step////////////////////////////
    y[0]=0.0; y[1]=0.3e6; y[2]=0.15e6; y[3]=0.0;y[4]=0.0;y[5]=0.0;
    t=0.0; h=6e-5/m;
/////////////////////////////create a date file/////////////////////////
    FILE* fp;
    fp = fopen("xyz++.dat", "w");
    if (!fp)
    {
        perror("cannot open file");
        //exit(-1);
    }

////////////////////////////////calculation/////////////////////////////////
////////////////////////////////calculation/////////////////////////////////
    sode(t,n,h,m+1,z,rr);
    for ( i = 0; i <= m ; i++)
    {
        t=i*h;
        //        printf("t=%5.2f\n", t);
        fprintf(fp,"%13.5e " ,t );
        for ( j = 0; j <= n-1 ; j++)
        {
            fprintf(fp,"%13.5e  " ,z[j][i]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);


//printf("%13.5e\n", integrate(a,b,eps,bz,-1.0,-1.0,-1.0));
}
