#ifdef __APPLE__
    #include <OpenGL/gl.h>  
    #include <Glut/glut.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <GLUT/glcmolorut.h>
#else
     #include <GL/glut.h>
#endif

int main_title_id = -1;

float color[3] = {0,0,0};
float pos[3] = {0,0,0};
int i = 0;
int mode = 0;

//glutReshapeFunc
void ReshapeSence(int w,int h)
{
    float nRange = 300.0f;
    if(h == 0)
        h = 1;
    float nRatio = w / h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w <= h)
        glOrtho(-nRange,nRange,-nRange / nRatio,nRange / nRatio,-nRange,nRange);
    else
        glOrtho(-nRange * nRatio,nRange * nRatio,-nRange,nRange,-nRange,nRange);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setupRC(void)
{
    glClearColor(0.95,0.95,0.95,0.5f);
}

float colorPos(float x)
{
    return x > 1.0 ? x - 1.0 : x;
}

void drawline(float x1,float y1,float x2,float y2)  //The function to draw a line from point(x1,y1) to point (x2,y2)
{
    glColor3f (0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}

//glutDisplayFunc
void DisplaySence(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glBegin(GL_POLYGON);
    //glBegin(GL_QUADS);
    glColor3f(color[0],color[1],color[2]);
    glVertex3f(pos[0],pos[1],pos[2]);
    glColor3f(color[1],color[2],color[0]);
    glVertex3f(pos[1],pos[2],pos[0]);
    glColor3f(color[2],color[0],color[1]);
    glVertex3f(pos[2],pos[0],pos[1]);
    glEnd();


    drawline(-300,-128,300,-128);

    glutSwapBuffers();
}
void TimerFunction(int value)
{
    if(i == 256){
        i = 0;
        mode ++;
        if(mode == 3) mode = 0;
    }
    //color[mode % 3] = (float)i/255;
    //color[(mode + 1) % 3] = (float)(255 - i)/255;
    //color[(mode + 2) % 3] = 0;

    color[0] = 1.f;
    color[1] = 0;
    color[2] = 0;
    
    pos[mode % 3] = i - 128;
    pos[(mode + 1) % 3] = 128 - i;
    pos[(mode + 2) % 3] = -128;
    
    i++;
    glutPostRedisplay();
    glutTimerFunc(30,TimerFunction,1);
}

int main(int argc,char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE|GLUT_DEPTH);
    glutInitWindowPosition(100,40);
    glutInitWindowSize(800,600);
      
    main_title_id = glutCreateWindow("Hello, OpenGL!");
    glutDisplayFunc(DisplaySence);
    glutReshapeFunc(ReshapeSence);
    setupRC();
    glutTimerFunc(200,TimerFunction,1);
    glutMainLoop();
    return 0;
}


