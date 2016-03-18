#include <math.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



bool mouseisdown=false ;

bool loopr=false ;

int mx,my;

int ry=30;

int rx=30;







void timer(int p)

{

     ry-=5;

        glutPostRedisplay();

     if (loopr)

          glutTimerFunc(200,timer,0);







}







void mouse(int button, int state, int x, int y)

{

    if (button == GLUT_LEFT_BUTTON)

     {







        if (state == GLUT_DOWN)

         {    mouseisdown=true ; loopr=false ;}

         else

              mouseisdown=false ;

     }

     if (button== GLUT_RIGHT_BUTTON)

         if (state == GLUT_DOWN)

         {loopr=true ; glutTimerFunc(200,timer,0);}



}







void motion(int x, int y)

{

    if (mouseisdown==true )

    {

        ry+=x-mx;

         rx+=y-my;

         mx=x;

         my=y;

         glutPostRedisplay();

    }

}







void special(int key, int x, int y)







{

    switch (key)

    {

    case GLUT_KEY_LEFT:

        ry-=5;

        glutPostRedisplay();

        break ;

    case GLUT_KEY_RIGHT:

       ry+=5;

        glutPostRedisplay();

        break ;

    case GLUT_KEY_UP:

        rx+=5;

        glutPostRedisplay();

        break ;

    case GLUT_KEY_DOWN:

        rx-=5;



        glutPostRedisplay();

        break ;

    }

}













void init()

    //设置OpenGL的一些状态变量的初始值

{

    glEnable(GL_DEPTH_TEST);     //深度测试

    glDepthFunc(GL_LESS);                      //设置深度测试函数

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);        //设置多边形显示模式为正反面都是填充显示

    glClearColor(1,1,1,1);          //设置刷新背景色

    glClearDepth(1);          //设置清除深度缓冲区所用的值

}







void display()

{



         float red[3]={1,0,0};

         float blue[3]={0,1,0};

         float green[3]={0,0,1};

         float yellow[3]={1,1,0};







        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





         glLoadIdentity();

         glRotatef(ry,0,1,0);

         glRotatef(rx,1,0,0);





         glBegin(GL_QUADS);

              glColor3fv(green);

              glVertex3f(0.5,0.5,0);

              glVertex3f(-0.5,0.5,0);

              glVertex3f(-0.5,-0.5,0);

              glVertex3f(0.5,-0.5,0);





         glEnd();



         glBegin(GL_QUADS);

              glColor3fv(red);

              glVertex3f(0.5,0.5,0.3);

              glVertex3f(-0.5,0.5,-0.3);

              glVertex3f(-0.5,-0.5,-0.3);

              glVertex3f(0.5,-0.5,0.3);





         glEnd();



         glBegin(GL_QUADS);



              glColor3fv(yellow);

              glVertex3f(0.5,0.5,-0.3);

              glVertex3f(-0.5,0.5,0.3);

              glVertex3f(-0.5,-0.5,0.3);

              glVertex3f(0.5,-0.5,-0.3);





         glEnd();







        glFlush();

         glutSwapBuffers();



}






int main(int argc, char ** argv)

{



    glutInit(&argc, argv);

    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGBA|GLUT_DEPTH);         //设置显示模式：单缓冲区， RGBA颜色模式

    glutInitWindowSize (400, 400);        //设置窗口宽度、高度

    glutInitWindowPosition(100,100);         //设置窗口位置

    glutCreateWindow ("double");        //弹出窗口

     init();

    glutDisplayFunc (display);        //设置窗口刷新的回调函数

     glutMouseFunc(mouse);         //设置鼠标器按键回调函数

    glutMotionFunc(motion);

     glutSpecialFunc(special);

    glutMainLoop();        //开始主循环

    return 0;

}
