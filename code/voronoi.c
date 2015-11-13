#include <math.h>
#include "voronoi.h"
#include <stdbool.h>
#include <stdio.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "mac/MicroGlut.h"
#include <ApplicationServices/ApplicationServices.h>
#endif

struct Fragment* fragments;
Model** models;
vec3* dirs;

bool firstTime = true;


void allocate2D(double **dat, int nrows, int ncols) {
  int i;
  /*  allocate array of pointers  */
  dat = malloc( nrows*sizeof( double* ) );

  /*  allocate each row  */

  for(i = 0; i < nrows; i++) {
    dat[i] = malloc( ncols*sizeof( double ) );
  }
  if(dat==NULL || dat[i-1]==NULL) {
    printf("\nError allocating memory\n");
    exit(1);
  }

}


// The following functions are from http://www.mas.ncl.ac.uk/~ndjw1/teaching/sim/transf/norm.c
float surand()
{
  return( (float) rand()/RAND_MAX );
}

float urand(float low, float high)
{
  return(low+(high-low)*surand());
}

float genexp(float lambda)
{
  float u,x;
  u=surand();
  x=(-1/lambda)*log(u);
  return(x);
}

float gennor(GLfloat conc)
{
  float theta,rsq,x;
  theta=urand(0,2*M_PI);
  rsq=genexp(conc);
  x=sqrt(rsq)*cos(theta);
  return(x);
}

// end of http://www.mas.ncl.ac.uk/~ndjw1/teaching/sim/transf/norm.c


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

int** my2DAllocation(int rows,int columns)
{
  int i;
  int header= rows *sizeof(int *);
  int data=rows*columns*sizeof(int);
  int ** rowptr=(int **)malloc(header+data);
  if(rowptr==NULL)
  {
    return NULL;
  }
  int * buf=(int*)(rowptr+rows);
  for(i=0;i<rows;i++)
  {
    rowptr[i]=buf+i*columns;
  }
  return rowptr;
}

void free_data(int ***data, size_t xlen, size_t ylen)
{
  size_t i, j;

  for (i=0; i < xlen; ++i) {
    if (data[i] != NULL) {
      for (j=0; j < ylen; ++j)
      free(data[i][j]);
      free(data[i]);
    }
  }
  free(data);
}

int ***my3DAllocation(size_t xlen, size_t ylen, size_t zlen)
{
  int ***p;
  int i, j;
  //allocate all data in xlen direction
  if ((p = malloc(xlen * sizeof *p)) == NULL) {
    perror("malloc 1");
    return NULL;
  }
  //set all data to NULL.
  for (i=0; i < xlen; ++i)
  p[i] = NULL;

  //For each position alloc ylen dimension.
  for (i=0; i < xlen; ++i)
  if ((p[i] = malloc(ylen * sizeof *p[i])) == NULL) {
    perror("malloc 2");
    free_data(p, xlen, ylen);
    return NULL;
  }


  //set all data to NULL.
  for (i=0; i < xlen; ++i)
  for (j=0; j < ylen; ++j)
  p[i][j] = NULL;


  //For each position alloc zlen dimension and set data to NULL.
  for (i=0; i < xlen; ++i)
  for (j=0; j < ylen; ++j)
  if ((p[i][j] = malloc(zlen * sizeof *p[i][j])) == NULL) {
    perror("malloc 3");
    free_data(p, xlen, ylen);
    return NULL;
  }

  return p;
}

struct Fragment* mainVoronoi(int numPoints){
  int pointsX[numPoints];// = {25,75,25,75,55,12,44,11,42,95,87};
  int pointsY[numPoints];// = {25,25,75,75,30,12,84,67,23,64,83};

  int xSize = 100;
  int ySize = 100;

  int** pointsOnHullX = my2DAllocation(xSize*ySize,numPoints);
  int** pointsOnHullY = my2DAllocation(xSize*ySize,numPoints);
  int** stackX = my2DAllocation(xSize*ySize,numPoints);
  int** stackY = my2DAllocation(xSize*ySize,numPoints);
  int *** bin = my3DAllocation(xSize,ySize,numPoints);

