#include "math.h"
#include "stdio.h"
#include "stdlib.h"

/////////////////////////////integrate function////////////////////////////////
/////////////////////////////integrate function////////////////////////////////
double lrgs(a,b,eps,f)
double a,b,eps,(*f)();
{
    int m,i,j;
    double s,p,ep,h,aa,bb,w,x,g;
    double t[5]={-0.9061798459,-0.5384693101,0.0,0.5384693101,0.9061798459};
    double c[5]={0.2369268851,0.4786286705,0.5688888889,0.4786286705,0.2369268851};
    m=1;
    h=b-a; s=fabs(0.001*h);
    p=1.0e+35;ep=eps+1.0;
    while ((ep>=eps)&&(fabs(h)>s))
    {
        g=0.0;
        for (i=1;i<=m;i++)
        {
            aa=a+(i-1.0)*h; bb=a+i*h;
            w=0.0;
            for (j=0;j<=4;j++)
            {
                x=((bb-aa)*t[j]+(bb+aa))/2.0;
                w=w+(*f)(x)*c[j];
            }
            g=g+w;
        }
        g=g*h/2.0;
        ep=fabs(g-p)/(1.0+fabs(g));
        p=g; m=m+1; h=(b-a)/m;
    }
    return (g);
}
/////////////////////////////////ode function//////////////////////////////////
/////////////////////////////////ode function//////////////////////////////////
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
/////////////////////////fuction of magnetic field/////////////////////////////
/////////////////////////fuction of magnetic field/////////////////////////////
double x,y,z;

double bx(double xxx)
{
    double xx;
    xx=(z-1)*cos(xxx)/pow((x*x+y*y+(z-1)*(z-1)+1-2*x*cos(xxx)-2*y*sin(xxx)),1.5)+(z+1)*cos(xxx)/pow((x*x+y*y+(z+1)*(z+1)+1-2*x*cos(xxx)-2*y*sin(xxx)),1.5);
    return (xx);
}

double by(double xxx)
{
    double yy;
    yy=(z-1)*sin(xxx)/pow((x*x+y*y+(z-1)*(z-1)+1-2*x*cos(xxx)-2*y*sin(xxx)),1.5)+(z+1)*sin(xxx)/pow((x*x+y*y+(z+1)*(z+1)+1-2*x*cos(xxx)-2*y*sin(xxx)),1.5);
    return (yy);
}

double bz(double xxx)
{
    double zz;
    zz=(1-x*cos(xxx)-y*sin(xxx))*pow((x*x+y*y+(z-1)*(z-1)+1-2*x*cos(xxx)-2*y*sin(xxx)),-1.5)+(1-x*cos(xxx)-y*sin(xxx))*pow((x*x+y*y+(z+1)*(z+1)+1-2*x*cos(xxx)-2*y*sin(xxx)),-1.5);
    return (zz);
}

double test(double xxx)
{
    double tt;
    tt=(1-2*cos(xxx)-2*sin(xxx))*pow((4+4+4+1-4*cos(xxx)-4*sin(xxx)),-1.5);
    return (tt);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int main()
{
    double i,j,k;
    int n=50;
    double a,b,eps,gx,gy,gz,bx(double),by(double),bz(double);
    a=0.0; b=2*3.1415926; eps=0.000001;
    FILE* fp;

    fp = fopen("magnetic_field.txt", "w");
    if (!fp)
    {
        perror("cannot open file");
        //exit(-1);
    }


    for ( i = -n/2; i <=n/2; i++)
    {
        for ( j = -n/2; j <=n/2; j++)
        {
            for ( k = -n/2; k <=n/2; k++)
            {
                    x=2.0*i/n; y=2.0*j/n; z=4.0*k/n;
                    gx=lrgs(a,b,eps,bx);
                    gy=lrgs(a,b,eps,by);
                    gz=lrgs(a,b,eps,bz);
                    fprintf(fp,"gx(%4.3f,%4.3f,%4.3f)=%13.5e  ", x,y,z,gx);
                    fprintf(fp,"gy(%4.3f,%4.3f,%4.3f)=%13.5e  ", x,y,z,gy);
                    fprintf(fp,"gz(%4.3f,%4.3f,%4.3f)=%13.5e\n", x,y,z,gz);
            }
        }
    }
    fclose(fp);

x=-2.0; y=-2.0; z=-2.0;
printf("%13.5e\n", lrgs(a,b,eps,bz));
printf("%13.5e\n", lrgs(a,b,eps,test));
}
