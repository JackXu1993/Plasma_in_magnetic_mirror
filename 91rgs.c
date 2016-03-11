#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#define PI 3.1415926

/////////////////////////////integrate function////////////////////////////////
/////////////////////////////integrate function////////////////////////////////
double px,py,pz;

double lrgs(a,b,eps,f,xx,yy,zz)
double a,b,eps,(*f)(),xx,yy,zz;
{
    int m,i,j;
    double s,p,ep,h,aa,bb,w,xxxx,g;
    double t[5]={-0.9061798459,-0.5384693101,0.0,0.5384693101,0.9061798459};
    double c[5]={0.2369268851,0.4786286705,0.5688888889,0.4786286705,0.2369268851};
    m=1;
    px=xx;
    py=yy;
    pz=zz;
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
                xxxx=((bb-aa)*t[j]+(bb+aa))/2.0;
                w=w+(*f)(xxxx)*c[j];
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

void rkt1f(t,y,n,d)
int n;
double t, y[],d[];
{
    double a,b,eps,gx,gy,gz,bx(double),by(double),bz(double);
    t=t; n=n;
    a=0.0; b=2*PI; eps=0.000001;
    d[0]=-615479*(y[1]*lrgs(a,b,eps,bz,y[3],y[4],y[5])-y[2]*lrgs(a,b,eps,by,y[3],y[4],y[5]));
    d[1]=-615479*(y[2]*lrgs(a,b,eps,bx,y[3],y[4],y[5])-y[0]*lrgs(a,b,eps,bz,y[3],y[4],y[5]));
    d[2]=-615479*(y[0]*lrgs(a,b,eps,by,y[3],y[4],y[5])-y[1]*lrgs(a,b,eps,bx,y[3],y[4],y[5]));
    d[3]=y[0];
    d[4]=y[1];
    d[5]=y[2];
    return;
}
/////////////////////////////////////main function////////////////////////////////////
/////////////////////////////////////main function////////////////////////////////////
int main()
{
    int n=6,m=10,i,j,k;
    double a,b,eps,gx,gy,gz,bx(double),by(double),bz(double);
    a=0.0; b=2*PI; eps=0.000001;
    void rkt1f(double,double [],int, double []);
    double t,h,y[n],z[n][m+1];
/////////////////////initial values and step////////////////////////////
    y[0]=0.0; y[1]=1.0; y[2]=0.0;
    t=0.0; h=0.01;
/////////////////////////////create a date file/////////////////////////
    FILE* fp;
    fp = fopen("xyz.txt", "w");
    if (!fp)
    {
        perror("cannot open file");
        //exit(-1);
    }

////////////////////////////////calculation/////////////////////////////////
////////////////////////////////calculation/////////////////////////////////
    rkt1(t,y,n,h,m+1,z,rkt1f);
    printf("\n");
    for ( i = 0; i <=m ; i++)
    {
        t=i*h;
        //        printf("t=%5.2f\n", t);
        fprintf(fp,"%5.3e " ,t );
        for ( j = 0; j <= n-1 ; j++)
        {
            fprintf(fp,"%13.5e  " ,z[j][i]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);


printf("%13.5e\n", lrgs(a,b,eps,bz,-2.0,-2.0,-2.0));
}