  int** firstPoints = my2DAllocation(2,numPoints);


  int stack = 0;
  bool firstPoint = true;

  int endPoint[2];
  bool notDone;

  GLfloat t;
  GLfloat conc = 5;
  for (int i = 0; i < numPoints; i++) {
    t = round((gennor(conc)*50)+50);
    while(t > 99 || t < 0 ){
      t = round((gennor(conc)*50)+50);
    }
    pointsX[i] = t;
    t = round((gennor(conc)*50)+50);
    while(t > 99 || t < 0 ){
      t = round((gennor(conc)*50)+50);
    }
    pointsY[i] = t;
  }

  int lastK  = 0;
  for (int i = 0; i < xSize; i++) {
    for (int j = 0; j < ySize; j++) {
      int minDist = 8000000;
      for (int k = 0; k < numPoints; k++) {
        bin[i][j][k] = 0;
        int tmpDist = pow((pointsX[k] - i),2) + pow((GLfloat)(pointsY[k]-j),2);
        if(tmpDist < minDist){
          minDist = tmpDist;
          bin[i][j][lastK] = 0;
          bin[i][j][k] = 1;
          lastK = k;
        }
      }
    }
  }



  //printf("First point and stacks \n");
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
          ////printf("%i %i %i\n", i, j, k);
          if(firstPoint){
            firstPoint = false;
            firstPoints[0][k] = i;
            firstPoints[1][k] = j;
            //printf("%i %i \n", i, j);
          }
        }
      }
    }
    stackX[0][k] = stack;
    stackY[0][k] = stack;
  }

  for (int k = 0; k < numPoints; k++) {
    //printf("%i\n", k);
    int i = 1;
    notDone = true;

    pointsOnHullX[1][k] = firstPoints[0][k];
    pointsOnHullY[1][k] = firstPoints[1][k];



    while(notDone){

      endPoint[0] = stackX[1][k];
      endPoint[1] = stackY[1][k];

      for (int j = 1; j < stackX[0][k]; j++) {
        ////printf("%i %i %i\n\n", stackX[j][k], stackY[j][k], k);
        if((endPoint[0] == pointsOnHullX[i][k]  && endPoint[1] == pointsOnHullY[i][k]) ||
        (leftOf(pointsOnHullX[i][k],pointsOnHullY[i][k],endPoint[0],endPoint[1],stackX[j][k],stackY[j][k],firstPoints[0][k],firstPoints[1][k])) ){
          //printf("%i %i %i\n", stackX[j][k], stackY[j][k], k);
          //printf("EndPoint: %i %i %i\n\n", endPoint[0], endPoint[1], k);
          endPoint[0] = stackX[j][k];
          endPoint[1] = stackY[j][k];
        }
      }

      ////printf("%i %i %i %i %i  \n", endPoint[0],endPoint[1], firstPoints[0][k], firstPoints[1][k], k);

      i = i + 1;
      pointsOnHullX[i][k] = endPoint[0];
      pointsOnHullY[i][k] = endPoint[1];

      if(endPoint[0] == firstPoints[0][k] && endPoint[1] ==  firstPoints[1][k]){
        pointsOnHullX[0][k] = i+1;
        notDone = false;
        //printf("%s \n", "Done-------------------------------------------------");
      }
    }
  }


  fragments = malloc(sizeof(struct Fragment)*numPoints);
  for (int k = 0; k < numPoints; k++) {
    fragments[k].numVertices = (GLint)pointsOnHullX[0][k];
    printf("numVerts: %i \n ", fragments[k].numVertices);
    fragments[k].numFragments = (GLint)numPoints;

    //Allocate the vertices
    GLfloat *tempVertices;
    tempVertices = malloc(sizeof(GLfloat)*pointsOnHullX[0][k]*3);


    tempVertices[0] = (GLfloat)pointsX[k]/50.0f-1;
    tempVertices[1] = (GLfloat)pointsY[k]/50.0f-1;
    tempVertices[2] = 0;

    GLuint *tempIndices;
    tempIndices = malloc(sizeof(GLuint)*(pointsOnHullX[0][k] -1)*3);

    GLfloat *tempTexCoord;
    tempTexCoord = malloc(sizeof(GLfloat)*pointsOnHullX[0][k]*2);

    tempTexCoord[0] = (GLfloat)pointsX[k]/100.0f;
    tempTexCoord[1] = (GLfloat)pointsY[k]/100.0f;

    for (int i = 1; i < pointsOnHullX[0][k]; i++) {
      tempVertices[i*3] = (GLfloat)pointsOnHullX[i][k]/50.0f-1;
      tempVertices[i*3+1] = (GLfloat)pointsOnHullY[i][k]/50.0f-1;
      tempVertices[i*3+2] = 0;

      tempIndices[(i-1)*3] = 0;
      tempIndices[(i-1)*3+1] = i;
      tempIndices[(i-1)*3+2] = i+1;

      tempTexCoord[i*2] = (GLfloat)pointsOnHullX[i][k]/100.0f;
      tempTexCoord[i*2+1] = (GLfloat)pointsOnHullX[i][k]/100.0f;
    }

    for (int i = 0; i < pointsOnHullX[0][k]-2; i++) {

      tempIndices[(i)*3] = 0;
      tempIndices[(i)*3+1] = i+1;
      tempIndices[(i)*3+2] = i+2;
    }

    fragments[k].vertices = tempVertices;
    fragments[k].indicies = tempIndices;
    fragments[k].texCoord = tempTexCoord;
  }

  //testFragments(fragments);
  free(pointsOnHullX);
  free(pointsOnHullY);
  free(stackX);
  free(stackY);
  free(bin);
  free(firstPoints);

  return fragments;
}

