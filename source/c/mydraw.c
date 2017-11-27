#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
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
	glVertex3f(0.7f, -1.5f, -5.0f);
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
	glutInitWindowSize(400, 400); //Set the window size
	//Create the window
	glutCreateWindow("Basic Shapes - programming-technique.blogspot.com");
	initRendering(); //Initialize rendering
	 glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(draw_rec);
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