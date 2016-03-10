#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
//#include <GLFW/glfw3.h>

GLfloat xRotated, yRotated, zRotated;
GLfloat xx;
GLdouble innerRaidus=0.15;
GLdouble outterRaidus=1;
GLint sides =50;
GLint rings =50;

void myWireSphere(GLfloat radius, int slices, int stacks)
{
  glPushMatrix();
  glColor3f(1.0, 1.0, 0.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  glTranslatef(xx,0.0,0.0);
  glutSolidSphere(radius, slices, stacks);
  glPopMatrix();
}


void displayTorus(void)
{

    glMatrixMode(GL_MODELVIEW);   // clear the drawing buffer.
    glClear(GL_COLOR_BUFFER_BIT);  // clear the identity matrix.
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(0.0,0.0,-4.0);
    glColor3f(1.0, 0.0, 0.0);
    glRotatef(xRotated,1.0,0.0,0.0);
    glRotatef(yRotated,0.0,1.0,0.0);
    glRotatef(zRotated,0.0,0.0,1.0);
    glScalef(1.0,1.0,1.0);
    glutSolidTorus(innerRaidus,outterRaidus,sides,rings);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,0.0,-2);
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(xRotated,1.0,0.0,0.0);
    glRotatef(yRotated,0.0,1.0,0.0);
    glRotatef(zRotated,0.0,0.0,1.0);
    glScalef(1.0,1.0,1.0);
    glutSolidTorus(innerRaidus,outterRaidus,sides,rings);
    glPopMatrix();
    // Flush buffers to screen

    //glPushMatrix();
//    glColor3f(0.0, 0.0, 1.0);
    myWireSphere(0.1, 50, 50);
    //glPopMatrix();

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
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.5
    //Far clipping plane distance: 20.0

    gluPerspective(80.0,(GLdouble)x/(GLdouble)y,0.5,50.0);

    glViewport(0,0,x,y);  //Use the whole window for rendering
}

void idleTorus(void)
{
     yRotated += 1.0;
     xx += 0.1;
    displayTorus();
}


int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1200,1200);
    glutCreateWindow("Plasma");
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    xRotated = yRotated = zRotated = 30.0;
//     xRotated=33;
//     yRotated=40;
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
