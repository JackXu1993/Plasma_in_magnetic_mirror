#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#define PI 3.1415926
#define A 0.0
#define B 2*PI
#define EPS 0.000001
#define number 6
#define m 600
#define RADPERDEG 0.0174533
using namespace std;

/////////////////////////////////definition of functions////////////////////////////////
/////////////////////////////////definition of functions////////////////////////////////
double px,py,pz;
const int n=6;
//const int m=6000;
double y[n];
double d[n];
double z[number][m+1];
typedef double (*PT) ( double );
typedef void (*PF) ( double ,int , double[] );
double bx(double xxx);
double by(double xxx);
double bz(double xxx);
void ode(double t ,int n , double d[] );
void sode(double t ,int n ,double h ,int k ,double z[number][m+1], PF f);
double integrate( double a ,double b ,double eps ,PT f ,double xx ,double yy ,double zz );
//////////////////////////////////////variables of OpenGL////////////////////////////////
//////////////////////////////////////variables of OpenGL////////////////////////////////
GLdouble xRotated=0.0, yRotated=90.0, zRotated=0.0;
GLdouble xx=0.0, yy=0.0, zz=0.0 ;
GLdouble lx, ly, lz;
GLdouble innerRaidus=0.10;
GLdouble outterRaidus=1.0;
GLint sides =50;
GLint rings =50;
float angle = 0.0f;// angle of rotation for the camera direction
float llx=0.0f,llz=-1.0f;// actual vector representing the camera's direction
float cx=0.0f, cz=4.0f;// XZ position of the camera
float deltaAngle = 0.0f;// the key states. These variables will be zero/ /when no key is being presses
float deltaMove = 0;
int xOrigin = -1;

/////////////////////////////////Integration function////////////////////////////////
/////////////////////////////////Integration function////////////////////////////////
double integrate( double a ,double b ,double eps ,PT f ,double xx ,double yy ,double zz )
{
    int mm,i,j;
    double s,p,ep,h,aa,bb,w,xxxx,g;
    const double t[5]={-0.9061798459,-0.5384693101,0.0,0.5384693101,0.9061798459};
    const double c[5]={0.2369268851,0.4786286705,0.5688888889,0.4786286705,0.2369268851};
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
void sode(double t ,int n ,double h ,int k ,double z[number][m+1] ,PF f)
{
    int i,j,l;
    double a[4], tt, *b, *d ;
    b=(double*)std::malloc(n*sizeof(double));
    d=(double*)std::malloc(n*sizeof(double));
    a[0]=h/2.0; a[1]=a[0]; a[2]=h; a[3]=h;
    for ( i = 0; i <= n-1 ; i++)
    {
        z[i][0]=y[i];
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
                y[i]=z[i][l-1]+a[j]*d[i];
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
                z[i][l]=y[i];
            }
            t=t+h;
        }
    std::free(b);
    std::free(d);
//    return;
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

/////////////////////////////////////OpenGL functions////////////////////////////////////
/////////////////////////////////////OpenGL functions////////////////////////////////////
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
int i;
    glMatrixMode(GL_MODELVIEW);   // clear the drawing buffer.
    glClear(GL_COLOR_BUFFER_BIT);  // clear the identity matrix.
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(1.0,0.0,0.0);
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(xRotated,1.0,0.0,0.0);
    glRotatef(yRotated,0.0,1.0,0.0);
    glRotatef(zRotated,0.0,0.0,1.0);
    glScalef(1.0,1.0,1.0);
    glutSolidTorus(innerRaidus,outterRaidus,sides,rings);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0,0.0,0.0);
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(xRotated,1.0,0.0,0.0);
    glRotatef(yRotated,0.0,1.0,0.0);
    glRotatef(zRotated,0.0,0.0,1.0);
    glScalef(1.0,1.0,1.0);
    glutSolidTorus(innerRaidus,outterRaidus,sides,rings);
    glPopMatrix();

    glBegin(GL_LINES);
    // draw line for x axis
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    // draw line for y axis
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    // draw line for Z axis
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();

    glBegin(GL_LINES);
    for (i = -10; i <= 10; i++) {
        glColor3f(1.0,1.0,1.0);
        glVertex3f(10.0,-4.0,i);
        glVertex3f(-10.0,-4.0,i);
    }
    glEnd();

    glBegin(GL_LINES);
    for (i = -10; i <= 10; i++) {
        glColor3f(1.0,1.0,1.0);
        glVertex3f(i,-4.0,10.0);
        glVertex3f(i,-4.0,-10.0);
    }
    glEnd();
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
/////////////////////////////////////////////////////////////////
void computePos(float deltaMove) {

	cx += deltaMove * llx * 0.1f;
	cz += deltaMove * llz * 0.1f;
}


void reshapeTorus(int x, int y)
{
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0,(GLdouble)x/(GLdouble)y,0.01,50.0);
    glViewport(0,0,x,y);  //Use the whole window for rendering
    if (deltaMove)
        computePos(deltaMove);

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
//    glLoadIdentity();
    // Set the camera
    gluLookAt(	cx, 1.0f, cz,
            cx+llx, 1.0f,  cz+llz,
            0.0f, 1.0f,  0.0f);
}


void processNormalKeys(unsigned char key, int cxx, int cyy) {

        if (key == 27)
              exit(0);
}

void pressKey(int key, int cxx, int cyy) {

       switch (key) {
             case GLUT_KEY_UP : deltaMove = 0.5f; break;
             case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
       }
}

void releaseKey(int key, int x, int y) {

        switch (key) {
             case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : deltaMove = 0;break;
        }
}

void mouseMove(int x, int y) {

         // this will only be true when the left button is down
         if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		llx = sin(angle + deltaAngle);
		llz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

void idleTorus(void)
{
    int i;
for (i = 0; i < m; i++)
{
    xx = z[5][i];
    yy = z[4][i];
    zz = z[3][i];

   displayTorus();
}
}

/////////////////////////////////////main function////////////////////////////////////
/////////////////////////////////////main function////////////////////////////////////
int main (int argc, char **argv)
{
    int i,j,k;
    double t,h;
    float ratio;
//    double z[n][m+1];
    PF rr;
    rr=ode;
/////////////////////initial values and step////////////////////////////
    printf("input the ratio of vy and vz (default value is 2): ");
    scanf_s("%f",&ratio);
//    printf("%7.2f\n",ratio );
    y[0]=0.0; y[1]=ratio*0.15e6; y[2]=0.15e6; y[3]=0.0;y[4]=0.0;y[5]=0.0;
    t=0.0; h=6e-5/m;
/////////////////////////////create a date file/////////////////////////
    FILE* fp;
    fopen_s(&fp, "xyz++.dat", "w");
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
////////////////////////////////animation by OpenGL/////////////////////////////////
////////////////////////////////animation by OpenGL/////////////////////////////////
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
    glutIgnoreKeyRepeat(1);

glutKeyboardFunc(processNormalKeys);
glutSpecialFunc(pressKey);
glutSpecialUpFunc(releaseKey);

// here are the two new functions
glutMouseFunc(mouseButton);
glutMotionFunc(mouseMove);
// OpenGL init
glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
