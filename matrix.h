#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>


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

#endif
