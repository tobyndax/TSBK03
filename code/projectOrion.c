//Project for TSBK07

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "mac/MicroGlut.h"
#include <ApplicationServices/ApplicationServices.h>
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "bill.h"
#include "globals.h"
#include <math.h>

vec3 billNormal;
mat4 billRot;

Model* GenerateTerrain(TextureData *tex){
    int vertexCount = tex->width * tex->height;
    int triangleCount = (tex->width-1) * (tex->height-1) * 2;
    int x, z;

    GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

    printf("bpp %d\n", tex->bpp);
    for (x = 0; x < tex->width; x++)
        for (z = 0; z < tex->height; z++)
        {
            // Vertex array. You need to scale this properly
            vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
            vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)];
            vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
            // Normal vectors. You need to calculate these.
            normalArray[(x + z * tex->width)*3 + 0] = 0.0;
            normalArray[(x + z * tex->width)*3 + 1] = 1.0;
            normalArray[(x + z * tex->width)*3 + 2] = 0.0;
            // Texture coordinates. You may want to scale them.
            if(vertexArray[(x + z * tex->width)*3 + 1]<=0){
                texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width;
                texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height;
            }else{
                texCoordArray[(x + z * tex->width)*2 + 0] = (float)x;
                texCoordArray[(x + z * tex->width)*2 + 1] = (float)z;
            }

        }
    for (x = 0; x < tex->width-1; x++)
        for (z = 0; z < tex->height-1; z++)
        {
            // Triangle 1
            indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
            indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
            indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
            // Triangle 2
            indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
            indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
            indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
        }
    //-----------------nonedge-normal calc----------------
    for (x = 1; x < tex->width-1; x++)
        for (z = 1; z < tex->height-1; z++)
        {
            vec3 v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);

            vec3 v1 = Normalize(SetVector(vertexArray[(x+1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + z * tex->width)*3 + 2]-v.z));
            vec3 v2 = Normalize(SetVector(vertexArray[(x+1 + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + (z-1) * tex->width)*3 + 2]-v.z));
            vec3 v3 = Normalize(SetVector(vertexArray[(x + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z-1) * tex->width)*3 + 2]-v.z));
            vec3 v4 = Normalize(SetVector(vertexArray[(x-1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + z * tex->width)*3 + 2]-v.z));
            vec3 v5 = Normalize(SetVector(vertexArray[(x-1 + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + (z+1) * tex->width)*3 + 2]-v.z));
            vec3 v6 = Normalize(SetVector(vertexArray[(x + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z+1) * tex->width)*3 + 2]-v.z));

            vec3 n1 = Normalize(CrossProduct(v1, v2));
            vec3 n2 = Normalize(CrossProduct(v2, v3));
            vec3 n3 = Normalize(CrossProduct(v3, v4));
            vec3 n4 = Normalize(CrossProduct(v4, v5));
            vec3 n5 = Normalize(CrossProduct(v5, v6));
            vec3 n6 = Normalize(CrossProduct(v6, v1));
            vec3 n = Normalize(VectorAdd(n1,VectorAdd(n2,VectorAdd(n3,VectorAdd(n4,VectorAdd(n5,n6))))));

            normalArray[(x + z * tex->width)*3 + 0] =n.x;
            normalArray[(x + z * tex->width)*3 + 1] =n.y;
            normalArray[(x + z * tex->width)*3 + 2] =n.z;
        }

    for (x = 1; x < tex->width-1; x++){
        z=0;
        vec3 v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);

        vec3 v1 = Normalize(SetVector(vertexArray[(x+1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + z * tex->width)*3 + 2]-v.z));
        vec3 v4 = Normalize(SetVector(vertexArray[(x-1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + z * tex->width)*3 + 2]-v.z));
        vec3 v5 = Normalize(SetVector(vertexArray[(x-1 + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + (z+1) * tex->width)*3 + 2]-v.z));
        vec3 v6 = Normalize(SetVector(vertexArray[(x + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z+1) * tex->width)*3 + 2]-v.z));

        vec3 n4 = Normalize(CrossProduct(v4, v5));
        vec3 n5 = Normalize(CrossProduct(v5, v6));
        vec3 n6 = Normalize(CrossProduct(v6, v1));
        vec3 n = Normalize(VectorAdd(n4,VectorAdd(n5,n6)));

        normalArray[(x + z * tex->width)*3 + 0] =n.x;
        normalArray[(x + z * tex->width)*3 + 1] =n.y;
        normalArray[(x + z * tex->width)*3 + 2] =n.z;
    }
    for (x = 1; x < tex->width-1; x++){
        z=tex->height-1;
        vec3 v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);
        vec3 v1 = Normalize(SetVector(vertexArray[(x+1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + z * tex->width)*3 + 2]-v.z));
        vec3 v2 = Normalize(SetVector(vertexArray[(x+1 + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + (z-1) * tex->width)*3 + 2]-v.z));
        vec3 v3 = Normalize(SetVector(vertexArray[(x + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z-1) * tex->width)*3 + 2]-v.z));
        vec3 v4 = Normalize(SetVector(vertexArray[(x-1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + z * tex->width)*3 + 2]-v.z));

        vec3 n1 = Normalize(CrossProduct(v1, v2));
        vec3 n2 = Normalize(CrossProduct(v2, v3));
        vec3 n3 = Normalize(CrossProduct(v3, v4));

        vec3 n = Normalize(VectorAdd(n1,VectorAdd(n2,n3)));

        normalArray[(x + z * tex->width)*3 + 0] =n.x;
        normalArray[(x + z * tex->width)*3 + 1] =n.y;
        normalArray[(x + z * tex->width)*3 + 2] =n.z;
    }
    for (z = 1; z < tex->height; z++){
        x=0;
        vec3 v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);
        vec3 v1 = Normalize(SetVector(vertexArray[(x+1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + z * tex->width)*3 + 2]-v.z));
        vec3 v2 = Normalize(SetVector(vertexArray[(x+1 + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + (z-1) * tex->width)*3 + 2]-v.z));
        vec3 v3 = Normalize(SetVector(vertexArray[(x + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z-1) * tex->width)*3 + 2]-v.z));
        vec3 v6 = Normalize(SetVector(vertexArray[(x + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z+1) * tex->width)*3 + 2]-v.z));


        vec3 n1 = Normalize(CrossProduct(v1, v2));
        vec3 n2 = Normalize(CrossProduct(v2, v3));
        vec3 n6 = Normalize(CrossProduct(v6, v1));


        vec3 n = Normalize(VectorAdd(n1,VectorAdd(n2,n6)));
        normalArray[(x + z * tex->width)*3 + 0] =n.x;
        normalArray[(x + z * tex->width)*3 + 1] =n.y;
        normalArray[(x + z * tex->width)*3 + 2] =n.z;
    }
    for (z = 1; z < tex->height-1; z++){
        x=tex->width-1;
        vec3 v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);

        vec3 v3 = Normalize(SetVector(vertexArray[(x + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z-1) * tex->width)*3 + 2]-v.z));
        vec3 v4 = Normalize(SetVector(vertexArray[(x-1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + z * tex->width)*3 + 2]-v.z));
        vec3 v5 = Normalize(SetVector(vertexArray[(x-1 + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + (z+1) * tex->width)*3 + 2]-v.z));
        vec3 v6 = Normalize(SetVector(vertexArray[(x + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z+1) * tex->width)*3 + 2]-v.z));

        vec3 n3 = Normalize(CrossProduct(v3, v4));
        vec3 n4 = Normalize(CrossProduct(v4, v5));
        vec3 n5 = Normalize(CrossProduct(v5, v6));
        vec3 n = Normalize(VectorAdd(n3,VectorAdd(n4,n5)));

        normalArray[(x + z * tex->width)*3 + 0] =n.x;
        normalArray[(x + z * tex->width)*3 + 1] =n.y;
        normalArray[(x + z * tex->width)*3 + 2] =n.z;
    }

    //--------------------code for first corner-------------------------
    x=0;
    z=0;
    vec3 v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);
    vec3 v1 = Normalize(SetVector(vertexArray[(x+1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + z * tex->width)*3 + 2]-v.z));
    vec3 v6 = Normalize(SetVector(vertexArray[(x + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z+1) * tex->width)*3 + 2]-v.z));

    vec3 n = Normalize(CrossProduct(v6, v1));
    normalArray[(x + z * tex->width)*3 + 0] =n.x;
    normalArray[(x + z * tex->width)*3 + 1] =n.y;
    normalArray[(x + z * tex->width)*3 + 2] =n.z;

    //--------------------------------code for second corner--------------------------------------
    x=tex->width-1;
    z=0;
    v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);
    vec3 v4 = Normalize(SetVector(vertexArray[(x-1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + z * tex->width)*3 + 2]-v.z));
    vec3 v5 = Normalize(SetVector(vertexArray[(x-1 + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + (z+1) * tex->width)*3 + 2]-v.z));
    v6 = Normalize(SetVector(vertexArray[(x + (z+1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z+1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z+1) * tex->width)*3 + 2]-v.z));

    vec3 n4 = Normalize(CrossProduct(v4, v5));
    vec3 n5 = Normalize(CrossProduct(v5, v6));
    n = Normalize(VectorAdd(n4,n5));
    normalArray[(x + z * tex->width)*3 + 0] =n.x;
    normalArray[(x + z * tex->width)*3 + 1] =n.y;
    normalArray[(x + z * tex->width)*3 + 2] =n.z;

    //------------------------------code for third corner--------------------------------
    x=tex->width-1;
    z=tex->height-1;

    v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);
    vec3 v3 = Normalize(SetVector(vertexArray[(x + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z-1) * tex->width)*3 + 2]-v.z));
    v4 = Normalize(SetVector(vertexArray[(x-1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x-1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x-1 + z * tex->width)*3 + 2]-v.z));

    n = Normalize(CrossProduct(v3, v4));
    normalArray[(x + z * tex->width)*3 + 0] =n.x;
    normalArray[(x + z * tex->width)*3 + 1] =n.y;
    normalArray[(x + z * tex->width)*3 + 2] =n.z;

    //---------------------------code for fourth corner--------------------
    x=0;
    z=tex->height-1;
    v=SetVector(vertexArray[(x + z * tex->width)*3 + 0],vertexArray[(x + z * tex->width)*3 + 1],vertexArray[(x + z * tex->width)*3 + 2]);
    v1 = Normalize(SetVector(vertexArray[(x+1 + z * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + z * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + z * tex->width)*3 + 2]-v.z));
    vec3 v2 = Normalize(SetVector(vertexArray[(x+1 + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x+1 + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x+1 + (z-1) * tex->width)*3 + 2]-v.z));
    v3 = Normalize(SetVector(vertexArray[(x + (z-1) * tex->width)*3 + 0]-v.x,vertexArray[(x + (z-1) * tex->width)*3 + 1]-v.y,vertexArray[(x + (z-1) * tex->width)*3 + 2]-v.z));

    vec3 n1 = Normalize(CrossProduct(v1, v2));
    vec3 n2 = Normalize(CrossProduct(v2, v3));
    n=Normalize(VectorAdd(n1,n2));

    normalArray[(x + z * tex->width)*3 + 0] =n.x;
    normalArray[(x + z * tex->width)*3 + 1] =n.y;
    normalArray[(x + z * tex->width)*3 + 2] =n.z;


    for (x = 0; x < tex->width; x++)
        for (z = 0; z < tex->height; z++)
        {
            vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
        }

    // End of terrain generation

    // Create Model and upload to GPU:

    Model* model = LoadDataToModel(
                                   vertexArray,
                                   normalArray,
                                   texCoordArray,
                                   NULL,
                                   indexArray,
                                   vertexCount,
                                   triangleCount*3);

    return model;
}

