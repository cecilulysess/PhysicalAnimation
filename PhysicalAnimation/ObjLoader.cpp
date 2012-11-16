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

//=======================ODE============================

//receive an initial state vector y0, ode calculate the value using dydt
// function to get dy(t)/dt
void ODE(StateVector& y0, StateVector& yend,
         double t0, double dt, MotionController& controller, dydt_func dydt){
  StateVector K1(y0), K2(y0), K3(y0), K4(y0);
  
  (controller.*dydt)(t0, y0, K1);
  StateVector tmp = K1 * dt;
  K1 = tmp;
//  K2 = calculate_dynamics(X + K1 * 0.5, t + 0.5 * dt) * dt;
//  K3 = calculate_dynamics(X + K2 * 0.5, t + 0.5 * dt) * dt;
//  K4 = calculate_dynamics(X + K3, t + dt) * dt;
  yend = y0 + K1;//res = X + (K1 + K2 * 2 + K3 * 2 + K4) * (1.0 / 6.0);
  
}

//===================MotionController==================
void MotionController::next_step() {
  
  StateVector& y0 = StateVector::RigidBody_State_to_Array(*object);
  
  // create a tmp statevector to receive the result
  StateVector yend = StateVector(y0);
  
  // calculate the result by ode
  ODE(y0, yend, current_time, current_time + dt, *this, &MotionController::dydt);
  
  // re assign the state vector back to the original object
  StateVector::RigidBody_Array_to_state(*object, yend);
  object->make_array();
}

void MotionController::compute_force_and_torque(double t) {
  object->rigid_body.force = Vector3d(1,1,1);
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
  
}

StateVector& StateVector::RigidBody_State_to_Array(ModelObject& obj){
  StateVector *ss = new StateVector(18);
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
  
  
  obj.rigid_body.P[0] = vector[idx++];
  obj.rigid_body.P[1] = vector[idx++];
  obj.rigid_body.P[2] = vector[idx++];
  
  obj.rigid_body.L[0] = vector[idx++];
  obj.rigid_body.L[1] = vector[idx++];
  obj.rigid_body.L[2] = vector[idx++];
  
  RigidBody& rb = obj.rigid_body;
  
  rb.v = obj.rigid_body.P / rb.mass;
  rb.R = StateVector::quaternion_to_matrix(rb.q.normalize());
  rb.Iinv = (rb.R * rb.Ibodyinv) * rb.R.transpose();
  
  rb.omega = rb.Iinv * rb.L;
  
}

StateVector operator*(double s, const StateVector& v){
  return StateVector(static_cast<const Vector&>(v) * s);
}