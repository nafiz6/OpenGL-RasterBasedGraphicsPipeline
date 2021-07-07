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


void stage2Func(){
    cout << "STAGE 2\n\n";

    // Stage 2

    string input;
    fstream stage2;
    stage2.open("stage2.txt", ios::out);
    ifstream stage1Read("stage1.txt"); 
    stage2 << std::setprecision(7) << std::fixed;

    double l[4];
    double lNorm[4];
    vectorSub(look, eye, l);
    vectorNormalize(l, lNorm);

    double r[4];
    double rNorm[4];
    crossProduct(lNorm, up, r);
    vectorNormalize(r, rNorm);

    double u[4];
    crossProduct(rNorm, lNorm, u);

    double cameraTranslation[4][4];
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if (i == j)cameraTranslation[i][j] = 1;
            else{
                cameraTranslation[i][j] = 0;
            }
        }
        if (i < 3){
            cameraTranslation[i][3] = -eye[i];
        }
    }

    double cameraRotation[4][4];
    for (int j=0; j<3; j++){
        cameraRotation[0][j] = rNorm[j];
        cameraRotation[1][j] = u[j];
        cameraRotation[2][j] = -lNorm[j];

        cameraRotation[3][j] = 0;
        cameraRotation[j][3] = 0;
    }

    cameraRotation[3][3] = 1;

    double viewMatrix[4][4];
    matrixMul(cameraRotation, cameraTranslation, viewMatrix);


    while (getline(stage1Read, input)){
        double points[4][4] = {0};

        for (int i = 0; i < 3; i++)
        {
            if (i != 0)
                getline(stage1Read, input);
            stringstream linePoints(input);

            for (int j = 0; j < 3; j++)
            {
                linePoints >> points[j][i];
            }
            points[3][i] = 1;
        }
        getline(stage1Read, input);

        double output[4][4];
        matrixMul(viewMatrix, points, output);
        normalize(output);

        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++){
                stage2 << output[j][i] << " ";
            }
            stage2 << endl;
        }
        stage2 << endl;

    }

    stage1Read.close();
    stage2.close();





}