GLfloat getHeight(GLfloat x,GLfloat z,TextureData *tex){
    if(x>tex->width || z>tex->height || x<0||z<0){
        return 0.0;
    }
    int xmin = floor(x);
    int zmin = floor(z);
    int xmax = floor(x+1);
    int zmax = floor(z+1);
    GLfloat xPos = x-xmin;
    GLfloat zPos = z-zmin;
    //printf("%d %d\n", xmin, xmax);
    GLfloat yll = tex->imageData[(xmin + zmin * tex->width) * (tex->bpp/8)] / 10.0;//gives y as in GenerateTerrain
    GLfloat yhl = tex->imageData[(xmax + zmin * tex->width) * (tex->bpp/8)] / 10.0;
    GLfloat ylr = tex->imageData[(xmin + zmax * tex->width) * (tex->bpp/8)] / 10.0;
    GLfloat yhr = tex->imageData[(xmax + zmax * tex->width) * (tex->bpp/8)] / 10.0;

    vec3 phr = SetVector(xmax,yhr,zmax);	//points in square
    vec3 plr = SetVector(xmin,ylr,zmax);
    vec3 phl = SetVector(xmax,yhl,zmin);
    vec3 pll = SetVector(xmin,yll,zmin);

    vec3 normal;
    if((xPos+zPos)>=0.5){
        vec3 v1 = VectorSub(phl,phr);
        vec3 v2 = VectorSub(plr,phr);
        normal = Normalize(CrossProduct(v1,v2));
    }
    else{
        vec3 v2 = VectorSub(phl,pll);
        vec3 v1 = VectorSub(plr,pll);
        normal = Normalize(CrossProduct(v1,v2));
    }
    //lös höjden med planets eqvation
    GLfloat D = normal.x*phl.x+normal.y*phl.y+normal.z*phl.z;

    GLfloat y = (D-normal.x*x-normal.z*z)/normal.y;

    return y;
}

