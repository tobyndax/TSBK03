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

struct PhysicsObj* objs;

bool firstTime = true;

void initObj(){
  int numObjs = fragments[0].numFragments;
  objs = malloc(sizeof(struct PhysicsObj)*numObjs);
  float cBallSize = 0.1;
  for (int i = 0; i < numObjs; i++)
  {
    objs[i].mass = 1.0;
    objs[i].Pos = SetVector((fragments[i].vertices)[0]+100.0f, (fragments[i].vertices)[1]+2.0f, (fragments[i].vertices[2])+85.0f);
    objs[i].LinMom = SetVector(0.0, 0.0, 0.0);
    objs[i].Rot = IdentityMatrix();
    float s = pow(cBallSize,2)*objs[i].mass/3;
    objs[i].I = mat4tomat3(S(s,s,s));
  }
  GLint numPoints = fragments[0].numFragments;
  models = malloc(numPoints*sizeof(Model*));
  firstTime = false;

  for (int k = 0; k < fragments[0].numFragments; k++) {
    GLint verts = fragments[k].numVertices;
    models[k] = LoadDataToModel(
      (fragments[k].vertices),fragments[k].normals, (fragments[k].texCoord), NULL,
      (fragments[k].indicies),verts, verts);
      vec3 curCenter = {((fragments[k].vertices))[0],((fragments[k].vertices))[1],((fragments[k].vertices))[2]};
    }
  }

  void updateWorld(double dT)
  {
    dT = 0.01f;
    int numObjs = fragments[0].numFragments;
    if(firstTime){
      initObj();
    }
    // Zero forces
    int i, j;
    for (i = 0; i < numObjs; i++)
    {
      objs[i].F = SetVector(0,-9.82f*objs[i].mass,0);
      objs[i].T = SetVector(0,0,0);
    }

    // Wall tests
    for (i = 0; i < numObjs; i++)
    {
      if (objs[i].Pos.y <= 0.0f + 0.4f){
        objs[i].LinMom.y = 0.5*abs(objs[i].LinMom.y);
      }

      objs[i].v = ScalarMult(objs[i].LinMom, 1.0/(objs[i].mass));
    }

    // Update state, follows the book closely
    for (i = 0; i < numObjs; i++)
    {
      vec3 dPos, dLinMom, dAngMom, dO;
      mat4 Rd;
      vec3 up = SetVector(0,1,0);
      // Note: omega is not set. How do you calculate it?
      vec3 rot_axis = CrossProduct(up, objs[i].LinMom);
      //ball[i].omega = ScalarMult(rot_axis, 1.0/(ball[i].mass*kBallSize));


      //		v := P * 1/mass
      objs[i].v = ScalarMult(objs[i].LinMom, 1.0/(objs[i].mass));

      //		X := X + v*dT
      dPos = ScalarMult(objs[i].v, dT); // dX := v*dT
      objs[i].Pos = VectorAdd(objs[i].Pos, dPos); // X := X + dX
      //		R := R + Rd*dT
      dO = ScalarMult(objs[i].omega, dT); // dO := omega*dT
      Rd = CrossMatrix(dO); // Calc dO, add to R
      Rd = Mult(Rd, objs[i].Rot); // Rotate the diff (NOTE: This was missing in early versions.)
      objs[i].Rot = MatrixAdd(objs[i].Rot, Rd);
      //		P := P + F * dT
      dLinMom = ScalarMult(objs[i].F, dT); // dP := F*dT
      objs[i].LinMom = VectorAdd(objs[i].LinMom, dLinMom); // P := P + dP
      //		L := L + t * dT
      dAngMom = ScalarMult(objs[i].T, dT); // dL := T*dT
      objs[i].AngMom = VectorAdd(objs[i].AngMom, dAngMom); // L := L + dL

      OrthoNormalizeMatrix(&objs[i].Rot);
    }
  }

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

    srand(25);

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

      GLfloat depth = 0.05f;
      fragments[k].numVertices = (GLint)((pointsOnHullX[0][k]-1)*12);

      fragments[k].numFragments = (GLint)numPoints;

      fragments[k].numIndices = fragments[k].numVertices;

      //Allocate the vertices
      GLfloat *tempVertices;
      tempVertices = malloc(sizeof(GLfloat)*fragments[k].numVertices*3);

      GLfloat *tempNormal;
      tempNormal = malloc(sizeof(GLfloat)*fragments[k].numVertices*3);

      GLuint *tempIndices;
      tempIndices = malloc(sizeof(GLuint)*(fragments[k].numIndices));

      int str = 12*3; // 12*3 triangle point stride;
      int str2 = 12;
      for (int i = 1; i < fragments[k].numVertices/12; i++) {
        //triangle 1
        tempVertices[(i-1)*str +0] = pointsX[k]/50.0f-1;
        tempVertices[(i-1)*str +1] = pointsY[k]/50.0f-1;
        tempVertices[(i-1)*str +2] = 0;

        tempVertices[(i-1)*str +3] = pointsOnHullX[i][k]/50.0f-1;
        tempVertices[(i-1)*str +4] = pointsOnHullY[i][k]/50.0f-1;
        tempVertices[(i-1)*str +5] = 0;

        tempVertices[(i-1)*str +6] = pointsOnHullX[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str +7] = pointsOnHullY[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str +8] = 0;

        //triangle 2
        tempVertices[(i-1)*str+9 +0] = pointsOnHullX[i][k]/50.0f-1;
        tempVertices[(i-1)*str+9 +1] = pointsOnHullY[i][k]/50.0f-1;
        tempVertices[(i-1)*str+9 +2] = 0;

        tempVertices[(i-1)*str+9 +3] = pointsOnHullX[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str+9 +4] = pointsOnHullY[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str+9 +5] = 0;

        tempVertices[(i-1)*str+9 +6] = pointsOnHullX[i][k]/50.0f-1;
        tempVertices[(i-1)*str+9 +7] = pointsOnHullY[i][k]/50.0f-1;
        tempVertices[(i-1)*str+9 +8] = depth;

        //triangle 3
        tempVertices[(i-1)*str+18 +3] = pointsOnHullX[i][k]/50.0f-1;
        tempVertices[(i-1)*str+18 +4] = pointsOnHullY[i][k]/50.0f-1;
        tempVertices[(i-1)*str+18 +5] = depth;

        tempVertices[(i-1)*str+18 +0] = pointsOnHullX[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str+18 +1] = pointsOnHullY[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str+18 +2] = depth;

        tempVertices[(i-1)*str+18 +6] = pointsOnHullX[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str+18 +7] = pointsOnHullY[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str+18 +8] = 0;

        //triangle 4
        tempVertices[(i-1)*str+27 +0] = pointsOnHullX[i][k]/50.0f-1;
        tempVertices[(i-1)*str+27 +1] = pointsOnHullY[i][k]/50.0f-1;
        tempVertices[(i-1)*str+27 +2] = depth;

        tempVertices[(i-1)*str+27 +6] = pointsOnHullX[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str+27 +7] = pointsOnHullY[i+1][k]/50.0f-1;
        tempVertices[(i-1)*str+27 +8] = depth;

        tempVertices[(i-1)*str+27 +3] = pointsX[k]/50.0f-1;
        tempVertices[(i-1)*str+27 +4] = pointsY[k]/50.0f-1;
        tempVertices[(i-1)*str+27 +5] = depth;
      }

      for (int i = 0; i < fragments[k].numVertices; i++) {
        tempIndices[i] = i;
      }


      for (int i = 0; i < fragments[k].numVertices/3; i++) {
        vec3 vert1 =SetVector( tempVertices[tempIndices[i*3+0]*3+0],tempVertices[tempIndices[i*3+0]*3+1],tempVertices[tempIndices[i*3+0]*3+2]);
        vec3 vert2 =SetVector( tempVertices[tempIndices[i*3+1]*3+0],tempVertices[tempIndices[i*3+1]*3+1],tempVertices[tempIndices[i*3+1]*3+2]);
        vec3 vert3 =SetVector( tempVertices[tempIndices[i*3+2]*3+0],tempVertices[tempIndices[i*3+2]*3+1],tempVertices[tempIndices[i*3+2]*3+2]);

        vec3 d1 = VectorSub(vert2,vert1);
        vec3 d2 = VectorSub(vert3,vert1);
        vec3 normal = Normalize(CrossProduct(d2,d1));

        tempNormal[tempIndices[i*3+0]*3+0] = normal.x;
        tempNormal[tempIndices[i*3+0]*3+1] = normal.y;
        tempNormal[tempIndices[i*3+0]*3+2] = normal.z;

        tempNormal[tempIndices[i*3+1]*3+0] = normal.x;
        tempNormal[tempIndices[i*3+1]*3+1] = normal.y;
        tempNormal[tempIndices[i*3+1]*3+2] = normal.z;

        tempNormal[tempIndices[i*3+2]*3+0] = normal.x;
        tempNormal[tempIndices[i*3+2]*3+1] = normal.y;
        tempNormal[tempIndices[i*3+2]*3+2] = normal.z;

      }

      fragments[k].vertices = tempVertices;
      fragments[k].indicies = tempIndices;
      fragments[k].normals = tempNormal;
    }

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
    for (int i = 0; i < curFrag.numVertices/3; i++) {
      printf("Vertices: %f : %f : %f \n",((curFrag.vertices))[i*3 +0],((curFrag.vertices))[i*3 +1],((curFrag.vertices))[i*3 +2]);
    }
    for (int i = 0; i < curFrag.numIndices/3; i++) {
      printf("Indices %i : %i : %i \n",((curFrag.indicies))[i*3 +0],((curFrag.indicies))[i*3 +1],((curFrag.indicies))[i*3 +2]);
    }

  }

  GLfloat shatterObj(mat4 viewMatrix,GLfloat timeScale){
    glUseProgram(objectProgram);
    glUniformMatrix4fv(glGetUniformLocation(objectProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

    for (int k = 0; k < fragments[0].numFragments; k++) {
      mat4 trans2 = T(objs[k].Pos.x,objs[k].Pos.y,objs[k].Pos.z);

      glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, trans2.m);

      DrawModel(models[k],objectProgram,"inPosition",NULL,NULL);

    }

    return timeScale;
  }
