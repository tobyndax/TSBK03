#include <math.h>
#include "voronoi.h"
#include <stdbool.h>
#include <stdio.h>

bool leftOf(int Ax ,int Ay,int Bx,int By,int Mx,int My){
  int tmp = (Bx-Ax)*(My-Ay) -
            (By-Ay)*(Mx-Ax);

  if(tmp > 0){
    return true;
  }
  else if(tmp == 0){
    int dist1 = pow((Ax -Bx),2) - pow((Ay -By),2);
    int dist2 = pow((Ax -Mx),2) - pow((Ay -My),2);
    if(dist2 > dist1){
      return true;
    }
  }
  return false;

}

void mainVoronoi(){
  int numPoints = 4;
  int pointsX[] = {25,75,25,75};
  int pointsY[] = {25,25,75,75};
  int xSize = 100;
  int ySize = 100;
  int bin[xSize][ySize][numPoints];

    printf("Distance \n");

  for (int j = 0; j < xSize; j++) {
    for (int i = 0; i < ySize; i++) {
      int minDist = pow((pointsX[0] - i),2) + pow((pointsY[0]-j),2);
      bin[i][j][0] = 1;
      for (int k = 1; k < numPoints; k++) {
        int tmpDist = pow((pointsX[k] - i),2) + pow((pointsY[k]-j),2);
        if(tmpDist < minDist){
          minDist = tmpDist;
          bin[i][j][k] = 1;
        }
      }
    }
  }

  int firstPoints[2][numPoints];
  int pointsOnHull[2][100][numPoints];
  int stackX[xSize][numPoints];
  int stackY[ySize][numPoints];
  int stack = 0;
  bool firstPoint = true;


  printf("First point and stacks \n");

  for (int k = 0; k < numPoints; k++) {
    stack = 1;
    firstPoint = true;
    for (int j = 0; j < ySize; j++) {
      for (int i = 0; i < xSize; i++) {
        if(bin[i][j][k] == 1){
          stackX[stack][k] = i;
          stackY[stack][k] = j;
          stack++;
          if(firstPoint){
            firstPoint = false;
            firstPoints[0][k] = i;
            firstPoints[1][k] = j;
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

  for (int k = 0; k < numPoints; k++) {
    printf("%i\n", k);
    int i = 1;
    notDone = true;
    while(notDone){

      pointsOnHull[0][i][k] = firstPoints[0][k];
      pointsOnHull[1][i][k] = firstPoints[1][k];
      endPoint[0] = stackX[1][k];
      endPoint[1] = stackY[1][k];

      for (int j = 1; j < stackX[0][k]; j++) {
        if((endPoint[0] == pointsOnHull[0][i][k]  && endPoint[1] == pointsOnHull[1][i][k]) ||
        (leftOf(pointsOnHull[0][i][k],pointsOnHull[1][i][k],endPoint[0],endPoint[1],stackX[j][k],stackY[j][k])) ){
          endPoint[0] = stackX[j][k];
          endPoint[1] = stackY[j][k];
          //printf(" %i",j);
        }
      }

      i += 1;
      pointsOnHull[0][i][k] = endPoint[0];
      pointsOnHull[1][i][k] = endPoint[1];

      if(pointsOnHull[0][i][k] == pointsOnHull[0][1][k] && pointsOnHull[1][i][k] == pointsOnHull[1][1][k]){
        pointsOnHull[0][0][k] = i;
        notDone = false;
      }
    }
  }

  for (int i = 1; i < pointsOnHull[0][0][0]; i++) {
    printf("%i : %i \n", pointsOnHull[0][i][0] , pointsOnHull[1][i][0]);
  }
}
