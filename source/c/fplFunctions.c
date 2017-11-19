/*
 *  A function illustrating how to link C code to code generated from LLVM 
 */

#include <stdio.h>

void put_wall(int a, int b, int c, int d, int e, int f){
    printf("Hello put_wall\n");
    printf("a:%d, b:%d, c:%d, d:%d, e:%d, f:%d\n", a, b ,c, d, e, f);
}

void put_bed(int a, int b, int c, int d, int e, int f){
    printf("Hello put_bed\n");
    printf("a:%d, b:%d, c:%d, d:%d, e:%d, f:%d\n", a, b ,c, d, e, f);
}
