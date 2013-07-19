//
//  main.cpp
//  cgtest
//
//  Created by plewis on 7/15/13.
//  Copyright (c) 2013 plewis. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include <Cg/Cg.h>
#include <Cg/cgGL.h>
#include <unistd.h>
#include <math.h>

void display(void);
void reshape(int, int);
void OnIdle(void);
void mouseMove(int, int);
void mouseAction(int, int, int, int);
void keyAction(unsigned char, int, int);
void keyUp(unsigned char key, int x, int y);
CGprogram julia;
CGprogram newton;
CGprogram mandel;
CGprogram ship;
CGprogram gen;
CGprogram pass;
float delta = 0.0;
float _w, _h;
float mouseX = 1.0;
float mouseY = 1.0;
bool mouse_l_down = false;
bool mouse_r_down = false;
float zoom = 2.0;
float centerX = 0.0;
float centerY = 0.0;
float lastZoom = zoom;
float lastCenterX = centerX;
float lastCenterY = centerY;
float lastMouseX = mouseX;
float lastMouseY = mouseY;
float startDragX, startDragY;
float cursorX, cursorY;
bool zoomIn = false;
bool zoomOut = false;
bool panL = false;
bool panR = false;
bool panU = false;
bool panD = false;
bool mouseL = false;
bool mouseR = false;
bool mouseU = false;
bool mouseD = false;

float var1 = 0.0;
float var2 = 0.0;
float var3 = 0.0;

bool var1up = false;
bool var2up = false;
bool var3up = false;
bool var1down = false;
bool var2down = false;
bool var3down = false;

CGprofile f_prof, v_prof;

struct Position{
	float px;
	float py;
	float z;
	float v1;
	float time;
	int listLen;
};

void fillPoint(Position *p, float x, float y, float z, float v, float t, int l)
{
	p->px = x;
	p->py = y;
	p->z = z;
	p->v1 = v;
	p->time = t;
	p->listLen = l;
}

Position *posList;
int posIndex = 0;

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitWindowSize(1000, 1000);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Fractal Shader");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMove);
    glutMouseFunc(mouseAction);
    glutIdleFunc(OnIdle);
    glutKeyboardFunc(keyAction);
    glutKeyboardUpFunc(keyUp);
    glClearColor(0, 0, 0, 1);
    CGcontext context = cgCreateContext();
    if(context == NULL){
        printf("Failed to create CGcontext\n");
        return 1;
    }
	f_prof = cgGLGetLatestProfile(CG_GL_FRAGMENT); 
	v_prof = cgGLGetLatestProfile(CG_GL_VERTEX);
	printf("%d, %d\n", f_prof, v_prof);
    chdir("/Users/peter/Desktop/CG/cgtest");
    julia = cgCreateProgramFromFile(context, CG_SOURCE, "Julia.cg", f_prof, "main", NULL);
    newton = cgCreateProgramFromFile(context, CG_SOURCE, "Newton.cg", f_prof, "main", NULL);
    mandel = cgCreateProgramFromFile(context, CG_SOURCE, "Mandel.cg", f_prof, "main", NULL);
    ship = cgCreateProgramFromFile(context, CG_SOURCE, "Ship.cg", f_prof, "main", NULL);
    pass = cgCreateProgramFromFile(context, CG_SOURCE, "Pass.cg", v_prof, "main", NULL);
	gen = cgCreateProgramFromFile(context, CG_SOURCE, "GenIter.cg", f_prof, "main", NULL);
    printf("%s\n", cgGetErrorString(cgGetError()));
    cgGLLoadProgram(julia);
    cgGLLoadProgram(newton);
    cgGLLoadProgram(mandel);
    cgGLLoadProgram(ship);
	cgGLLoadProgram(gen);
    cgGLLoadProgram(pass);
    //printf("%s\n", cgGetErrorString(cgGetError()));
    cgGLEnableProfile(f_prof);
    cgGLEnableProfile(v_prof);
    printf("%s\n", cgGetErrorString(cgGetError()));
	
	Position demoList[7];
	fillPoint(&demoList[0], 0.297205, 0.063085, 0.001157, 1.012991, 15.0, 7);
	fillPoint(&demoList[1], 0.134370, 0.042840, 0.043976, 1.008928, 15.0, 7);
	fillPoint(&demoList[2], -0.129831, -0.637758, 0.507148, 1.103996, 15.0, 7);
	fillPoint(&demoList[3], -0.162030, -1.02009, 0.041826, 0.983991, 15.0, 7);
	fillPoint(&demoList[4], -0.139263, -1.846086, 0.002345, 0.516934, 15.0, 7);
	fillPoint(&demoList[5], -0.138215, -1.910406, 0.001638, 0.495934, 15.0, 7);
	fillPoint(&demoList[6], -0.138215, -3.552904, 0.302149, 0.149937, 15.0, 7);
	
	posList = demoList;
	zoom = 0.001157;
	
    glutMainLoop();
    return 0;
}

