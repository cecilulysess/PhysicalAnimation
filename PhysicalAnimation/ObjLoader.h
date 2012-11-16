//
//  ObjLoader.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/7/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef __PhysicalAnimation__ObjLoader__
#define __PhysicalAnimation__ObjLoader__
#ifndef GLfloat
	#define GLfloat float
#endif
#ifndef GLubyte
	#define GLubyte unsigned char
#endif
#define STATE_SIZE 18

#include <vector>
#include "Quaternion.h"
#include "Vector.h"

typedef struct RigidBody{
  double mass;
  Vector3d Ibody, Ibodyinv;
  
  //state var
  Vector3d x;
  Quaternion q;
  Vector3d P, L;
  
  //state var
  Matrix3x3 Iinv, R;
  Vector3d v, omega;
  Vector3d force, torque;
}RigidBody;

typedef struct MotionController{
  double current_time;
  double dt;
  
//  void simulate
} MotionController;

// a defined model
class ModelObject {
public:

  ModelObject(char const *obj_name);
  
  // get the data from vector so that opengl draw it correct
  void make_array();
  
  //print the vertices
  void print();
  
  // rotate this object along vector(x,y,z) for degree
  void rotate(float degree, float x, float y, float z) ;
  void rotate(float degree, Vector3d axis);
  
  Vector3d center() {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    long cnt = vertices.size();
    for (int i = 0 ; i < vertices.size(); i+=3) {
      x += vertices[i];
      y += vertices[i+1];
      z += vertices[i+2];
    }
    this->center_ = Vector3d(x/cnt, y/cnt, z/cnt);
    return center_;
  }
  
  std::vector<GLfloat> vertices;
  std::vector<GLubyte> indices;
  std::string objectName;
  
  double *body_array, *body_array_dot;
  
  GLfloat *vertices_array;
  GLubyte *indice_array;
  RigidBody rigid_body;
  
private:
  //geometric center of this object
  Vector3d center_;
};


class ObjLoader{
public:
  static std::vector<ModelObject*> objects;
  //read object into an array, if file have problem, return NULL
  // return obj numbers and its obj
  static ModelObject* loadObject(char const *path, int &obj_no);
  
private:
  
};



#endif /* defined(__PhysicalAnimation__ObjLoader__) */