void testFragments(int k){
  struct Fragment curFrag = fragments[k];
  printf("%s\n", "-------------------------------------" );
  printf("%i \n",curFrag.numVertices);
  for (int i = 0; i < curFrag.numVertices; i++) {

    printf("Vertices: %f : %f : %f \n",((curFrag.vertices))[i*3 +0],((curFrag.vertices))[i*3 +1],((curFrag.vertices))[i*3 +2]);

  }
  for (int i = 0; i < curFrag.numVertices-2; i++) {
    printf("Indices %i : %i : %i \n",((curFrag.indicies))[i*3 +0],((curFrag.indicies))[i*3 +1],((curFrag.indicies))[i*3 +2]);
  }

}

GLfloat shatterObj(mat4 viewMatrix,GLfloat timeScale){
  vec3 origCenter = {0,0,0};

  timeScale += 0.001f;
  glUseProgram(objectProgram);

  mat4 trans = T(100.0f,2.0f,85.0f);

  glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, trans.m);
  glUniformMatrix4fv(glGetUniformLocation(objectProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

  if(firstTime){
    GLint numPoints = fragments[0].numFragments;
    models = malloc(numPoints*sizeof(Model*));
    dirs = malloc(numPoints*sizeof(vec3));
    firstTime = false; 

    for (int k = 0; k < fragments[0].numFragments; k++) {
      GLint verts = fragments[k].numVertices;
      models[k] = LoadDataToModel(
        (fragments[k].vertices), NULL, (fragments[k].texCoord), NULL,
        (fragments[k].indicies),verts, (verts-2)*3);
        vec3 curCenter = {((fragments[k].vertices))[0],((fragments[k].vertices))[1],((fragments[k].vertices))[2]};

        dirs[k] = VectorSub(curCenter,origCenter);

        vec3 dir = ScalarMult(dirs[k],timeScale);

        mat4 trans2 = Mult(T(dir.x,dir.y,dir.z),trans);

        glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, trans2.m);

        DrawModel(models[k],objectProgram,"inPosition",NULL,NULL);
      }
    }else{
      for (int k = 0; k < fragments[0].numFragments; k++) {
        vec3 dir = ScalarMult(dirs[k],timeScale);

        mat4 trans2 = Mult(T(dir.x,dir.y,dir.z),trans);

        glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, trans2.m);

        DrawModel(models[k],objectProgram,"inPosition",NULL,NULL);

      }
    }

    return timeScale;
  }
