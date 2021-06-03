#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>
#include <cmath>
#include <iomanip>
#include <vector>
#include "bitmap_image.hpp"

#include <stdlib.h>
#include <time.h>   


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



double eye[4];
double look[4];
double up[4];
double fovY, aspectRatio, near, far;

void stage1Func(){

    cout << "STAGE 1\n\n";
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

void stage4Func(){

    cout << "STAGE 4\n\n";
    vector<Triangle> triangles;

    fstream zBufferWrite;
    ifstream configRead("config.txt");
    string input;

    ifstream stage3Read("stage3.txt");
    zBufferWrite.open("z_buffer.txt", ios::out);
    zBufferWrite << std::setprecision(6) << std::fixed;

    double screenWidth, screenHeight, leftLim, rightLim, topLim, botLim, frontLim, rearLim;

    while (getline(stage3Read, input))
    {

        //cout << endl;
        Triangle triangle;
        for (int i = 0; i < 3; i++)
        {
            if (i != 0)
                getline(stage3Read, input);

            stringstream linePoints(input);

            linePoints >> triangle.points[i].x;
            linePoints >> triangle.points[i].y;
            linePoints >> triangle.points[i].z;

            //cout << triangle.points[i].x << " " << triangle.points[i].y << " " << triangle.points[i].z << endl;
        }
        //cout << endl;

        triangles.push_back(triangle);
        //cout << endl;
        getline(stage3Read, input);
    }

    getline(configRead, input);
    stringstream lineNums(input);
    lineNums >> screenWidth;
    lineNums >> screenHeight;

    bitmap_image image(screenWidth, screenHeight);
    getline(configRead, input);
    stringstream limXLine(input);
    limXLine >> leftLim;
    rightLim = leftLim * -1;

    getline(configRead, input);
    stringstream limYLine(input);
    limYLine >> botLim;
    topLim = botLim * -1;

    getline(configRead, input);
    stringstream limZLine(input);
    limZLine >> frontLim;
    limZLine >> rearLim;

    //cout << "STAGE 4 " << endl;

    // allocate zBuffer with zMax
    vector<vector<double>> zBuffer(screenHeight, vector<double>(screenHeight, rearLim));

    // initialize as black
    for (int i = 0; i < screenWidth; i++)
    {
        for (int j = 0; j < screenHeight; j++)
        {
            image.set_pixel(i, j, 0, 0, 0);
        }
    }

    // pixel mapping
    double dx = (rightLim - leftLim) / screenWidth;
    double dy = (topLim - botLim) / screenHeight;

    double Top_Y = topLim - dy / 2.0;
    double Bot_Y = botLim + dy / 2.0;
    double Left_X = leftLim + dx / 2.0;
    double Right_X = rightLim - dx / 2.0;

    for (int k = 0; k < triangles.size(); k++)
    {
        Triangle triangle = triangles[k];
        double minY, maxY, minX, maxX;
        minY = triangle.points[0].y;
        maxY = triangle.points[0].y;

        minX = triangle.points[0].x;
        maxX = triangle.points[0].x;

        for (int i = 0; i < 3; i++)
        {
            minY = min(minY, triangle.points[i].y);
            maxY = max(maxY, triangle.points[i].y);

            minX = min(minX, triangle.points[i].x);
            maxX = max(maxX, triangle.points[i].x);

            //cout << triangle.points[i].x << " " << triangle.points[i].y << " " << triangle.points[i].z  << endl;
        }

        // clip
        minY = max(Bot_Y, minY);
        maxY = min(Top_Y, maxY);

        minX = max(Left_X, minX);
        maxX = min(Right_X, maxX);

        int minYRow = round((Top_Y - minY) / dy);
        int maxYRow = round((Top_Y - maxY) / dy);

        //cout << minYRow << " " << maxYRow << endl;

        // Y - scanline
        for (int i = maxYRow; i <= minYRow; i++)
        {

            double xab[2];
            double zab[2];
            for (int j = 0; j < 2; j++)
            {
                xab[j] = 0;
                zab[j] = 0;
            }

            int idx = 0;
            double ys = Top_Y - i * dy;
            for (int j = 0; j < 3; j++)
            {
                int next = (j + 1) % 3;
                // check which pair cuts the scanline
                if (min(triangle.points[j].y, triangle.points[next].y) <= ys &&
                    max(triangle.points[next].y, triangle.points[j].y) >= ys && triangle.points[next].y != triangle.points[j].y)
                {
                    xab[idx] = triangle.points[j].x - (triangle.points[j].x - triangle.points[next].x) * ((triangle.points[j].y - ys) / (triangle.points[j].y - triangle.points[next].y));
                    zab[idx] = triangle.points[j].z - (triangle.points[j].z - triangle.points[next].z) * ((triangle.points[j].y - ys) / (triangle.points[j].y - triangle.points[next].y));
                    idx++;
                }
                if (idx == 2)
                    break;
            }

            double unClippedXab[2];
            double unClippedZab[2];

            unClippedXab[0] = xab[0];
            unClippedXab[1] = xab[1];

            unClippedZab[0] = zab[0];
            unClippedZab[1] = zab[1];

            // clip
            for (int j = 0; j < 2; j++)
            {
                if (xab[j] > maxX){
                    xab[j] = maxX;
                }
                if (xab[j] < minX){
                    xab[j] = minX;
                }
            }

            zab[0] = unClippedZab[1] - (unClippedZab[1] - unClippedZab[0]) * ((unClippedXab[1] - xab[0])/ (unClippedXab[1] - unClippedXab[0]));
            zab[1] = unClippedZab[1] - (unClippedZab[1] - unClippedZab[0]) * ((unClippedXab[1] - xab[1])/ (unClippedXab[1] - unClippedXab[0]));

            double xa, xb, za, zb;
            if (xab[0] < xab[1])
            {
                xa = xab[0];
                za = zab[0];

                xb = xab[1];
                zb = zab[1];
            }
            else
            {
                xa = xab[1];
                za = zab[1];

                xb = xab[0];
                zb = zab[0];

                double temp = unClippedXab[1];
                unClippedXab[1] = unClippedXab[0];
                unClippedXab[0] = temp;
            }

            int xaRow = round((xa - Left_X) / dx);
            int xbRow = round((xb - Left_X) / dx);

            //xa = unClippedXab[0];
            //xb = unClippedXab[1];

            for (int j = xaRow; j <= xbRow; j++)
            {
                double xp = Left_X + j * dx;
                double zp = zb - (zb - za) * ((xb - xp) / (xb - xa));
                if (zp < frontLim)
                    continue;

                if (zp < zBuffer[i][j])
                {
                    zBuffer[i][j] = zp;
                    image.set_pixel(j, i, triangle.color[0], triangle.color[1], triangle.color[2]);
                }
            }
        }
        //cout << endl;
    }

    for (int i = 0; i < screenWidth; i++)
    {
        for (int j = 0; j < screenHeight; j++)
        {
            if (zBuffer[i][j] != rearLim)
            {
                zBufferWrite << zBuffer[i][j] << "\t";
            }
        }
        zBufferWrite << endl;
    }

    stage3Read.close();
    configRead.close();
    zBufferWrite.close();

    image.save_image("out.bmp");
    ;
}

int main(int argc, char* argv[]){
    srand (time(NULL));
    cout << std::setprecision(6) << std::fixed;

    string input;
    stage1Func();
    stage2Func();
    stage3Func();
    stage4Func();


    
}

