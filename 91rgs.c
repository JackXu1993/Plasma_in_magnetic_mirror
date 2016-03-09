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

void rkt1f(t,y,n,d)
int n;
double t, y[],d[];
{
    t=t; n=n;
    d[0]=-615479*(y[1]*lrgs(a,b,eps,bz)-y[2]*lrgs(a,b,eps,by));
    d[1]=-615479*(y[2]*lrgs(a,b,eps,bx)-y[0]*lrgs(a,b,eps,bz));
    d[2]=-615479*(y[0]*lrgs(a,b,eps,by)-y[1]*lrgs(a,b,eps,bx));
    d[3]=y[0];
    d[4]=y[1];
    d[5]=y[2];
    return;
}
/////////////////////////////////////main function////////////////////////////////////
/////////////////////////////////////main function////////////////////////////////////
int main()
{
    double i,j,k,n=6,m=20;
    int n=10;
    double a,b,eps,gx,gy,gz,bx(double),by(double),bz(double);
    a=0.0; b=2*3.1415926; eps=0.000001;
    void rkt1f(double,double [],int, double []);
    double t,h,y[n],z[n][m+1];
/////////////////////initial values and step////////////////////////////
    y[0]=0.0; y[1]=1.0; y[2]=0.0;
    t=0.0; h=0.01;
/////////////////////////////create a date file/////////////////////////
    FILE* fp;
    fp = fopen("magnetic_field.txt", "w");
    if (!fp)
    {
        perror("cannot open file");
        //exit(-1);
    }

////////////////////////////////calculation/////////////////////////////////
////////////////////////////////calculation/////////////////////////////////
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
    fclose(fp);

x=-2.0; y=-2.0; z=-2.0;
printf("%13.5e\n", lrgs(a,b,eps,bz));
}
