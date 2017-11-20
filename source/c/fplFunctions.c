/*
 *  A function illustrating how to link C code to code generated from LLVM 
 */

#include <stdio.h>

void put_wall(int x1, int y1, int x2, int y2, int degree, int shiftX, int shiftY){
    printf("Hello put_wall\n");
    printf("x1:%d, y1:%d, x2:%d, y2:%d, degree:%d, shiftX:%d, shigtY:%d\n", x1, y1, x2, y2, degree, shiftX, shiftY);
}

void put_bed(int x1, int y1, int x2, int y2, int degree, int shiftX, int shiftY){
    printf("Hello put_bed\n");
    printf("x1:%d, y1:%d, x2:%d, y2:%d, degree:%d, shiftX:%d, shigtY:%d\n", x1, y1, x2, y2, degree, shiftX, shiftY);
}

#ifdef BUILD_TEST
int main()
{
  put_wall(0, 1, 2, 3, 90, 400, 500);
  put_bed(0, 1, 2, 3, 180, 600, 700);
}
#endif
