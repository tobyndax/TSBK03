//void billify(Model *model);



#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

typedef struct
{
    Model *model;
    vec3 billNormal;
    mat4 billRot;
    Model *squareModel;
    
    GLfloat x;
    GLfloat y;
    
    FBOstruct* fbo;
    
    vec3 treePos;
    
}Tree;

Tree setupTree(Model* model);