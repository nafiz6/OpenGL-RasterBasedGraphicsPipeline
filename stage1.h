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

void stage1Func(){

    std::cout << "STAGE 1\n\n";
    string input;
    ifstream inputFile("scene.txt"); 
    ofstream stage1("stage1.txt");

    stage1 << std::setprecision(7) << std::fixed;

    getline(inputFile, input);
    int idx = 0;
    stringstream lineEye(input);
    double val;
    while (lineEye >> val){
        eye[idx] = val;
        idx++;
    }

    getline(inputFile, input);
    stringstream lineLook(input);
    idx = 0;
    while (lineLook >> val){
        look[idx] = val;
        idx++;
    }

    getline(inputFile, input);
    stringstream lineUp(input);
    idx = 0;
    while (lineUp >> val){
        up[idx] = val;
        idx++;
    }


    getline(inputFile, input);
    stringstream linestream(input);
    linestream >> fovY;
    linestream >> aspectRatio;
    linestream >> near;
    linestream >> far;
    

    Stack stack;

    while (getline(inputFile, input)){
        if (input == "triangle"){
            double points[4][4] = {0};

            for (int i=0; i<3; i++){
                getline(inputFile, input);
                stringstream linePoints(input);

                for (int j=0; j<3; j++){
                    linePoints >> points[j][i];
                }
                points[3][i] = 1;

            }

            double output[4][4];
            matrixMul(stack.top, points, output);
            normalize(output);
            
            for (int i=0; i<3; i++){
                for (int j=0; j<3; j++){
                    stage1 << output[j][i] << " ";
                }
                stage1 << endl;
            }
            stage1 << endl;


        }
        else if (input == "translate"){
            double translateMat[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}} ;
            getline(inputFile, input);
            stringstream linePoints(input);
            for (int i=0; i<3; i++){

                linePoints >> translateMat[i][3];
            }

            stack.add(translateMat);
        }
        else if (input == "scale"){

            double scaleMat[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
            getline(inputFile, input);
            stringstream linePoints(input);
            for (int i = 0; i < 3; i++)
            {
                linePoints >> scaleMat[i][i];
            }

            stack.add(scaleMat);
        }
        else if (input == "rotate"){
            getline(inputFile, input);
            stringstream linePoints(input);

            double rotateVec[4] = {0, 0, 0, 0};
            double angle;
            linePoints >> angle;

            for (int i = 0; i < 3; i++)
            {

                linePoints >> rotateVec[i];
            }

            double iVec[4] = {1, 0, 0, 0};
            double jVec[4] = {0, 1, 0, 0};
            double kVec[4] = {0, 0, 1, 0};

            double normalizedA[4] = {0};
            vectorNormalize(rotateVec, normalizedA);

            double c1[4];
            double c2[4];
            double c3[4];
            angle = angle * PI / 180;

            rotate(iVec, normalizedA, angle, c1);
            rotate(jVec, normalizedA, angle, c2);
            rotate(kVec, normalizedA, angle, c3);

            double rotateMatrix[4][4] = {0};
            for (int i = 0; i < 3; i++)
            {
                rotateMatrix[i][0] = c1[i];
                rotateMatrix[i][1] = c2[i];
                rotateMatrix[i][2] = c3[i];
            }

            rotateMatrix[3][3] = 1;

            //cout << "ROTATE " << endl;

            for (int i=0; i<4; i++){
                for (int j=0; j<4; j++){
                    //cout << rotateMatrix[i][j] << " ";
                }
                //cout << endl;
            }
            stack.add(rotateMatrix);
        }
        else if (input == "push"){
            stack.push();
        }
        else if (input == "pop"){
            stack.pop();
        }
        else if (input == "end"){
            //cout << input;
            break;
        }
        else{
            //cout << "UNPROCESSED :" << input;

        }



    }
    inputFile.close();
    stage1.close();

}