#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "stack.h"
#include "object.h"
#include <vector>

#include "bitmap_image.hpp"

using namespace std;

extern double eye[4];
extern double fovY, aspectRatio, near, far;
extern double look[4];
extern double up[4];

extern double PI ;


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