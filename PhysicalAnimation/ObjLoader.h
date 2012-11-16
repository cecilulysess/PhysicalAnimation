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
  Matrix3x3 Ibody, Ibodyinv;
  
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
  
  void finished_loading();
private:
  //geometric center of this object
  Vector3d center_;
};


class StateVector : public Vector{
public:
  explicit StateVector(int Size, double *data = NULL) : Vector(Size, data) {}
  StateVector(const Vector& v);
  
  int size() { return this->getn(); }
  
  const StateVector& operator=(const StateVector& v2) {
    return (StateVector&) Vector::operator=(v2);
  }
  friend StateVector operator*(double s, const StateVector& v);
  
  // get the state vector from an object
  static StateVector& RigidBody_State_to_Array(ModelObject& obj);
  static void RigidBody_Array_to_state(ModelObject& obj, const StateVector& vector);
  inline static double sqr(double a) {
    return a*a;
  }
  
  inline static Matrix3x3& quaternion_to_matrix(Quaternion q) {
    Matrix3x3 *res = new Matrix3x3(
                                   // row 1
                                   1 - 2 * sqr(q.q.y) - 2 * sqr(q.q.z),
                                   2 * q.q.x * q.q.y - 2 * q.q.w * q.q.z,
                                   2 * q.q.x * q.q.z + 2 * q.q.w * q.q.y,
                                   // row 2
                                   2 * q.q.x * q.q.y + 2 * q.q.w * q.q.z,
                                   1 - 2 * sqr(q.q.x) - 2 * sqr(q.q.z),
                                   2 * q.q.y * q.q.z - 2 * q.q.w * q.q.x,
                                   // row 3
                                   2 * q.q.x * q.q.z - 2 * q.q.y,
                                   2 * q.q.y * q.q.z + 2 * q.q.w * q.q.x,
                                   1 - 2 * sqr(q.q.x) - 2 * sqr(q.q.y)
                                   );
    return *res;
  }
  
};

class MotionController{
public:
  double current_time;
  double dt;
  ModelObject *object;
  MotionController(ModelObject* object_, double begin_time, double timestep) {
    current_time = begin_time;
    dt = timestep;
    object = object_;
  }

  void next_step();
private:
  // compute the force F(t) and torque tau(t) acting on the rigid body at
  // time t
  void compute_force_and_torque(double t);
  
  // compute dy/dt
  void dydt(double t, StateVector& y, StateVector& ydot);
  
  // computing dy/dt and store it to ydot
  void ddt_State_to_Array(StateVector& ydot);
  
  inline Matrix3x3& Star(Vector3d a){
    Matrix3x3 *res = new Matrix3x3(
                  0, -a.z, a.y,
                  a.z, 0, -a.x,
                  -a.y, a.x, 0
                  );
    
    return *res;
  }

  
};

typedef void (MotionController::*dydt_func) (double t,
                StateVector& y, StateVector& ydot);
//receive an initial state vector y0, ode calculate the value using dydt
// function to get dy(t)/dt
void ODE(StateVector& y0, StateVector& yend,
         double t0, double dt, MotionController& controller, dydt_func dydt);


class ObjLoader{
public:
  static std::vector<ModelObject*> objects;
  //read object into an array, if file have problem, return NULL
  // return obj numbers and its obj
  static ModelObject* loadObject(char const *path, int &obj_no);
  
private:
  
};



#endif /* defined(__PhysicalAnimation__ObjLoader__) */
