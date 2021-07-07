#ifndef OBJECT_H
#define OBJECT_H

#include "matrix.h"
#include <stdlib.h>
#include <time.h>   

class Point {
    public:
        double x,y,z;
        Point(int i, int j, int k){
            x = i;
            y = j;
            z = k;
        }

        Point(){
            x=0;
            y=0;
            z=0;
        }
};

class Triangle {
    public:
        Point points[3];
        int color[3];

        Triangle(){
            color[0] = rand() % 255 + 1;
            color[1] = rand() % 255 + 1;
            color[2] = rand() % 255 + 1;

        }
};

#endif