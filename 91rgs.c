#include "math.h"
#include "stdio.h"

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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double x,y,z;

double bx(phi)
double phi;
{
    double xx;
    xx=z*cos(phi)/pow((x*x+y*y+z*z+1-2*x*cos(phi)-2*y*sin(phi)),1.5);
    return (xx);
}

double by(phi)
double phi;
{
    double yy;
    yy=z*sin(phi)/pow((x*x+y*y+z*z+1-2*x*cos(phi)-2*y*sin(phi)),1.5);
    return (yy);
}

double bz(phi)
double phi;
{
    double zz;
    zz=(1-x*cos(phi)-y*sin(phi))/pow((x*x+y*y+z*z+1-2*x*cos(phi)-2*y*sin(phi)),1.5);
    return (zz);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int main()
{
    int i,j,k;
    int n=10;
    double a,b,eps,gx,gy,gz,bx(double),by(double),bz(double);
    a=0.0; b=2*3.1415926; eps=0.000001;
    FILE* fp;

    /*打开文本文件以写入内容*/
    fp = fopen("magnetic_field.txt", "w");
    if (!fp)
    {
        perror("cannot open file");
        //exit(-1);
    }


    for ( i = 0; i < n; i++)
    {
        for ( j = 0; j < n; j++)
        {
            for ( k = 0; k < n; k++)
            {
                    x=i; y=j; z=k;
                    gx=lrgs(a,b,eps,bx);
                    gy=lrgs(a,b,eps,by);
                    gz=lrgs(a,b,eps,bz);
                    printf("\n" );
                    fprintf(fp,"gx(%f,%f,%f)=%13.5e\n", x,y,z,gx);
                    fprintf(fp,"gy(%f,%f,%f)=%13.5e\n", x,y,z,gy);
                    fprintf(fp,"gz(%f,%f,%f)=%13.5e\n", x,y,z,gz);
                    fputc("\n" );
            }
        }
    }

}
