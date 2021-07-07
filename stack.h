#ifndef STACK_H
#define STACK_H

#include "matrix.h"

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

#endif