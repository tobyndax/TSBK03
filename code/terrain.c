#include "terrain.h"

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
