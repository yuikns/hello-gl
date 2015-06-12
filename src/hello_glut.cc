#ifdef __APPLE__
    #include <OpenGL/gl.h>  
    #include <Glut/glut.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <GLUT/glut.h>
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
    glColor3f(color[0],color[1],color[2]);
    glBegin(GL_QUADS);
    glVertex3f(pos[0],pos[1],pos[2]);
    glVertex3f(pos[1],pos[2],pos[0]);
    glVertex3f(pos[2],pos[0],pos[1]);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(color[0],color[1],color[2]);
    glBegin(GL_QUADS);
    glVertex3f(-pos[0],-pos[1],-pos[2]);
    glVertex3f(-pos[1],-pos[2],-pos[0]);
    glVertex3f(-pos[2],-pos[0],-pos[1]);
    glEnd();
    
    drawline(-300,-128,300,-128);
    
    int z;
    int step = 1;
    for(z = 0 ; z+color[0]*100 < 80 && z+color[1]*100 < 80 && z+color[2]*100 < 80; z += 5 )
    {
            glBegin(GL_POLYGON);
            glColor3f(colorPos(color[0] +z * 0.1),colorPos(color[1]+z * 0.1),colorPos(color[2]+z * 0.1));
            glBegin(GL_QUADS);
            glVertex3f(pos[0]+ z * step,pos[1]+ z * step,pos[2]+ z * step);
            glVertex3f(pos[1]+ z * step,pos[2]+ z * step,pos[0]+ z * step);
            glVertex3f(pos[2]+ z * step,pos[0]+ z * step,pos[1]+ z * step);
            glEnd();
            glBegin(GL_POLYGON);
            glColor3f(colorPos(color[1]+z * 0.1),colorPos(color[2]+z * 0.1),colorPos(color[0]+z * 0.1));
            glBegin(GL_QUADS);
            glVertex3f(-pos[0]+ z * step,-pos[1]+ z * step,-pos[2]+ z * step);
            glVertex3f(-pos[1]+ z * step,-pos[2]+ z * step,-pos[0]+ z * step);
            glVertex3f(-pos[2]+ z * step,-pos[0]+ z * step,-pos[1]+ z * step);
            glEnd();    
    }
    glutSwapBuffers();
}
void TimerFunction(int value)
{
    if(i == 256){
        i = 0;
        mode ++;
        if(mode == 3) mode = 0;
    }
    color[mode % 3] = (float)i/255;
    color[(mode + 1) % 3] = (float)(255 - i)/255;
    color[(mode + 2) % 3] = 0;
    
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


