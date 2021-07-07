#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "stack.h"

using namespace std;

extern double eye[4];
extern double fovY, aspectRatio, near, far;
extern double look[4];
extern double up[4];

extern double PI;


void stage3Func(){
    cout << "STAGE 3\n\n";

    // Stage 3
    string input;
    double fovX = fovY * aspectRatio;
    double t = near * tan((fovY/2) * (PI / 180));
    double r2 = near * tan((fovX/2) * (PI / 180));

    ofstream stage3("stage3.txt");
    fstream stage2Read;
    stage2Read.open("stage2.txt", ios::in);
    stage3 << std::setprecision(7) << std::fixed;

    double projectionMat[4][4] = {0};

    projectionMat[0][0] = near / r2;
    projectionMat[1][1] = near / t;
    projectionMat[2][2] = -(far + near)/(far - near);
    projectionMat[2][3] = -(2*far*near)/(far - near);
    projectionMat[3][2] = -1;

    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            //cout << projectionMat[i][j] << " ";
        }

        //cout << endl;
    }

    while (getline(stage2Read, input)){
        double points[4][4] = {0};

        for (int i = 0; i < 3; i++)
        {
            if (i != 0)
                getline(stage2Read, input);
            stringstream linePoints(input);

            for (int j = 0; j < 3; j++)
            {
                linePoints >> points[j][i];
                //cout << points[j][i] << " " ;
            }
            //cout << 1 << endl ;
            points[3][i] = 1;
        }
        //cout << endl;
        getline(stage2Read, input);

        double output[4][4];
        matrixMul(projectionMat, points, output);
        normalize(output);

        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++){
                stage3 << output[j][i] << " ";
            }
            stage3 << endl;
        }
        stage3 << endl;

    }

    stage3.close();
    stage2Read.close();


}