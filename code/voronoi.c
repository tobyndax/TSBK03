#include <math.h>
#include "voronoi.h"
#include <stdbool.h>
#include <stdio.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "mac/MicroGlut.h"
#include <ApplicationServices/ApplicationServices.h>
#endif
#include "GL_utilities.h"

struct Fragment{
  int numVertices;
  GLfloat (*vertices)[];
  GLfloat (*textCoord)[];
  GLuint (*indicies)[];
};

bool leftOf(int Ax ,int Ay,int Bx,int By,int Mx,int My,double firstX, double firstY){

  double tmp = (Bx-Ax)*(My-Ay) -
  (By-Ay)*(Mx-Ax);

  if(tmp > 0){
    return true;
  }
  else if(fabs(tmp) == 0){
    if(firstX == Mx && firstY == My)
    return true;

    double dist1 = pow((Ax -Bx),2) + pow((Ay -By),2);
    double dist2 = pow((Ax -Mx),2) + pow((Ay -My),2);

    if(dist2 > dist1){
      return true;
    }
  }
  return false;

}

void mainVoronoi(){
  int numPoints = 5;
  int pointsX[] = {25,75,25,75,55};
  int pointsY[] = {25,25,75,75,30};
  int xSize = 100;
  int ySize = 100;
  int bin[xSize][ySize][numPoints];

  printf("Distance \n");
  int lastK  = 0;
  for (int i = 0; i < xSize; i++) {
    for (int j = 0; j < ySize; j++) {
      int minDist = 8000000;
      for (int k = 0; k < numPoints; k++) {
        bin[i][j][k] = 0;
        int tmpDist = pow((pointsX[k] - i),2) + pow((pointsY[k]-j),2);
        if(tmpDist < minDist){
          minDist = tmpDist;
          bin[i][j][lastK] = 0;
          bin[i][j][k] = 1;


          if(i==99 && j==43){
            printf("bin[lastK]: %i \n",bin[i][j][lastK]);
            printf("lastK: %i \n",lastK);
            printf("bin[k]: %i \n",bin[i][j][k]);
            printf("k: %i \n\n",k);
            int apa=1;
          }
          lastK = k;
        }
      }
    }
  }

  int firstPoints[2][numPoints];
  int pointsOnHull[2][100][numPoints];
  int stackX[xSize*ySize][numPoints];
  int stackY[xSize*ySize][numPoints];
  int stack = 0;
  bool firstPoint = true;


  printf("First point and stacks \n");
  //seems to be right
  for (int k = 0; k < numPoints; k++) {
    stack = 1;
    firstPoint = true;
    for (int i = 0; i < xSize; i++) {
      for (int j = 0; j < ySize; j++) {
        if(bin[i][j][k] == 1){
          stackX[stack][k] = i;
          stackY[stack][k] = j;
          stack = stack +1;
          //printf("%i %i %i\n", i, j, k);
          if(firstPoint){
            firstPoint = false;
            firstPoints[0][k] = i;
            firstPoints[1][k] = j;
            printf("%i %i \n", i, j);
          }
        }
      }
    }
    stackX[0][k] = stack;
    stackY[0][k] = stack;
  }

  int endPoint[2];

  bool notDone;

  printf("Gift Wrapping: numPoints: %i stack: %i \n", numPoints, stackX[0][0]);
  printf("First points: %i : %i   %i : %i   %i : %i   %i : %i  \n", firstPoints[0][0], firstPoints[1][0], firstPoints[0][1], firstPoints[1][1]
  , firstPoints[0][2], firstPoints[1][2], firstPoints[0][3], firstPoints[1][3]);

  for (int k = 0; k < numPoints; k++) {
    printf("%i\n", k);
    int i = 1;
    notDone = true;

    pointsOnHull[0][1][k] = firstPoints[0][k];
    pointsOnHull[1][1][k] = firstPoints[1][k];



    while(notDone){

      endPoint[0] = stackX[1][k];
      endPoint[1] = stackY[1][k];

      for (int j = 1; j < stackX[0][k]; j++) {
        //printf("%i %i %i\n\n", stackX[j][k], stackY[j][k], k);
        if((endPoint[0] == pointsOnHull[0][i][k]  && endPoint[1] == pointsOnHull[1][i][k]) ||
        (leftOf(pointsOnHull[0][i][k],pointsOnHull[1][i][k],endPoint[0],endPoint[1],stackX[j][k],stackY[j][k],firstPoints[0][k],firstPoints[1][k])) ){
          printf("%i %i %i\n", stackX[j][k], stackY[j][k], k);
          printf("EndPoint: %i %i %i\n\n", endPoint[0], endPoint[1], k);
          endPoint[0] = stackX[j][k];
          endPoint[1] = stackY[j][k];
        }
      }

      //printf("%i %i %i %i %i  \n", endPoint[0],endPoint[1], firstPoints[0][k], firstPoints[1][k], k);

      i = i + 1;
      pointsOnHull[0][i][k] = endPoint[0];
      pointsOnHull[1][i][k] = endPoint[1];

      if(endPoint[0] == firstPoints[0][k] && endPoint[1] ==  firstPoints[1][k]){
        pointsOnHull[0][0][k] = i+1;
        notDone = false;
        printf("%s \n", "Done-------------------------------------------------");
      }
    }
  }

  struct Fragment* fragments[numPoints];
  for (int k = 0; k < numPoints; k++) {
    fragments[k] = malloc(sizeof(struct Fragment));
    fragments[k]->numVertices = pointsOnHull[0][0][k];
    GLfloat ((*tempVertices))[] = malloc(sizeof(GLfloat)*pointsOnHull[0][0][k]*3);
    ((*tempVertices))[0] = pointsX[k]/50-1;
    ((*tempVertices))[1] = pointsY[k]/50-1;
    ((*tempVertices))[2] = 0;

    GLuint (*tempIndices)[] = malloc(sizeof(GLuint)*pointsOnHull[0][0][k]*3);
    (*tempIndices)[0] = 0;
    (*tempIndices)[1] = 1;
    (*tempIndices)[2] = 2;

    GLuint (*tempTexCoord)[] = malloc(sizeof(GLuint)*pointsOnHull[0][0][k]*2);
    (*tempTexCoord)[0] = pointsX[k]/100;
    (*tempTexCoord)[1] = pointsY[k]/100;

    for (int i = 1; i < pointsOnHull[0][0][k]; i++) {
      (*tempVertices)[i*3] = pointsOnHull[0][i][k]/50-1;
      (*tempVertices)[i*3+1] = pointsOnHull[1][i][k]/50-1;
      (*tempVertices)[i*3+2] = 0;

      (*tempIndices)[i*3] = 0;
      (*tempIndices)[i*3+1] = i+1;
      (*tempIndices)[i*3+2] = i+2;

      (*tempTexCoord)[i*2] = pointsOnHull[0][i][k]/100;
      (*tempTexCoord)[i*2+1] = pointsOnHull[0][i][k]/100;
    }
    fragments[k]->vertices = tempVertices;
    fragments[k]->indicies = tempIndices;
    fragments[k]->textCoord = tempTexCoord;
  }
}
