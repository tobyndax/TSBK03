#include <iostream>
#include <q3.h>
#include <vector>
#include "program.h"
std::vector<q3Body*>* bodies;


using namespace std;
int main(){
  Program program;
  program.exec();
  return 0;
}

int testQ(){
  bodies = new vector<q3Body*>;
  q3Scene* scene = new q3Scene(1.0/60.0);

  q3BodyDef bodyDef;
  q3Body* body = scene->CreateBody( bodyDef );
  q3BoxDef boxDef;
  boxDef.SetRestitution( 0 );
  q3Transform tx;
  q3Identity( tx );
  boxDef.Set( tx, q3Vec3( 50.0f, 1.0f, 50.0f ) );
  body->AddBox( boxDef );

  bodyDef.bodyType = eDynamicBody;
  bodyDef.position.Set( 0, 5.0f, 0 );
  q3Body* body1 = scene->CreateBody( bodyDef );
  bodies->push_back(body1);

  bodyDef.bodyType = eDynamicBody;
  bodyDef.position.Set( 0, 10.0f, 0 );
  q3Body* body2 = scene->CreateBody( bodyDef );
  bodies->push_back(body2);

  for ( int i = 0; i < 2; ++i )
  {
    tx.position.Set(0,float(i)+1.0f,0); // this is in body coordinates. (i.e one box at 5.0f and one box at 6.0f)
    boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, 1.0f )); //the latter vector here is the exapansion of the box.
    body1->AddBox(boxDef);
  }

  for ( int i = 0; i < 2; ++i )
  {
    tx.position.Set(0,0,i); // this is in body coordinates. (i.e one box at 5.0f and one box at 6.0f)
    boxDef.Set( tx, q3Vec3( 1.0f, 1.0f, 1.0f )); //the latter vector here is the exapansion of the box.
    body2->AddBox(boxDef);
  }

  printf("%f %f %f \n",body->GetTransform().position.x,body->GetTransform().position.y, body->GetTransform().position.z);

  for (size_t i = 0; i < 250; i++) {

    scene->Step();
    printf("Pos: %f %f %f \n",body2->GetTransform().position.x,body2->GetTransform().position.y, body2->GetTransform().position.z);
    printf("Rot: %f %f %f \n",body2->GetTransform().rotation.ex.x,body2->GetTransform().rotation.ex.y, body2->GetTransform().rotation.ex.z);

  }


  return 0;
}
