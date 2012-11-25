//
//  ObjLoader.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/7/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#include "ObjLoader.h"
#include "Quaternion.h"
#include <stdio.h>
#include "Matrix.h"
std::vector<ModelObject*> ObjLoader::objects;
#define KMASS 0.1
#define pho 0.5

ModelObject* ObjLoader::loadObject(char const *path, int &obj_no){
  FILE *file ;
  if(!(file= fopen(path, "r"))){
    return NULL;
  }
  char identifier[255];
  char objectname[255];
  float value1, value2, value3;
  int v1, vt1, v2, vt2, v3, vt3, v4, vt4;
  
  ModelObject*  objpt;
  fscanf(file, "%s", identifier);
  int i = 0, off = -1;
  bool have_vt = false;
  while( !feof(file) ){
//    printf("%d: ", i);
    i++;
    switch (identifier[0]) {
        case '#':
//        printf("#\n");
        fgets(identifier, 100, file);
        break;
        
        case 'o':
        fscanf(file, "%s", objectname);
        objpt = new ModelObject(objectname);
        objects.push_back(objpt);
        printf("\tRead Object:%s\n", objectname);
        obj_no ++;
        fgets(identifier, 100, file);
        have_vt = false;
        break;
        
        case 'v':
        if (identifier[1] == ' ') {
          fscanf(file, "%f %f %f", &value1, &value2, &value3 );
          objpt->vertices.push_back(value1);
          objpt->vertices.push_back(value2);
          objpt->vertices.push_back(value3);
        } else {
//          printf("vt\n");
          // it is vt
        }
        fgets(identifier, 100, file);
        break;
        
        case 'u':
//        printf("u\n");
        // texture image?
        fgets(identifier, 100, file);
        break;
        case 's':
//        printf("s\n");
        have_vt = true;
        fgets(identifier, 100, file);
        // skip s line
        break;
        case 'f':
        //load face indice
//        printf("f\n");
        if(have_vt) {
          fscanf(file, "%d/%d %d/%d %d/%d %d/%d",
               &v1, &vt1, &v2, &vt2, &v3, &vt3, &v4, &vt4);
        } else {
          
        }
        
        objpt->indices.push_back(v1 + off);
        objpt->indices.push_back(v2 + off);
        objpt->indices.push_back(v3 + off);
        objpt->indices.push_back(v4 + off);
        break;
        case 'm':
//        printf("m\n");
        fgets(identifier, 100, file);
        break;
    }
    fgets(identifier, 3, file);
  }
  for(int i = 0 ; i < objects.size(); ++i ) {
    objects[i]->make_array();
    objects[i]->print();
    objects[i]->finished_loading();
  }
  
  
  
  return objects[0];
}

void ModelObject::finished_loading(){
  RigidBody& rb = this->rigid_body;
  rb.mass = KMASS;
  rb.x = this->center();
  rb.q = Quaternion();
  rb.P = Vector3d(0,0,0);
  rb.L = Vector3d(0,0,0);
}

//======================Model Object=============================
void ModelObject::rotate(float degree, float x, float y, float z) {
  Quaternion qt(degree, x, y, z);
  for (int i = 0 ; i < vertices.size(); i += 3) {
    Vector3d v(vertices[i], vertices[i+1], vertices[i+2]);
    Quaternion qout = qt * Quaternion(v) * qt.inv();
    Vector3d vout = Vector3d(qout);
    vertices[i] = vout.x;
    vertices[i+1] = vout.y;
    vertices[i+2] = vout.z;
  }
  make_array();
}

void ModelObject::rotate(float degree, Vector3d axis){
  rotate(degree, axis.x, axis.y, axis.z);
}

ModelObject::ModelObject(char const *obj_name){
  objectName = std::string(obj_name);
  body_array = (double *)malloc(sizeof(double) * STATE_SIZE);
  body_array_dot = (double *)malloc(sizeof(double) * STATE_SIZE);
}

void ModelObject::print(){
  std::cout<<"Object Name: "<<this->objectName<<", vertices #: " <<
  this->vertices.size()<<", indice #:"<<this->indices.size()<<std::endl;
  for(int i = 0; i < vertices.size(); i+=3){
    printf("v %f %f %f\n", vertices_array[i], vertices_array[i+1], vertices_array[i+2]);
  }
  for(int i = 0; i < indices.size(); i+=4){
    printf("f %d %d %d %d\n", indice_array[i], indice_array[i+1], indice_array[i+2], indice_array[i+3]);
  }
}

