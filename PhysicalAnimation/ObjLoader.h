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

typedef struct RigidBody {
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
  
  Vector3d center();
  
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


class StateVector {
public:  
  StateVector(int size, double const *data);
  explicit StateVector(int size);
  StateVector(StateVector& a);
  ~StateVector();
  
  int size() const {return this->size_;}
  double const *vector() {return this->vector_;}
  
  
  
  static StateVector& RigidBody_State_to_Array(ModelObject& obj, int size);
  
  
private:
  void init(int size, double const *data);
  int size_;
  double *vector_;
};

typedef struct MotionController{
  double current_time;
  double dt;
  ModelObject *object;
  MotionController(ModelObject* object_, double begin_time, double timestep) {
    current_time = begin_time;
    dt = timestep;
    object = object_;
  }

  void next_step(){
    Vector3d rot_axis = object->center() + Vector3d(1.0, 1.0, 1.0);
//    object->rotate(3, rot_axis);
    for (int i = 1 ; i < object->vertices.size(); i += 3 ) {
      object->vertices[i] -= 0.05;
    }
    object->make_array();
  }

  
} MotionController;

class ObjLoader{
public:
  static std::vector<ModelObject*> objects;
  //read object into an array, if file have problem, return NULL
  // return obj numbers and its obj
  static ModelObject* loadObject(char const *path, int &obj_no);
  
private:
  
};



#endif /* defined(__PhysicalAnimation__ObjLoader__) */
