/*
 *  A function illustrating how to link C code to code generated from LLVM 
 */

#include <stdio.h>

void put_wall(float x, float y, int degree, float shiftX, float shiftY){
    printf("Hello put_wall\n");
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

void put_bed(float x, float y, int degree, float shiftX, float shiftY){
    printf("Hello put_bed\n");
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

void put_desk(float x, float y, int degree, float shiftX, float shiftY){
    printf("Hello put_desk\n");
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

void put_door(float x, float y, int degree, float shiftX, float shiftY){
    printf("Hello put_door\n");
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

void put_window(float x, float y, int degree, float shiftX, float shiftY){
    printf("Hello put_window\n");
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

void put_rectangle(float x, float y, int degree, float shiftX, float shiftY){
    printf("Hello put_rectangle\n");
    printf("x:%f, y:%f, degree:%d, shiftX:%f, shigtY:%f\n", x, y, degree, shiftX, shiftY);
}

void put_circle(float x, float y, float diameter, float shiftX, float shiftY){
    printf("Hello put_circle\n");
    printf("x:%f, y:%f, diameter:%f, shiftX:%f, shigtY:%f\n", x, y, diameter, shiftX, shiftY);
}

void render(){
    printf("Hello render\n");
}

#ifdef BUILD_TEST
int main()
{
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
