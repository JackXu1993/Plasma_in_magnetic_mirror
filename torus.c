#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//#include <GLFW/glfw3.h>
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#define PI 3.1415926
#define steps 600

/////////////////////////////integrate function////////////////////////////////
/////////////////////////////integrate function////////////////////////////////
GLdouble xRotated=0.0, yRotated=90.0, zRotated=0.0;
GLdouble xx=0.0, yy=0.0, zz=-3.0 ;
GLdouble lx, ly, lz;
GLdouble innerRaidus=0.10;
GLdouble outterRaidus=1.0;
GLint sides =50;
GLint rings =50;
GLint angle;

double px,py,pz;
double z[6][steps+1];

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


void sphere(GLfloat radius, int slices, int stacks)
{
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(xx, yy, zz);
  glutWireSphere(radius, slices, stacks);
  glPopMatrix();
}


void displayTorus(void)
{

    glMatrixMode(GL_MODELVIEW);   // clear the drawing buffer.
    glClear(GL_COLOR_BUFFER_BIT);  // clear the identity matrix.
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(1.0,0.0,-2.5);
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(xRotated,1.0,0.0,0.0);
    glRotatef(yRotated,0.0,1.0,0.0);
    glRotatef(zRotated,0.0,0.0,1.0);
    glScalef(1.0,1.0,1.0);
    glutSolidTorus(innerRaidus,outterRaidus,sides,rings);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0,0.0,-2.5);
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(xRotated,1.0,0.0,0.0);
    glRotatef(yRotated,0.0,1.0,0.0);
    glRotatef(zRotated,0.0,0.0,1.0);
    glScalef(1.0,1.0,1.0);
    glutSolidTorus(innerRaidus,outterRaidus,sides,rings);
    glPopMatrix();

//    glBegin(GL_LINES);
//     glColor3f(1.0, 1.0, 1.0);
//      glVertex3f(1.0,0.0,-2.5);
//      glVertex3f(xx,yy,zz);
//    glEnd();

//    glBegin(GL_LINE_STRIP);
//    glColor3f(1.0, 1.0, 1.0);
//    for(angle = 0; angle <= steps; angle ++)
//      {
//      lx = z[5][angle];
//      ly = z[4][angle];
//      lz = z[3][angle]-3;
//      glVertex3f(lx, ly, lz);
//      }
//    glEnd();

    sphere(0.05, 50, 50);

    glFlush();
    // sawp buffers called because we are using double buffering
   // glutSwapBuffers();
}

void reshapeTorus(int x, int y)
{
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0,(GLdouble)x/(GLdouble)y,0.01,50.0);
    glViewport(0,0,x,y);  //Use the whole window for rendering
    gluLookAt(	3.0f, 0.0f, -2.0f,
                0.0f, 0.0f, -3.0f,
                0.0f, 1.0f,  0.0f);
}

void idleTorus(void)
{
    int i;
for (i = 0; i < steps; i++)
{
    xx = z[5][i];
    yy = z[4][i];
    zz = z[3][i]-3;

   displayTorus();
}

}


int main (int argc, char **argv)
{
    int n=6,m=steps,i,j,k;
    double a,b,eps,gx,gy,gz,bx(double),by(double),bz(double);
    a=0.0; b=2*PI; eps=0.000001;
    void rkt1f(double,double [],int, double []);
    double t,h,y[n];
/////////////////////initial values and step////////////////////////////
    y[0]=0.0; y[1]=0.4e6; y[2]=0.15e6;y[3]=0.0;y[4]=0.0;y[5]=0.0;
    t=0.0; h=6e-5/m;
/////////////////////////////create a date file/////////////////////////
    FILE* fp;
    fp = fopen("xyz.dat", "w");
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
        fprintf(fp,"%13.5e " ,t );
        for ( j = 0; j <= n-1 ; j++)
        {
            fprintf(fp,"%13.5e  " ,z[j][i]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1200,1200);
    glutCreateWindow("Plasma");
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glClearColor(0.0,0.0,0.0,0.0);
    //Assign  the function used in events
    glutDisplayFunc(displayTorus);
    glutReshapeFunc(reshapeTorus);
    glutIdleFunc(idleTorus);
    //Let start glut loop
    glutMainLoop();
    return 0;
}
//g++ -framework OpenGL -framework GLUT torus.cpp
