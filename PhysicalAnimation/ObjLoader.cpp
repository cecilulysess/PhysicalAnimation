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
std::vector<ModelObject*> ObjLoader::objects;

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
  }
  
  return objects[0];
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