void debugPrint()
{
	printf("Position: %f, %f\n", centerX, centerY);
	printf("Zoom: %f\n", zoom);
	printf("Var1: %f\n", var1);
}

void keyAction(unsigned char key, int x, int y)
{
    switch (key) {
        case ';':
            zoomIn = true;
            break;
            
        case '/':
            zoomOut = true;
            break;
        
        case 'a':
            panL = true;
            break;
            
        case 's':
            panD = true;
            break;
            
        case 'd':
            panR = true;
            break;
            
        case 'w':
            panU = true;
            break;
        
        case 'j':
            mouseL = true;
            break;
            
        case 'k':
            mouseD = true;
            break;
            
        case 'l':
            mouseR = true;
            break;
            
        case 'i':
            mouseU = true;
            break;
			
		case '1':
			var1up = true;
			break;
			
		case '2':
			var1down = true;
			break;
			
		case '9':
			var2up = true;
			break;
			
		case '0':
			var2down = true;
			break;
			
		case ' ':
			debugPrint();
			break;
            
        default:
            break;
    }
}
           
void keyUp(unsigned char key, int x, int y)
{
    switch (key) {
        case ';':
            zoomIn = false;
            break;
            
        case '/':
            zoomOut = false;
            break;
            
        case 'a':
            panL = false;
            break;
            
        case 's':
            panD = false;
            break;
            
        case 'd':
            panR = false;
            break;
            
        case 'w':
            panU = false;
            break;
            
        case 'j':
            mouseL = false;
            break;
            
        case 'k':
            mouseD = false;
            break;
            
        case 'l':
            mouseR = false;
            break;
            
        case 'i':
            mouseU = false;
            break;
			
		case '1':
			var1up = false;
			break;
			
		case '2':
			var1down = false;
			break;
			
		case '9':
			var2up = false;
			break;
			
		case '0':
			var2down = false;
			break;
            
        default:
            break;
    }
}

void updateMouse(int x, int y, float *nx, float *ny)
{
    *nx = (float)x / 500.0 - 1.0;
    *ny = -1*((float)y / 500.0 - 1.0);
}

void mouseAction(int button, int state, int x, int y)
{
    if(button == GLUT_RIGHT_BUTTON){
        if(state == GLUT_DOWN){
            mouse_r_down = true;
            updateMouse(x, y, &cursorX, &cursorY);
            startDragX = cursorX;
            startDragY = cursorY;

        }
        else if(state == GLUT_UP){
            mouse_r_down = false;
            lastZoom = zoom;
            lastCenterX = centerX;
            lastCenterY = centerY;
        }
    }
    else if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            mouse_l_down = true;
			updateMouse(x, y, &cursorX, &cursorY);
			startDragX = cursorX;
			startDragY = cursorY;
        }
        else if(state == GLUT_UP){
            mouse_l_down = false;
            lastCenterX = centerX;
            lastCenterY = centerY;
			lastMouseX = mouseX;
			lastMouseY = mouseY;
        }
    }
}

void mouseMove(int x, int y)
{
    if(mouse_l_down && !mouse_r_down){
        updateMouse(x, y, &cursorX, &cursorY);
		mouseX = lastMouseX - (zoom/5.0) * (cursorX - startDragX);
		mouseY = lastMouseY - (zoom/5.0) * (cursorY - startDragY);
		printf("%f, %f\n", mouseX, mouseY);
    }
    if(mouse_r_down){
        updateMouse(x, y, &cursorX, &cursorY);
        centerX = lastCenterX - pow(zoom, 0.1) * (cursorX - startDragX);
        centerY = lastCenterY - pow(zoom, 0.1) * (cursorY - startDragY);
        //printf("%f, %f\n", centerX, centerY);
    }
}

void updateCamera()
{
	Position pos = posList[posIndex];
	if(pos.listLen > posIndex + 1)
	{
		Position posNext = posList[posIndex + 1];
		float prog = delta / pos.time;
		//prog *= pow(zoom, 0.3);
		float progN = 1.0 - prog;
		centerX = prog * posNext.px + progN * pos.px;
		centerY = prog * posNext.py + progN * pos.py;
		zoom = prog * posNext.z + progN * pos.z;
		var1 = prog * posNext.v1 + progN * pos.v1;
		
		if(prog >= 1.0)
		{
			printf("Point Reached: %d\n", posIndex);
			delta = 0.0;
			posIndex += 1;
		}
	}
}

