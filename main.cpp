#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>
#include <cmath>
#include <iomanip>

using namespace std;

double PI = 3.1415926535897932384626433;

void matrixMul(double a[4][4], double b[4][4], double res[4][4]){
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            double sum = 0;
            for (int k=0; k<4; k++){
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

void vectorSub(double a[4], double b[4], double res[4]){
    for (int i=0; i<4; i++){
        res[i] = a[i] - b[i];
    }
}

void vectorMul(double a, double b[4], double res[4]){
    for (int i=0; i<4; i++){
        res[i] = a*b[i];
    }
}

void vectorNormalize(double a[4], double res[4]){
    double sum = 0;
    for (int i=0; i<4; i++){
        sum += a[i] * a[i];
    }
    sum = 1 / sqrt(sum);

    vectorMul(sum, a, res);
}

double dotProduct(double a[4], double b[4]){
    double sum = 0;
    for (int i=0; i<4; i++){
        sum += a[i] * b[i];
    }
    return sum;

}

void crossProduct(double a[4], double b[4], double res[4]){
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = -(a[0] * b[2] - a[2] * b[0]);
    res[2] = a[0] * b[1] - a[1] * b[0];

    /*
    cout << "CROSS" << endl;
    cout << a[0] << " " << a[1] << " " << a[2] << endl;
    cout << b[0] << " " << b[1] << " " << b[2] << endl;
    cout << res[0] << " " << res[1] << " " << res[2] << endl;
    */
    
}

void normalize(double a[4][4]){
    for (int i=0; i<4; i++){
        for (int j=0; j<3; j++){
            if (a[i][j] == 0)continue;
            a[i][j] /= a[3][j];
        }
    }
}

void rotate(double axis[4], double point[4], double angle, double res[4]){
    double vec1[4];
    double vec2[4];
    double vec3[4];

    for (int i=0; i<4; i++){
        //cout << "P : " << point[i] <<endl;
    }

    vectorMul(cos(angle), axis, vec1);
    vectorMul( (1 - cos(angle)) * dotProduct(axis, point), point, vec2);
    
    double cross[4];
    crossProduct(point, axis, cross);
    vectorMul( sin(angle), cross, vec3);

    //cout << endl;

    for (int i=0; i<3; i++){
        res[i] = vec1[i] + vec2[i] + vec3[i];
        //cout << vec1[i] << " " << vec2[i] << " " << vec3[i] << endl;
    }
    //cout << endl;
    
    
}

class Stack{
    public :
        double top[4][4];
        double stack[100][4][4];
        int size;

        Stack(){
            size = 0;
            //Identity matrix;
            for (int i=0; i<4; i++){
                for (int j=0; j<4; j++){
                    top[i][j] = 0;
                    if (i==j){
                        top[i][j] = 1;
                    }
                }
            }
        }

        void push(){

            for (int i=0; i<4; i++){
                for (int j=0; j<4; j++){
                    stack[size][i][j] = top[i][j];

                }
            }
            size++;
        }
        

        void pop(){
            //cout << "POP " << endl;
            size--;
            for (int i=0; i<4; i++){
                for (int j=0; j<4; j++){
                    top[i][j] = stack[size][i][j];
                    //cout << stack[size][i][j] << " ";

                }
                //cout << endl;
            }
            //cout << endl;
        }

        void add(double mat[4][4]){
            double res[4][4];
            matrixMul(top, mat, res);
            
            for (int i=0; i<4; i++){
                for (int j=0; j<4; j++){
                    top[i][j] = res[i][j];
                }
            }
            
        }
        
};

int main(int argc, char* argv[]){

    double identity[4][4] = {{1,0,0,0}, {0,1,0,0},{0,0,1,0},{0,0,0,1}};


    string input;
    ifstream inputFile("scene.txt"); 
    ofstream stage1("stage1.txt");

    stage1 << std::setprecision(7) << std::fixed;

    double eye[4];
    double look[4];
    double up[4];
    double fovY, aspectRatio, near, far;

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


    // Stage 2

    ofstream stage2("stage2.txt");
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

    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            cout << viewMatrix[i][j] << " ";
        }

        cout << endl;
    }

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




/*

    // Stage 3
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY/2);
    double r2 = near * tan(fovX/2);

    ofstream stage3("stage3.txt");
    ifstream stage2Read("stage2.txt");

    double projectionMat[4][4] = {0};

    projectionMat[0][0] = near / r2;
    projectionMat[1][1] = near / t;
    projectionMat[2][2] = -(far + near)/(far - near);
    projectionMat[2][3] = -(2*far*near)/(far - near);
    projectionMat[3][2] = -1;

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
                cout << points[j][i] << " " ;
            }
            cout << 1 << endl ;
            points[3][i] = 1;
        }
        cout << endl;
        getline(stage2Read, input);

        double output[4][4];
        matrixMul(viewMatrix, points, output);

        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++){
                stage3 << output[j][i] << " ";
            }
            stage3 << endl;
        }
        stage3 << endl;

    }

    stage3.close();
    */



    ofstream stage4("stage4.txt");





    
}