void initBillboard(){
    billboardProgram = loadShaders("plaintextureshader.vert", "plaintextureshader.frag");

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);

    glUseProgram(billboardProgram);

    glUniformMatrix4fv(glGetUniformLocation(billboardProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
}

void initSky(){

    skyProgram = loadShaders("sky.vert", "sky.frag");
    glUseProgram(skyProgram);


    glUniformMatrix4fv(glGetUniformLocation(skyProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    printError("pre skybox load ERROR");
    LoadTGATextureSimple("SkyBox512.tga", &skyTex);


    mat4 viewMatrix=lookAtv(camPosition,viewPoint, upVector);
    glUniformMatrix4fv(glGetUniformLocation(skyProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

    printError("pre skybox load ERROR2");

    //--------------------skybox------------------------------
    skybox = LoadModelPlus("skybox.obj",
                           skyProgram,
                           "inPosition",
                           "inNormal",
                           "inTexCoord");
    printError("skybox.obj load Error");
    glUseProgram(objectProgram);
}

void initLeaves(){

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);

    glUniform3fv(glGetUniformLocation(objectProgram, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(objectProgram, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1fv(glGetUniformLocation(objectProgram, "specularExponent"), 4, specularExponent);
    glUniform1iv(glGetUniformLocation(objectProgram, "isDirectional"), 4, isDirectional);

    // Load and compile shader
    leafProgram = loadShaders("leaf.vert", "leaf.frag");
    glUseProgram(leafProgram);
    printError("init leaf shader");

    glUniformMatrix4fv(glGetUniformLocation(leafProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    LoadTGATextureSimple("half_branch.tga", &leafTex);

    mat4 viewMatrix=lookAtv(camPosition,viewPoint, upVector);
    glUniformMatrix4fv(glGetUniformLocation(leafProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

    leaves = LoadModelPlus("leaf2.obj",leafProgram,"inPosition","inNormal","inTexCoord");
    printError("Model Loading leaves");

    leaves2 = LoadModelPlus("leaf2-2.obj",leafProgram,"inPosition","inNormal","inTexCoord");
    printError("Model Loading leaves");



    //--------------------LightSources-----------------------

    glUniform3fv(glGetUniformLocation(objectProgram, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(objectProgram, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1fv(glGetUniformLocation(objectProgram, "specularExponent"), 4, specularExponent);
    glUniform1iv(glGetUniformLocation(objectProgram, "isDirectional"), 4, isDirectional);


}

void initTerrain(){
    program = loadShaders("terrain4.vert", "terrain4.frag");
    glUseProgram(program);

    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    LoadTGATextureSimple("grass.tga", &tex1);
    LoadTGATextureSimple("poolwater.tga", &waterTex);
    LoadTGATextureSimple("snow.tga", &snowTex);
    LoadTGATextureSimple("dirt.tga", &dirtTex);

    mat4 viewMatrix=lookAtv(camPosition,viewPoint, upVector);
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

    //----------------terrain data -----------------------

    LoadTGATextureData("fft-terrain.tga", &ttex);
    tm = GenerateTerrain(&ttex);
    printError("init terrain");

    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

    glUseProgram(objectProgram);
}

void initCross(void){


    LoadTGATextureSimple("cross.tga", &crossTex);

    GLfloat orthProj[] =
    {1,0,0,0,
        0,1,0,0,
        0,0,0,0,
        0,0,0,50};

    crossProgram = loadShaders("cross.vert", "cross.frag");
    glUseProgram(crossProgram);

    cross = LoadModelPlus("cross.obj",crossProgram,"inPosition","inNormal","inTexCoord");

    glUniformMatrix4fv(glGetUniformLocation(crossProgram, "projMatrix"), 1, GL_TRUE,orthProj);


    mat4 total = IdentityMatrix();
    glUniformMatrix4fv(glGetUniformLocation(crossProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
    printError("Crosshair init");


}

void initGodzilla(mat4 viewMatrix){
	// Load and compile shader
    godzillaProgram = loadShaders("godzilla.vert", "godzilla.frag");
    glUseProgram(godzillaProgram);
    printError("init godzilla shader");
    glUniformMatrix4fv(glGetUniformLocation(godzillaProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(godzillaProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);
    printError("Model Loading Godzilla");
	//--------------------Godzilla-----------------------------
    Godzilla = LoadModelPlus("Godzilla/Godzilla.obj",godzillaProgram,"inPosition","inNormal","inTexCoord");
    GodzillaFight = LoadModelPlus("Godzilla/GodzillaKarate.obj",godzillaProgram,"inPosition","inNormal","inTexCoord");

    LoadTGATextureSimple("Godzilla/Godzilla_D.tga", &GodzillaTex);
    LoadTGATextureSimple("Godzilla/Godzilla_E.tga", &GlowGodzillaTex);

	glUseProgram(objectProgram);
	Hokmuto = LoadModelPlus("Godzilla/Hokmuto.obj",objectProgram,"inPosition","inNormal","inTexCoord");
    Femuto = LoadModelPlus("Godzilla/Femuto.obj",objectProgram,"inPosition","inNormal","inTexCoord");
    LoadTGATextureSimple("Godzilla/Hokmuto_D.tga", &HokmutoTex);
	LoadTGATextureSimple("Godzilla/Femuto_D.tga", &FemutoTex);

	glUseProgram(godzillaProgram);
    printError("Texture Loading Godzilla");
}

void init(void){
    // GL inits
#ifdef __APPLE__
    glutHideCursor();
#endif
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);


    // Load and compile shader
    objectProgram = loadShaders("object.vert", "object.frag");
    glUseProgram(objectProgram);
    printError("init shader");

    glUniformMatrix4fv(glGetUniformLocation(objectProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    LoadTGATextureSimple("bark.tga", &treeTex);

    viewPoint = SetVector(100.0f,0.0f,80.0f);
    camPosition = SetVector(100.0f,0.0f,90.0f);
    GLfloat y=getHeight(camPosition.x,camPosition.z,&ttex)+10;
    camPosition.y = y;
    viewPoint.y = y;
    upVector = SetVector(0.0f,1.0f,0.0f);

    mat4 viewMatrix=lookAtv(camPosition,viewPoint, upVector);
    glUniformMatrix4fv(glGetUniformLocation(objectProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);
    //--------------------Tree1------------------------------

    glUniform3fv(glGetUniformLocation(objectProgram, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(objectProgram, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1fv(glGetUniformLocation(objectProgram, "specularExponent"), 4, specularExponent);
    glUniform1iv(glGetUniformLocation(objectProgram, "isDirectional"), 4, isDirectional);

    initTerrain();
    initSky();
    initLeaves();
    initCross();
    printError("kaos --------------------");
    initGodzilla(viewMatrix);
    //Move initially just to set everything up properly!

    vec3 direction = VectorSub(viewPoint,camPosition);
    direction = ScalarMult(Normalize(direction),3);
    vec3 orthDir = CrossProduct(direction,upVector);
    orthDir = Normalize(orthDir);


    if((camPosition.x<=2 && DotProduct(SetVector(1.0,0.0,0.0), direction)>0) ||
       (camPosition.x>=254 && DotProduct(SetVector(1.0,0.0,0.0), direction)<0) ||
       (camPosition.z<=2 && DotProduct(SetVector(0.0,0.0,1.0), direction)>0) ||
       (camPosition.z>=254 && DotProduct(SetVector(0.0,0.0,1.0), direction)<0) ||
       (camPosition.x>2 && camPosition.x<254 && camPosition.z>2 && camPosition.z<254)){
        if(true){
            hasMoved = true;
            camPosition = VectorAdd(camPosition,ScalarMult(direction,0.05f));
            camPosition.y = getHeight(camPosition.x,camPosition.z,&ttex)+1.0;
            viewPoint = VectorAdd(camPosition,direction);
            //upVector = SetVector(0.0f,1.0f,0.0f);
        }
    }

    initBillboard();
}

void yaw(vec3* orthDir,vec3* direction,int deltax){
    mat4 latRot = Ry(0.005f*deltax);
    *direction=MultVec3(latRot,*direction);
    viewPoint = VectorAdd(camPosition,*direction);
    *orthDir = Normalize(CrossProduct(*direction,upVector));
    orthDir->y = 0;
    return;
}

void pitch(vec3* orthDir,vec3* direction,int deltay){
    mat4 latRot = ArbRotate(*orthDir, 0.005f*deltay);
    *direction=MultVec3(latRot,*direction);
    upVector=Normalize(CrossProduct(*orthDir,*direction));
    viewPoint = VectorAdd(camPosition,*direction);
    return;
}

mat4 keyInputs2(){
    //-----------------------key inputs-----------------------------------


    vec3 direction = VectorSub(viewPoint,camPosition);
    direction = ScalarMult(Normalize(direction),3);
    vec3 orthDir = CrossProduct(direction,upVector);
    orthDir = Normalize(orthDir);

    if(keyIsDown('k')){
        mat4 latRot = ArbRotate(orthDir, 0.05);
        direction=MultVec3(latRot,direction);
        upVector=MultVec3(latRot,upVector);
        viewPoint = VectorAdd(camPosition,direction);
    }
    if(keyIsDown('i')){
        mat4 latRot = ArbRotate(orthDir, -0.05);
        direction=MultVec3(latRot,direction);
        upVector=MultVec3(latRot,upVector);
        viewPoint = VectorAdd(camPosition,direction);
    }

    if((camPosition.x<=2 && DotProduct(SetVector(1.0,0.0,0.0), direction)>0) ||
       (camPosition.x>=254 && DotProduct(SetVector(1.0,0.0,0.0), direction)<0) ||
       (camPosition.z<=2 && DotProduct(SetVector(0.0,0.0,1.0), direction)>0) ||
       (camPosition.z>=254 && DotProduct(SetVector(0.0,0.0,1.0), direction)<0) ||
       (camPosition.x>2 && camPosition.x<254 && camPosition.z>2 && camPosition.z<254)){
        if(keyIsDown('w')){
            hasMoved = true;
            camPosition = VectorAdd(camPosition,ScalarMult(direction,0.05f));
            camPosition.y = getHeight(camPosition.x,camPosition.z,&ttex)+1.0;
            viewPoint = VectorAdd(camPosition,direction);
            //upVector = SetVector(0.0f,1.0f,0.0f);
        }
    }

    if((camPosition.x<=2 && DotProduct(SetVector(1.0,0.0,0.0), direction)<0) ||
       (camPosition.x>=254 && DotProduct(SetVector(1.0,0.0,0.0), direction)>0) ||
       (camPosition.z<=2 && DotProduct(SetVector(0.0,0.0,1.0), direction)<0) ||
       (camPosition.z>=254 && DotProduct(SetVector(0.0,0.0,1.0), direction)>0) ||
       (camPosition.x>2 && camPosition.x<254 && camPosition.z>2 && camPosition.z<254)){
        if(keyIsDown('s')){
            camPosition = VectorAdd(camPosition,ScalarMult(direction,-0.05f));
            camPosition.y = getHeight(camPosition.x,camPosition.z,&ttex)+1.0;
            viewPoint = VectorAdd(camPosition,direction);
            //upVector = SetVector(0.0f,1.0f,0.0f);
        }
    }

    if(keyIsDown('q')){
        mat4 latRot = ArbRotate(upVector, 0.1f);
        direction=MultVec3(latRot,direction);
        viewPoint = VectorAdd(camPosition,direction);
    }

    if(keyIsDown('e')){
        mat4 latRot = ArbRotate(upVector, -0.1f);
        direction=MultVec3(latRot,direction);
        viewPoint = VectorAdd(camPosition,direction);
    }
    if((camPosition.x<=2 && DotProduct(SetVector(0.0,0.0,1.0), direction)>0) ||
       (camPosition.x>=254 && DotProduct(SetVector(0.0,0.0,1.0), direction)<0) ||
       (camPosition.z<=2 && DotProduct(SetVector(1.0,0.0,0.0), direction)<0) ||
       (camPosition.z>=254 && DotProduct(SetVector(1.0,0.0,0.0), direction)>0) ||
       (camPosition.x>2 && camPosition.x<254 && camPosition.z>2 && camPosition.z<254)){
        if(keyIsDown('a')){
            camPosition = VectorAdd(camPosition,ScalarMult(orthDir,-0.05f));

            camPosition.y = getHeight(camPosition.x,camPosition.z,&ttex)+1.0;
            viewPoint = VectorAdd(camPosition,direction);
            //upVector = SetVector(0.0f,1.0f,0.0f);
        }
    }


    if((camPosition.x<=2 && DotProduct(SetVector(0.0,0.0,1.0), direction)<0) ||
       (camPosition.x>=254 && DotProduct(SetVector(0.0,0.0,1.0), direction)>0) ||
       (camPosition.z<=2 && DotProduct(SetVector(1.0,0.0,0.0), direction)>0) ||
       (camPosition.z>=254 && DotProduct(SetVector(1.0,0.0,0.0), direction)<0) ||
       (camPosition.x>2 && camPosition.x<254 && camPosition.z>2 && camPosition.z<254)){
        if(keyIsDown('d')){
            camPosition = VectorAdd(camPosition,ScalarMult(orthDir,0.05f));

            camPosition.y = getHeight(camPosition.x,camPosition.z,&ttex)+1.0;
            viewPoint = VectorAdd(camPosition,direction);
            //upVector = SetVector(0.0f,1.0f,0.0f);
        }
    }

    if(keyIsDown('j')){
        mat4 latRot = ArbRotate(direction, -0.1f);
        upVector=MultVec3(latRot,upVector);
    }
    if(keyIsDown('l')){
        mat4 latRot = ArbRotate(direction, 0.1f);
        upVector=MultVec3(latRot,upVector);
    }
    if(camPosition.y < getHeight(camPosition.x,camPosition.z,&ttex)+1.0){
        camPosition.y = getHeight(camPosition.x,camPosition.z,&ttex)+1.0;
        upVector = SetVector(0.0,1.0,0.0);
        viewPoint = VectorAdd(camPosition,direction);
    }

    if(keyIsDown('y')){
        firstBillboard = false;
    }
    if(keyIsDown('u')){
        firstBillboard = true;
    }
    if(keyIsDown('b')){
    	drawGodzilla = true;
		yGodzilla = -100.0f;
    }
    if(keyIsDown('n')){
    	drawGodzilla = false;
		yGodzilla = -100.0f;
    }
    if(keyIsDown('g')){
    	glowGodzilla = true;
    }

    if(keyIsDown('h')){
    	glowGodzilla = false;
    }


    yaw(&orthDir,&direction,deltax);
    pitch(&orthDir,&direction,-deltay);

    deltax = 0;
    deltay = 0;


    //printf("%f\n", getHeight(camPosition.x,camPosition.z,&ttex));
    //camPosition.y = getHeight(camPosition.x,camPosition.z,&ttex);
    //viewPoint.y = getHeight(viewPoint.x,viewPoint.z,&ttex);
    return lookAtv(camPosition,viewPoint, upVector);
}

void displaySkybox(mat4 viewMatrix){

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyTex);
    glUniform1i(glGetUniformLocation(skyProgram, "tex"), 0); // Texture unit 0


    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    mat4 trans = T(1.0f, -0.4f, 0.0f);
    viewMatrix.m[3] = 0;
    viewMatrix.m[7] = 0;
    viewMatrix.m[11] = 0;
    mat4 total = Mult(viewMatrix,trans);
    glUniformMatrix4fv(glGetUniformLocation(skyProgram, "mdlMatrix"), 1, GL_TRUE, total.m);

    glBindVertexArray(skybox->vao);    // Select VAO
    DrawModel(skybox,skyProgram, "inPosition", "inNormal", "inTexCoord");
    glEnable(GL_DEPTH_TEST);

    glDepthMask(GL_TRUE);
    printError("Skybox ERROR");

}

void displayTerrain(){

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, dirtTex);
    glUniform1i(glGetUniformLocation(program, "dirt"), 3); // Texture unit 3

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, snowTex);
    glUniform1i(glGetUniformLocation(program, "snow"), 2); // Texture unit 2

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, waterTex);
    glUniform1i(glGetUniformLocation(program, "water"), 1); // Texture unit 1

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glUniform1i(glGetUniformLocation(program, "grass"), 0); // Texture unit 0

    //-----------------light------------------------
    GLfloat camPos[] = {camPosition.x, camPosition.y, camPosition.z};
    glUniform3fv(glGetUniformLocation(program, "camPosition"), 1, camPos);

    //----------------Terrain-----------------------
    mat4 total = IdentityMatrix();
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

}

void displayObjects(mat4 viewMatrix){
    mat4 total, modelView, trans;

    //-----------------light------------------------
    GLfloat camPos[] = {camPosition.x, camPosition.y, camPosition.z};
    glUniform3fv(glGetUniformLocation(objectProgram, "camPosition"), 1, camPos);


    //----------------TreeTexture-------------------
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, treeTex);
    glUniform1i(glGetUniformLocation(objectProgram, "tex"), 0); // Texture unit 0


    /*
    //----------------LeafTexture-------------------
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, leafTex);
    glUniform1i(glGetUniformLocation(leafProgram, "texUnit"), 0); // Texture unit 0
    */
    printError("Program objects ERROR");
}

void displayLeaves(){

    mat4 total, modelView, trans;

    //-----------------light------------------------
    GLfloat camPos[] = {camPosition.x, camPosition.y, camPosition.z};
    glUniform3fv(glGetUniformLocation(leafProgram, "camPosition"), 1, camPos);


    //----------------TreeTexture-------------------
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, leafTex);
    glUniform1i(glGetUniformLocation(leafProgram, "texUnit"),0); // Texture unit 0

    //----------------Tree5-------------------------

    modelView = Rx(-M_PI/2);
    trans = T(25,getHeight(25.0f,25.0f,&ttex),25);
    total = Mult(trans,modelView);

    glUniformMatrix4fv(glGetUniformLocation(leafProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(leaves2, leafProgram, "inPosition", "inNormal", "inTexCoord");
    DrawModel(leaves2, leafProgram, "inPosition", "inNormal", "inTexCoord");



    printError("Leaf objects ERROR");

}

void crosshair(mat4 viewMatrix){

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glUseProgram(crossProgram);

    //----------------TreeTexture-------------------

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crossTex);
    glUniform1i(glGetUniformLocation(crossProgram, "texUnit"), 0); // Texture unit 0

    mat4 trans = T(1.0f, -0.4f, 0.0f);
    viewMatrix.m[3] = 0;
    viewMatrix.m[7] = 0;
    viewMatrix.m[11] = 0;
    mat4 total = Mult(viewMatrix,trans);

    total = Ry(0);
    printError("Crosshair display 1 ");
    glUniformMatrix4fv(glGetUniformLocation(crossProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
    printError("Crosshair display 2 ");

    DrawModel(cross, crossProgram, "inPosition", "inNormal", "inTexCoord");
    printError("Crosshair display 3 ");

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

}

void displayBillboard(mat4 viewMatrix){

    vec3 treePos = SetVector(100.0f,getHeight(100.0f,100.0f,&ttex),100.0f);
    vec3 camPos2 = camPosition;

    vec3 distance = VectorSub(treePos,camPos2);
    GLfloat d = sqrt(pow(distance.x,2)+pow(distance.z,2));

    if(d>30){

        vec3 zVector = SetVector(0,0,1);
        GLfloat zAlpha = acos(DotProduct(Normalize(zVector),Normalize(distance)));
        vec3 crossCheck = CrossProduct(zVector,distance);
        if(crossCheck.y<0){
            billRot = Ry(zAlpha);
        }else{
            billRot = Ry(-zAlpha);
        }

        vec3 distance = VectorSub(SetVector(treePos.x,0,treePos.z),SetVector(camPos2.x,0,camPos2.z));
        GLfloat alpha = acos(DotProduct(Normalize(billNormal),Normalize(distance)));

        if(alpha>(M_PI/24)){
            billNormal = MultVec3(billRot,zVector);
            printf("%f\n",alpha);

            if(firstBillboard && hasMoved){
                useFBO(fbo1, 0L, 0L);

                // Clear framebuffer & zbuffer
                glClearColor(0.0, 0.0, 0.0, -0.1);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate shader program
                glUseProgram(objectProgram);

                //----------------TreeTexture-------------------
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, treeTex);
                glUniform1i(glGetUniformLocation(objectProgram, "tex"), 0); // Texture unit 0

                //----------------Tree1-------------------------

                mat4 modelView = Rx(-M_PI/2);
                mat4 trans = T(treePos.x,treePos.y,treePos.z);
                mat4 total = Mult(trans,modelView);
                glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
                glBindVertexArray(tree1->vao);    // Select VAO

                //vec3 direction2 = VectorSub(treePos,camPos2);
                mat4 billViewMatrix=lookAtv(camPos2,SetVector(treePos.x,treePos.y+6,treePos.z), SetVector(0,1,0));
                glUniformMatrix4fv(glGetUniformLocation(objectProgram, "viewMatrix"), 1, GL_TRUE, billViewMatrix.m);

                //New Frustum!
                GLfloat znear = 0.1;
                GLfloat zfar = 500;
                GLfloat aspect = 1;


                //GLfloat d = sqrt(pow(distance.x,2) + pow(distance.z,2));

                GLfloat theta = atan(6/d)*2*180/M_PI;
                mat4 newProjection = perspective(theta,aspect,znear,zfar);

                glUniformMatrix4fv(glGetUniformLocation(objectProgram, "projMatrix"), 1, GL_TRUE, newProjection.m);


                // Enable Z-buffering
                glEnable(GL_DEPTH_TEST);
                // Enable backface culling
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);

                DrawModel(tree1, objectProgram, "inPosition", "inNormal", "inTexCoord");
                //firstBillboard = false;

                glUniformMatrix4fv(glGetUniformLocation(objectProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

                glUniformMatrix4fv(glGetUniformLocation(objectProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
            }
        }

        useFBO(0L, fbo1, 0L);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fbo1->texid);
        glUniform1i(glGetUniformLocation(objectProgram, "tex"), 0); // Texture unit 0
        //glClearColor(0.0, 0.0, 0.0, 0);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate second shader program
        glUseProgram(billboardProgram);

        glDisable(GL_CULL_FACE);

        //glDisable(GL_DEPTH_TEST);

        glUniformMatrix4fv(glGetUniformLocation(billboardProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

        mat4 trans = T(treePos.x,treePos.y,treePos.z);
        mat4 scale = S(12,12.5,0);
        mat4 total = Mult(trans,Mult(billRot,scale));
        glUniformMatrix4fv(glGetUniformLocation(billboardProgram, "mdlMatrix"), 1, GL_TRUE, total.m);


        DrawModel(squareModel, billboardProgram, "in_Position", NULL, "in_TexCoord");
    }else{
        glUseProgram(objectProgram);
        //----------------TreeTexture-------------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, treeTex);
        glUniform1i(glGetUniformLocation(objectProgram, "tex"), 0); // Texture unit 0

        //----------------Tree1-------------------------

        mat4 modelView = Rx(-M_PI/2);
        mat4 trans = T(treePos.x,treePos.y,treePos.z);
        mat4 total = Mult(trans,modelView);
        glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
        glBindVertexArray(tree1->vao);    // Select VAO

        DrawModel(tree1, objectProgram, "inPosition", "inNormal", "inTexCoord");
    }
    glFlush();

    printError("billboard ERROR");
}

void displaGodzilla(){

	mat4 scale,trans,total,modelView;


		//----------------GodzillaTexture-------------------
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GodzillaTex);
	glUniform1i(glGetUniformLocation(godzillaProgram, "tex"), 0); // Texture unit 0

	glActiveTexture(GL_TEXTURE1);
	if(glowGodzilla){
		glBindTexture(GL_TEXTURE_2D, GlowGodzillaTex);
	}else{
		glBindTexture(GL_TEXTURE_2D, GodzillaTex);
	}
	glUniform1i(glGetUniformLocation(godzillaProgram, "glowTex"), 1); // Texture unit 1
	//----------------Godzilla-------------------------

	GLfloat Sc = 0.5;
	if(!glowGodzilla){
		modelView = Ry(-M_PI/4);
		scale = S(Sc,Sc,Sc);
		trans = T(xGodzilla,yGodzilla,zGodzilla);
		total = Mult(trans,Mult(modelView,scale));
		glUniformMatrix4fv(glGetUniformLocation(godzillaProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
	    glBindVertexArray(Godzilla->vao);    // Select VAO
	    DrawModel(Godzilla, godzillaProgram, "inPosition", "inNormal", "inTexCoord");
	}

	else if(glowGodzilla){
		Sc = 0.5; //0.8 är bra för GodzillaFight
		modelView = Ry(-M_PI/4);
		scale = S(Sc,Sc,Sc);
		trans = T(xGodzilla,yGodzilla,zGodzilla);
		total = Mult(trans,Mult(modelView,scale));
    	glUniformMatrix4fv(glGetUniformLocation(godzillaProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
    	glBindVertexArray(GodzillaFight->vao);    // Select VAO
    	DrawModel(GodzillaFight, godzillaProgram, "inPosition", "inNormal", "inTexCoord");

    	glUseProgram(objectProgram);
    	//----------------Hokmuto---------------------------
    	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, HokmutoTex);
		glUniform1i(glGetUniformLocation(objectProgram, "tex"), 0); // Texture unit 0
	    GLfloat xH = 200.0;
	    GLfloat zH = 100.0;
	    modelView = Ry(M_PI/2);
		scale = S(0.2,0.2,0.2);
		trans = T(xH,getHeight(xH,zH,&ttex),zH);
		total = Mult(trans,Mult(modelView,scale));
		glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
	    glBindVertexArray(Hokmuto->vao);    // Select VAO
	    DrawModel(Hokmuto, objectProgram, "inPosition", "inNormal", "inTexCoord");

	    //---------------------Femuto--------------------
	    glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FemutoTex);

	    GLfloat xF = 100.0;
	    GLfloat zF = 200.0;
	    modelView = Ry(M_PI);
		scale = S(0.2,0.2,0.2);
		trans = T(xF,getHeight(xF,zF,&ttex),zF);
		total = Mult(trans,Mult(modelView,scale));
		glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, total.m);
	    glBindVertexArray(Femuto->vao);    // Select VAO
	    DrawModel(Femuto, objectProgram, "inPosition", "inNormal", "inTexCoord");
	}
	glUseProgram(godzillaProgram);

	if(yGodzilla<0){
    	yGodzilla = yGodzilla + 0.1;
    }


    printError("Display Godzilla");
}

void display(void){
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 viewMatrix=keyInputs2();
    glUseProgram(skyProgram);
    displaySkybox(viewMatrix);

    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);
    displayTerrain();

    glUseProgram(objectProgram);
    glUniformMatrix4fv(glGetUniformLocation(objectProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);
    displayObjects(viewMatrix);

    glUseProgram(godzillaProgram);
    if (drawGodzilla){
    	glUniformMatrix4fv(glGetUniformLocation(godzillaProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);
    	displaGodzilla();
	}

    glUseProgram(leafProgram);
    glUniformMatrix4fv(glGetUniformLocation(leafProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

    glutSwapBuffers();
    crosshair(viewMatrix);

    printError("display ERROR");

    glutSwapBuffers();
}

void timer(int i)
{
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
}

void mouse(int x, int y)
{
    static int lastx = 0.0;
    static int lasty = 0.0;
    float centerX = 200;
    float centerY = 200;

#ifdef __APPLE__
    centerX = glutGet(GLUT_WINDOW_WIDTH)/2;
    centerY = glutGet(GLUT_WINDOW_HEIGHT)/2;
#endif

    lastx = (float)x - lastx;
    lasty = (float)y - lasty;

    deltax = lastx;
    deltay = lasty;

    if((abs((int)lastx)>50) || (abs((int)lasty)>50))
    {
        deltax = 0;
        deltay = 0;

        lastx = (float)x;
        lasty = (float)y;

        return;

    }
    /*Fix for quartz issue found at http://stackoverflow.com/questions/10196603/using-cgeventsourcesetlocaleventssuppressioninterval-instead-of-the-deprecated/17547015#17547015
     */
    // if mouse wander off too much, warp it back.
    float dist = 100;
    if(x > centerX+dist || x < centerX-dist || y < centerY+dist || y > centerY-dist){
#ifdef __APPLE__
        CGPoint warpPoint = CGPointMake(centerX,centerY);
        CGWarpMouseCursorPosition(warpPoint);
        CGAssociateMouseAndMouseCursorPosition(true);
#endif
#ifndef __APPLE__
        glutWarpPointer( centerX, centerY );
#endif
    }
    printf("%d %d\n", deltax ,deltay);
    //printf("%d %d\n", deltax ,deltay);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize (600, 600);
    glutCreateWindow ("Project Orion");
    glutDisplayFunc(display);
#ifdef __APPLE__
    glutFullScreen();
#endif
    init ();
    initKeymapManager();
    glutTimerFunc(20, &timer, 0);

    glutPassiveMotionFunc(mouse);

    glutMainLoop();
    exit(0);
}
