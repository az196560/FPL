/* 
 * Author: Chih-Hung Lu 
 * Here we gonna draw a FPL using openGL with spiritual style ~~~~~~~~~~~!!!!!!!!!!!@@@@@@@@@@############# 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/**************super important********************/
/* OpenGL coordinate range X = [-2,2], Y = [-2, 2], coordinate obtained from FPL is X=[0,10], Y = [0,10] , so we would do a coordinate calibration */

//global float array to store every object's data to be drawn
static float data[64][6] =  {{0,0,0,0,0,0}};
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

// real wall
void put_wall(float x, float y, int degree, float shiftX, float shiftY){
    data[index][0] = 1.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) degree;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    index = index + 1;
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

// real bed
void put_bed(float x, float y, int degree, float shiftX, float shiftY){
    data[index][0] = 2.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) degree;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    index = index + 1;
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

//not a desk in your studying room, but a bunch of sofa and coffee table in living room
void put_desk(float x, float y, int degree, float shiftX, float shiftY){
    data[index][0] = 3.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) degree;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    index = index + 1;
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

// real door 
void put_door(float x, float y, int degree, float shiftX, float shiftY){
    data[index][0] = 4.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) degree;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    index = index + 1;
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

// real window
void put_window(float x, float y, int degree, float shiftX, float shiftY){
    data[index][0] = 5.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) degree;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    index = index + 1;
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

// real rectangle
void put_rectangle(float x, float y, int degree, float shiftX, float shiftY){
    data[index][0] = 6.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) degree;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    index = index + 1;
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

// no circle at all.............
void put_circle(float x, float y, float diameter, float shiftX, float shiftY){
    data[index][0] = 7.0;
    data[index][1] = x*0.4 - 2.0 + shiftX * 0.4;
    data[index][2] = y*0.4 - 2.0 + shiftY * 0.4;
    data[index][3] = (float) diameter;
    data[index][4] = shiftX*0.4 -2;
    data[index][5] = shiftY*0.4 - 2;
    index = index + 1;
    printf("x:%f, y:%f, diameter:%f, shiftX:%f, shigtY:%f\n", x, y, diameter, shiftX, shiftY);
}

void put(int degree, int type, float x, float y, float r, float shiftX, float shiftY){
    printf("Hello put\n");
    printf("type:%d, degree:%d, x:%f, y:%f, r:%f, shiftX:%f, shigtY:%f\n", type, degree, x, y, r, shiftX, shiftY);

    switch (type) {
        case 0:
            put_wall(x, y, degree, shiftX, shiftY);
            break;
        case 1:
            put_bed(x, y, degree, shiftX, shiftY);
            break;
        case 2:
            put_desk(x, y, degree, shiftX, shiftY);
            break;
        case 3:
            put_door(x, y, degree, shiftX, shiftY);
            break;
        case 4:
            put_window(x, y, degree, shiftX, shiftY);
            break;
        case 5:
            put_rectangle(x, y, degree, shiftX, shiftY);
            break;
        case 6:
            put_circle(x, y, r, shiftX, shiftY);
            break;
        default:
            break;
    }
}

