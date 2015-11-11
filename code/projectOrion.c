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
#include "globals.h"
#include "terrain.h"
#include <math.h>
#include "voronoi.h"

bool shatter = false;

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
  printError("Init sky + terrain");
  //Move initially just to set everything up properly!

  /*
  squareModel = LoadDataToModel(
    square, NULL, squareTexCoord, NULL,
    squareIndices, 6, 12);
*/



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

    if(keyIsDown('h')){
      shatter = true;
    }

    if(camPosition.y < getHeight(camPosition.x,camPosition.z,&ttex)+1.0){
      camPosition.y = getHeight(camPosition.x,camPosition.z,&ttex)+1.0;
      upVector = SetVector(0.0,1.0,0.0);
      viewPoint = VectorAdd(camPosition,direction);
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

    printError("Program objects ERROR");
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

    glUseProgram(objectProgram);

    mat4 trans = T(100.0f,2.0f,90.0f);

    glUniformMatrix4fv(glGetUniformLocation(objectProgram, "mdlMatrix"), 1, GL_TRUE, trans.m);
    glUniformMatrix4fv(glGetUniformLocation(objectProgram, "viewMatrix"), 1, GL_TRUE, viewMatrix.m);

    DrawModel(squareModel,objectProgram,"inPosition",NULL,NULL);


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
      glutWarpPointer( centerX, centerY );
      #endif
    }
    printf("%d %d\n", deltax ,deltay);
    //printf("%d %d\n", deltax ,deltay);
  }

  void shatterObj(struct Fragment* fragments[],int k){

    struct Fragment* curFrag = fragments[k];
    squareModel = LoadDataToModel(
      *(curFrag->vertices), NULL, *(curFrag->texCoord), NULL,
      *(curFrag->indicies), 6, 12);
  }


  int main(int argc, char **argv){
    struct Fragment** fragments  = mainVoronoi();
    testFragments(fragments,1);
    //return 0;


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
    shatterObj(fragments,1);
    initKeymapManager();



    glutTimerFunc(20, &timer, 0);

    glutPassiveMotionFunc(mouse);

    glutMainLoop();
    exit(0);
  }
