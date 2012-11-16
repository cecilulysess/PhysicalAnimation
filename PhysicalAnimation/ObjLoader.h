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

#include <iostream>
#include <vector>
#include "Vector.h"

// a defined model
class ModelObject {
public:
  
  std::vector<GLfloat> vertices;
  std::vector<GLubyte> indices;
  std::string objectName;
  ModelObject(char const *obj_name){
    objectName = std::string(obj_name);
    body_array = (double *)malloc(sizeof(double) * STATE_SIZE);
    body_array_dot = (double *)malloc(sizeof(double) * STATE_SIZE);
  }
  
  void make_array(){
    this->vertices_array = (GLfloat*) malloc(sizeof(GLfloat) * vertices.size());
    this->indice_array =  (GLubyte*) malloc(sizeof(GLubyte) * indices.size());
    for (int i = 0; i < vertices.size(); ++i) {
      vertices_array[i] = vertices[i];
    }
    for (int i = 0; i < indices.size(); ++i) {
      indice_array[i] = indices[i];
    }
  }
  
  void print(){
    std::cout<<"Object Name: "<<this->objectName<<", vertices #: " <<
    this->vertices.size()<<", indice #:"<<this->indices.size()<<std::endl;
    for(int i = 0; i < vertices.size(); i+=3){
      printf("v %f %f %f\n", vertices_array[i], vertices_array[i+1], vertices_array[i+2]);
    }
    for(int i = 0; i < indices.size(); i+=4){
      printf("f %d %d %d %d\n", indice_array[i], indice_array[i+1], indice_array[i+2], indice_array[i+3]);
    }
  }
  
  // rotate this object along vector(x,y,z) for degree
  void rotate(float degree, float x, float y, float z) ;
  
  double *body_array, *body_array_dot;
  GLfloat *vertices_array;
  GLubyte *indice_array;
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