// here we start to draw a spiritual FPL ~~~~~~!!!!!@@@@@########$$$$$$$$$$$
void draw()
{
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
    
    int len = index;
    printf("len : %d\n",len);
    int i;
    for(i = 0; i < len; i++)
    {
        // draw a wall using retangle
        if((int) data[i][0] == 1)
        {                
            glTranslatef(0.0f, 0.0f, 0.0f);
            glRotatef(data[i][3], (data[i][1]+data[i][4])/2, (data[i][2]+data[i][5])/2, -5.0f);
	        glBegin(GL_QUADS); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][2], -5.0f);
	        glVertex3f(data[i][4], data[i][2], -5.0f);
	        glEnd(); //End quadrilateral coordinates
        }
        // draw a bed using bunch of lines and shapes, with messy coordinates computation
        if((int) data[i][0] == 2)
        {                
            glTranslatef(0.0f, 0.0f, 0.0f);
            glRotatef(data[i][3], (data[i][1]+data[i][4])/2, (data[i][2]+data[i][5])/2, -5.0f);
	        glBegin(GL_LINE_LOOP); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][2], -5.0f);
	        glVertex3f(data[i][4], data[i][2], -5.0f);
	        glEnd(); //End quadrilateral coordinates
	        glBegin(GL_LINE_LOOP); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][1]/50 + data[i][4]*49/50, data[i][2]*29/30+data[i][5]/30, -5.0f);
	        glVertex3f(data[i][1]/50 + data[i][4]*49/50, data[i][2]*17/30+data[i][5]*13/30, -5.0f);
	        glVertex3f(data[i][1]*9/50 + data[i][4]*41/50, data[i][2]*17/30+data[i][5]*13/30, -5.0f);
	        glVertex3f(data[i][1]*9/50 + data[i][4]*41/50, data[i][2]*29/30+data[i][5]/30,  -5.0f);
	        glEnd(); //End quadrilateral coordinates
	        glBegin(GL_LINE_LOOP); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][1]/50 + data[i][4]*49/50, data[i][2]*13/30+data[i][5]*17/30, -5.0f);
	        glVertex3f(data[i][1]/50 + data[i][4]*49/50, data[i][2]/30+data[i][5]*29/30, -5.0f);
	        glVertex3f(data[i][1]*9/50 + data[i][4]*41/50, data[i][2]/30+data[i][5]*29/30, -5.0f);
	        glVertex3f(data[i][1]*9/50 + data[i][4]*41/50, data[i][2]*13/30+data[i][5]*17/30,  -5.0f);
	        glEnd(); //End quadrilateral coordinates
            glBegin(GL_LINES); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f((data[i][1]/5 + data[i][4]*4/5), data[i][2], -5.0f);
	        glVertex3f((data[i][1]/5 + data[i][4]*4/5), data[i][5], -5.0f);
	        glEnd(); //End quadrilateral coordinates
        }
        // draw a living sofa and coffe table using bunch of shapes
        if((int) data[i][0] == 3)
        {                
            glTranslatef(0.0f, 0.0f, 0.0f);
            glRotatef(data[i][3], (data[i][1]+data[i][4])/2, (data[i][2]+data[i][5])/2, -5.0f);
	        glBegin(GL_LINE_LOOP); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4], data[i][5]*2/3 + data[i][2]/3, -5.0f);
	        glVertex3f(data[i][1]*2/5 + data[i][4]*3/5, data[i][5]*2/3 + data[i][2]/3, -5.0f);
	        glVertex3f(data[i][1]*2/5 + data[i][4]*3/5, data[i][2],  -5.0f);
	        glVertex3f(data[i][4], data[i][2], -5.0f);
	        glEnd(); //End quadrilateral coordinates
	        glBegin(GL_LINE_LOOP); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4]*27/50 + data[i][1]*23/50, data[i][2], -5.0f);
	        glVertex3f(data[i][4]*27/50 + data[i][1]*23/50, data[i][5]*21/30 + data[i][2]*9/30, -5.0f);
	        glVertex3f(data[i][1], data[i][5]*21/30 + data[i][2]*9/30, -5.0f);
	        glVertex3f(data[i][1], data[i][2], -5.0f);
	        glEnd(); //End quadrilateral coordinates
	        glBegin(GL_LINE_LOOP); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4]*19/20 + data[i][1]/20, data[i][5]*21/30 + data[i][2]*9/30, -5.0f);
	        glVertex3f(data[i][4]*19/20 + data[i][1]/20, data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][5]*21/30 + data[i][2]*9/30, -5.0f);
	        glEnd(); //End quadrilateral coordinates
        }
        // draw a door
        if((int) data[i][0] == 4)
        {                
            glTranslatef(0.0f, 0.0f, 0.0f);
            glRotatef(data[i][3], (data[i][1]+data[i][4])/2, (data[i][2]+data[i][5])/2, -5.0f);
	        glBegin(GL_LINE_LOOP);
            glColor3f(0, 0, 0);	//Trapezoid
            float x = data[i][1];
            float y = data[i][5];
	        float r = data[i][1] - data[i][4];
            int ii;
            for(ii = 0; ii < 100; ii++) 
	        {    
		        float theta =  3.1415926f * ii / 300;//get the current angle 
		        float x1 = r * cosf(theta);//calculate the x component 
		        float y2 = r * sinf(theta);//calculate the y component 
		        glVertex3f(x - x1, y + y2,-5.0f);//output vertex 
	        } 
		    glVertex3f(data[i][1], data[i][5],-5.0f);//output vertex 
            glEnd(); //End quadrilateral coordinates
        }
        // draw a window using retangle
        if((int) data[i][0] == 5)
        {                
            glTranslatef(0.0f, 0.0f, 0.0f);
            glRotatef(data[i][3], (data[i][1]+data[i][4])/2, (data[i][2]+data[i][5])/2, -5.0f);
	        glBegin(GL_LINE_LOOP); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][2], -5.0f);
	        glVertex3f(data[i][4], data[i][2], -5.0f);
	        glEnd(); //End quadrilateral coordinates
            glBegin(GL_LINES); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4], data[i][2]*3/4+data[i][5]/4, -5.0f);
	        glVertex3f(data[i][1], data[i][2]*3/4+data[i][5]/4, -5.0f);
	        glEnd(); //End quadrilateral coordinates
        }
        if((int) data[i][0] == 6)
        {                
            glTranslatef(0.0f, 0.0f, 0.0f);
            glRotatef(data[i][3], (data[i][1]+data[i][4])/2, (data[i][2]+data[i][5])/2, -5.0f);
	        glBegin(GL_LINE_LOOP); //Begin quadrilateral coordinates
            glColor3f(0, 0, 0);	//Trapezoid
	        glVertex3f(data[i][4], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][5], -5.0f);
	        glVertex3f(data[i][1], data[i][2], -5.0f);
	        glVertex3f(data[i][4], data[i][2], -5.0f);
	        glEnd(); //End quadrilateral coordinates
        }
        if((int) data[i][0] == 7)
        {                
	        glBegin(GL_LINE_LOOP);
            glColor3f(0, 0, 0);	//Trapezoid
            float x = data[i][1];
            float y = data[i][2];
	        float r = data[i][3]/2;
            int ii;
            for(ii = 0; ii < 600; ii++) 
	        {    
		        float theta =  2 * 3.1415926f * ii / 600;//get the current angle 
		        float x1 = r * cosf(theta);//calculate the x component 
		        float y2 = r * sinf(theta);//calculate the y component 
		        glVertex3f(x + x1, y + y2,-5.0f);//output vertex 
	        } 
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
	//Create the windoW
	glutCreateWindow("Basic Shapes - programming-technique.blogspot.com");
	initRendering(); //Initialize rendering
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(draw);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop(); //Start the main loop 
}


#ifdef BUILD_TEST
int main()
{
  put(0, 0, 0.0, 1.0, 1.0, 400.0, 500.0);
  put_wall(0.0, 1.0, 90, 400.0, 500.0);
  put_bed(0.0, 1.0, 180, 600.0, 700.0);
  put_desk(0.0, 1.0, 270, 100.0, 200.0);
  put_door(0.0, 1.0, 0, 200.0, 300.0);
  put_window(0.0, 1.0, 90, 300.0, 400.0);
  put_rectangle(0.0, 1.0, 180, 400.0, 500.0);
  put_circle(0.0, 1.0, 1.0, 500.0, 600.0);
  render();
}
#endif