void ModelObject::make_array(){
  this->vertices_array = (GLfloat*) malloc(sizeof(GLfloat) * vertices.size());
  this->indice_array =  (GLubyte*) malloc(sizeof(GLubyte) * indices.size());
  for (int i = 0; i < vertices.size(); ++i) {
    vertices_array[i] = vertices[i];
  }
  for (int i = 0; i < indices.size(); ++i) {
    indice_array[i] = indices[i];
  }
}

Vector3d ModelObject::center() {
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

void ModelObject::update_object() {
  Vector3d newC = this->rigid_body.x, tran = newC - this->center_;
  this->center_ = newC;
  Vector3d omega = this->rigid_body.omega;
  Quaternion q = this->rigid_body.q;//(omega.norm(), omega.x, omega.y, omega.z);
//  printf("\tOmega: %f %f, %f\n", omega.x, omega.y, omega.z);
  for (int i = 0; i < vertices.size(); i+=3) {
    Vector3d v(vertices[i], vertices[i+1], vertices[i+2]);
    Quaternion qout = q * Quaternion(v) * q.inv();
    Vector3d vout = Vector3d(qout);
    vertices[i] = vout.x;
    vertices[i+1] = vout.y;
    vertices[i+2] = vout.z;
  }
  
  for (int i = 0 ; i < this->vertices.size(); i+=3) {
    this->vertices[i] += tran.x;
    this->vertices[i+1] += tran.y;
    this->vertices[i+2]  += tran.z;
  }

}

//=======================ODE============================

//receive an initial state vector y0, ode calculate the value using dydt
// function to get dy(t)/dt
void ODE(StateVector& y0, StateVector& yend,
         double t0, double dt, MotionController& controller, dydt_func dydt){
  StateVector K1(y0), K2(y0), K3(y0), K4(y0);
  
  (controller.*dydt)(t0, y0, K1);
  
  K1 = K1 * dt;
  StateVector sv = y0 + 0.5 * K1;
  (controller.*dydt)(t0 + dt * 0.5, sv, K2);
  
  K2 = K2 * dt;
  
  sv = y0 + 0.5 * K2;
  (controller.*dydt)(t0 + dt * 0.5, sv, K3);
  K3 = K3 * dt;
  
  sv = y0 + K3;
  (controller.*dydt)(t0 + dt, sv, K4);
  
//  K2 = calculate_dynamics(X + K1 * 0.5, t + 0.5 * dt) * dt;
//  K3 = calculate_dynamics(X + K2 * 0.5, t + 0.5 * dt) * dt;
//  K4 = calculate_dynamics(X + K3, t + dt) * dt;
  yend = y0 +(K1 + K2 * 2 + K3 * 2 + K4) * (1.0 / 6.0);
  
  
  
}

//===================MotionController==================
void MotionController::next_step() {
  
  StateVector& y0 = StateVector::RigidBody_State_to_Array(*object);
//  printf("Y0:\n");
//  y0.print();
  // create a tmp statevector to receive the result
  StateVector yend = StateVector(y0);
  
  // calculate the result by ode
  ODE(y0, yend, current_time, dt, *this, &MotionController::dydt);
//  if (is_collide ){
//    Vector3d norm(0, 1.0f, 0);
//    Vector3d P = collide_pt;
//    printf("\tFind min pt %f, %f, %f\n",P.x, P.y, P.z);
//    Vector3d R = P - object->center(), r0 = R.normalize();
//    double Vreln = P * norm, numerator = Vreln * -(1 - pho);
//    double denominator = ( norm * object->rigid_body.Iinv *
//                          ((R%norm)%R ) + 1.0 / object->rigid_body.mass );
//    Vector3d J = 1/ numerator *  denominator * norm;
//    object->rigid_body.force = object->rigid_body.force + pho * J;
//    ODE(y0, yend, current_time, dt, *this, &MotionController::dydt);
//    is_collide = false;
//  }
//  printf("Yend:\n");
//  yend.print();
  // re assign the state vector back to the original object
  StateVector::RigidBody_Array_to_state(*object, yend);
  // update the model's vertice so that it can showed in the scene
  object->update_object();
  object->make_array();
}

void MotionController::compute_force_and_torque(double t) {
  object->rigid_body.force = Vector3d(0.0f, -0.098f, 0.0f);
  double min = 100000.0;
  int idx = 0;
  //find minimium y
  for (int i = 1; i < object->vertices.size();  i+=3 ) {
    if (object->vertices[i] < min) {
      min = object->vertices[i];
      idx = i/3;
    }
  }
  if (min < 0.1) {
    is_collide = true;
    collide_pt = Vector3d(object->vertices[idx*3],
                          object->vertices[idx*3+1],
                          object->vertices[idx*3+2]);
    printf("Detected Collid: %f, %f, %f\n", collide_pt.x,
           collide_pt.y,
           collide_pt.z);
  } 
  if( is_collide ) {
    object->rigid_body.force = Vector3d(0, 1, 0);
    
  }

  
}

void MotionController::dydt(double t, StateVector& y, StateVector& ydot) {
  //for each rigid body
  compute_force_and_torque(t);
  ddt_State_to_Array(ydot);
}

void MotionController::ddt_State_to_Array(StateVector& ydot){
  int idx = 0;
  RigidBody& rb = this->object->rigid_body;
  ydot[idx++] = rb.v[0];
  ydot[idx++] = rb.v[1];
  ydot[idx++] = rb.v[2];
  
  Quaternion qdot = 0.5 * (rb.omega * rb.q);
  ydot[idx++] = qdot.q.x;
  ydot[idx++] = qdot.q.y;
  ydot[idx++] = qdot.q.z;
  ydot[idx++] = qdot.q.w;
  //assuming that the multiplication between rb.omega and rb.q defined to
  //return the quateronion product [0, rb->omega] * q
//----------------may cause problem--------------------------------------------------
  
  ydot[idx++] = rb.force[0];
  ydot[idx++] = rb.force[1];
  ydot[idx++] = rb.force[2]; 
  
  ydot[idx++] = rb.torque[0];
  ydot[idx++] = rb.torque[1];
  ydot[idx++] = rb.torque[2];
}



//===================StateVector=======================
StateVector::StateVector(const Vector& v) : Vector(v){
  //do nothing
}

StateVector& StateVector::RigidBody_State_to_Array(ModelObject& obj){
  StateVector *ss = new StateVector(13);
  StateVector& res = *ss;
  int idx = 0;
  res[idx] = 1.0;
  res[idx++] = obj.rigid_body.x.x;
  res[idx++] = obj.rigid_body.x.y;
  res[idx++] = obj.rigid_body.x.z;
  
//  using rotation matrix
//  for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      res[idx++] = obj.rigid_body.R[i][j];
//    }
//  }
  
  // using quaternion
  res[idx++] = obj.rigid_body.q.q.x;
  res[idx++] = obj.rigid_body.q.q.y;
  res[idx++] = obj.rigid_body.q.q.z;
  res[idx++] = obj.rigid_body.q.q.w;
  
  res[idx++] = obj.rigid_body.P[0];
  res[idx++] = obj.rigid_body.P[1];
  res[idx++] = obj.rigid_body.P[2];
  
  res[idx++] = obj.rigid_body.L[0];
  res[idx++] = obj.rigid_body.L[1];
  res[idx++] = obj.rigid_body.L[2];
  
  return res;
}

void StateVector::RigidBody_Array_to_state(ModelObject &obj, const StateVector &vector){
  int idx = 0;
  obj.rigid_body.x[0] = vector[idx++];
  obj.rigid_body.x[1] = vector[idx++];
  obj.rigid_body.x[2] = vector[idx++];
  
  obj.rigid_body.q.q[0] = vector[idx++];
  obj.rigid_body.q.q[1] = vector[idx++];
  obj.rigid_body.q.q[2] = vector[idx++];
  obj.rigid_body.q.q[3] = vector[idx++];

  obj.rigid_body.P[0] = vector[idx++];
  obj.rigid_body.P[1] = vector[idx++];
  obj.rigid_body.P[2] = vector[idx++];
  
  obj.rigid_body.L[0] = vector[idx++];
  obj.rigid_body.L[1] = vector[idx++];
  obj.rigid_body.L[2] = vector[idx++];
  
  RigidBody& rb = obj.rigid_body;
  
  rb.v = obj.rigid_body.P / rb.mass;
  rb.R = StateVector::quaternion_to_matrix(rb.q.normalize());
  rb.Ibodyinv = rb.Ibody.inv();
  rb.Iinv = (rb.R * rb.Ibodyinv) * rb.R.transpose();
  
  rb.omega = rb.Iinv * rb.L;
  
}

StateVector operator*(double s, const StateVector& v){
  StateVector r(v.N);
  for (int i = 0; i < v.N; ++i ) {
    r[i] = s * v.v[i];
  }
  return r;
}

StateVector operator*(const StateVector& v, double s) {
  StateVector r(v.N);
  for (int i = 0; i < v.N; ++i ) {
    r[i] = s * v.v[i];
  }
  return r;
}

StateVector operator+(const StateVector& v1, const StateVector& v2){
  StateVector r(v1.N);
  for (int i = 0; i < v2.N; ++i ) {
    r[i] = v1.v[i] + v2.v[i];
  }
  return r;
}