void OnIdle(void)
{
    delta += 0.016;
    if(zoomIn){
        zoom *= 0.99;
    }
    else if(zoomOut){
        zoom *= 1.02;
    }
    if(panL){
        centerX -= zoom/80;
    }
    else if(panR){
        centerX += zoom/80;
    }
    if(panU){
        centerY += zoom/80;
    }
    else if(panD){
        centerY -= zoom/80;
    }
    if(mouseL){
        mouseX -= zoom/80;
    }
    else if(mouseR){
        mouseX += zoom/80;
    }
    if(mouseU){
        mouseY += zoom/80;
    }
    else if(mouseD){
        mouseY -= zoom/80;
    }
	if(var1up){
		var1 += 0.001;
	}
	else if(var1down){
		var1 -= 0.001;
	}
	if(var2up){
		var2 += 0.001;
	}
	else if(var2down){
		var2 -= 0.001;
	}
    //sleep(0.016);
	
	updateCamera();
	
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);    
    cgGLBindProgram(pass);
    cgGLSetParameter1f(cgGetNamedParameter(pass, "startX"), -1);
    cgGLSetParameter1f(cgGetNamedParameter(pass, "endX"), 1);
    cgGLSetParameter1f(cgGetNamedParameter(pass, "startY"), -1);
    cgGLSetParameter1f(cgGetNamedParameter(pass, "endY"), 1);
	cgGLSetParameter1f(cgGetNamedParameter(julia, "zoom"), zoom);
	cgGLSetParameter2f(cgGetNamedParameter(julia, "zc"), centerX, centerY);
    cgGLSetParameter2f(cgGetNamedParameter(julia, "c"), mouseX, mouseY);
    cgGLSetParameter2f(cgGetNamedParameter(ship, "c"), mouseX, mouseY);
    cgGLSetParameter2f(cgGetNamedParameter(ship, "zc"), centerX, centerY);
    cgGLSetParameter1f(cgGetNamedParameter(ship, "delta"), 5*delta);
    cgGLSetParameter1f(cgGetNamedParameter(ship, "zoom"), zoom);
    cgGLSetParameter1f(cgGetNamedParameter(newton, "zoom"), zoom);
    cgGLSetParameter2f(cgGetNamedParameter(newton, "f"), mouseX, mouseY);
    cgGLSetParameter2f(cgGetNamedParameter(newton, "zc"), centerX, centerY);
	cgGLSetParameter1f(cgGetNamedParameter(mandel, "zoom"), zoom);
	cgGLSetParameter2f(cgGetNamedParameter(mandel, "zc"), centerX, centerY);
	cgGLSetParameter1f(cgGetNamedParameter(gen, "zoom"), zoom);
	cgGLSetParameter2f(cgGetNamedParameter(gen, "zc"), centerX, centerY);
	cgGLSetParameter1f(cgGetNamedParameter(gen, "var1"), var1);
	cgGLSetParameter1f(cgGetNamedParameter(gen, "var2"), var2);
	/*
    //General shader viewer
    cgGLBindProgram(newton);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();
	cgGLDisableProgramProfiles(newton);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD_EXT);
	cgGLBindProgram(gen);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();
	glDisable(GL_BLEND);
    */
	
	//DEMO
	cgGLSetParameter1f(cgGetNamedParameter(gen, "zoom"), zoom);
	cgGLSetParameter2f(cgGetNamedParameter(gen, "zc"), centerX, centerY);
	cgGLSetParameter1f(cgGetNamedParameter(gen, "var1"), var1);
	cgGLSetParameter1f(cgGetNamedParameter(gen, "var2"), var2);
	cgGLBindProgram(gen);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();
	glDisable(GL_BLEND);
	
    /*
    //Mandelbrot viewer with mini Julia set
    cgGLBindProgram(julia);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();
    cgGLSetParameter1f(cgGetNamedParameter(pass, "startX"), 0.25);
    cgGLSetParameter1f(cgGetNamedParameter(pass, "endX"), 1);
    cgGLSetParameter1f(cgGetNamedParameter(pass, "startY"), -0.25);
    cgGLSetParameter1f(cgGetNamedParameter(pass, "endY"), -1);
    cgGLBindProgram(julia);
    glBegin(GL_QUADS);
    glVertex2f(0.25, -0.25);
    glVertex2f(1, -0.25);
    glVertex2f(1, -1);
    glVertex2f(0.25, -1);
    glEnd();
    */
    /*
    //Julia and Newton quads
    cgGLBindProgram(julia);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(-1, 0);
    glVertex2f(0, 0);
    glVertex2f(0, -1);
    glVertex2f(0, 0);
    glVertex2f(1, 0);
    glVertex2f(1, 1);
    glVertex2f(0, 1);
    glEnd();
    cgGLBindProgram(newton);
    glBegin(GL_QUADS);
    glVertex2f(-1, 0);
    glVertex2f(0, 0);
    glVertex2f(0, 1);
    glVertex2f(-1, 1);
    glVertex2f(0, 0);
    glVertex2f(1, 0);
    glVertex2f(1, -1);
    glVertex2f(0, -1);
    glEnd();
    */
    /*
    //Interactive Newton
    cgGLBindProgram(newton);
    cgGLSetParameter1f(cgGetNamedParameter(newton, "delta"), 0.0);
    cgGLSetParameter1f(cgGetNamedParameter(newton, "fx"), mouseX);
    cgGLSetParameter1f(cgGetNamedParameter(newton, "fy"), mouseY);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();
    */
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

