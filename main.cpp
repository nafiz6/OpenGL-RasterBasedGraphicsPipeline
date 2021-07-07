#include <iostream>
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "stage4.h"


#include <stdlib.h>
#include <time.h>   


using namespace std;

double PI =(2*acos(0.0));


double eye[4];
double look[4];
double up[4];
double fovY, aspectRatio, near, far;


int main(int argc, char* argv[]){
    srand (time(NULL));
    cout << std::setprecision(6) << std::fixed;

    string input;
    stage1Func();
    stage2Func();
    stage3Func();
    stage4Func();

}

