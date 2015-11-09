#include "bill.h"
#include "globals.h"
#include <stdio.h>

Tree setupTree(Model* model){

  Tree tempTree;

  mat4 billRot = Rx(0);
  vec3 billNormal = SetVector(0,0,1);

  Model* sqMod = LoadDataToModel(square,
    NULL, squareTexCoord, NULL,squareIndices, 4, 6);

    FBOstruct* fbo1 = initFBO(W, H, 0);

    tempTree.billNormal = billNormal;
    tempTree.fbo = fbo1;
    tempTree.squareModel = sqMod;
    tempTree.billRot = billRot;
    tempTree.model = model;
    tempTree.x = 12;
    tempTree.y = 12.5;

    return tempTree;
  }
