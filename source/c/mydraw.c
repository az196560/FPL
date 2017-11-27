#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

/* OpenGL coordinate range X = [-2,2], Y = [-2, 2], coordinate obtained from FPL is X=[0,10], Y = [0,10] , so we would do a coordinate calibration */

//global float array to store every object's data to be drawn
static float data[10][6] =  {{0,0,0,0,0,0}};
static int index = 0;
//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
	}
}
//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
}
//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
				   (double)w / (double)h, //The width-to-height ratio
				   1.0,                   //The near z clipping coordinate
				   200.0);                //The far z clipping coordinate
}
//Draws the 3D scene
void draw_rec() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	glBegin(GL_QUADS); //Begin quadrilateral coordinates
glColor3f(0, 0, 0);	//Trapezoid
	glVertex3f(-0.7f, -1.5f, -5.0f);
	glVertex3f(0.7F, -1.5f, -5.0f);
	glVertex3f(0.7f, -0.5f, -5.0f);
	glVertex3f(-0.7f, -0.5f, -5.0f);
	glEnd(); //End quadrilateral coordinates
	glutSwapBuffers(); //Send the 3D scene to the screen
}

//Draws the 3D scene
void draw_line() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	glBegin(GL_LINES); //Begin quadrilateral coordinates
    glColor3f(0, 0, 0);	//Trapezoid
	glVertex3f(1.0f, -2.0f, -5.0f);
	glVertex3f(1.0f, 1.5f, -5.0f);
	glEnd(); //End quadrilateral coordinates
	glBegin(GL_LINES); //Begin quadrilateral coordinates
    glColor3f(0, 0, 0);	//Trapezoid
	glVertex3f(-1.0f, -1.5f, -5.0f);
	glVertex3f(-1.0f, 1.5f, -5.0f);
	glEnd(); //End quadrilateral coordinates
	glutSwapBuffers(); //Send the 3D scene to the screen
}

void drawLine(int a)
{
    int argc = 1;
    char *argv[1]={""};
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400); //Set the window size
	//Create the window
	glutCreateWindow("Basic Shapes - programming-technique.blogspot.com");
	initRendering(); //Initialize rendering
	 glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(draw_line);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop(); //Start the main loop
}
void drawRec(int a)
{
    int argc = 1;
    char *argv[1]={""};
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000); //Set the window size
	//Create the window
	glutCreateWindow("Basic Shapes - programming-technique.blogspot.com");
	initRendering(); //Initialize rendering
	 glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(draw_rec);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop(); //Start the main loop
}

void put_bed(float x, float y, int degree, float shiftX, float shiftY){
    data[index][0] = 2.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) degree;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    printf("x:%f, y:%f, degree:%f, shiftX:%f, shigtY:%f\n, sss:%f\n", data[index][0], data[index][1], data[index][2], data[index][3], data[index][4], data[index][5]);
    index = index + 1;
}

void put_wall(float x, float y, int degree, float shiftX, float shiftY){
    data[index][0] = 1.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) degree;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    printf("x:%f, y:%f, degree:%f, shiftX:%f, shigtY:%f\n, sss:%f\n", data[index][0], data[index][1], data[index][2], data[index][3], data[index][4], data[index][5]);
    index = index + 1;
}

void draw()
{
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
    
    int len = index;
    printf("len : %d\n",len);
    for(int i = 0; i < len; i++)
    {
        if((int) data[i][0] == 1)
        {                
            glBegin(GL_LINES); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][1], data[i][2], -5.0f);
	        glVertex3f(data[i][4], data[i][5], -5.0f);
	        glEnd(); //End quadrilateral coordinates
        }
        if((int) data[i][0] == 2)
        {                
	        glBegin(GL_QUADS); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][2], -5.0f);
	        glVertex3f(data[i][4], data[i][2], -5.0f);
	        glEnd(); //End quadrilateral coordinates
        }
        
    }
	glutSwapBuffers(); //Send the 3D scene to the screen
}

void render(void)
{
    int argc = 1;
    char *argv[1]={""};
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000); //Set the window size
	//Create the window
	glutCreateWindow("Basic Shapes - programming-technique.blogspot.com");
	initRendering(); //Initialize rendering
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(draw);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop(); //Start the main loop 
}
/*
int main(int argc, char** argv) {
	drawLine(1);
    return 0;
}
*/
