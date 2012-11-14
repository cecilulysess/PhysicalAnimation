//
//  ObjLoader.cpp
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/7/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#include "ObjLoader.h"

#include <stdio.h>

physical_objects::ModelObject* ObjLoader::loadObject(char const *path){
  FILE *file ;
  if(!(file= fopen(path, "r"))){
    return NULL;
  }
  char identifier[255];
  char objectname[255];
  float value1, value2, value3;
  int v1, vt1, v2, vt2, v3, vt3, v4, vt4;
  physical_objects::ModelObject* obj1;
  fscanf(file, "%s", identifier);
  int i = 0;
  while( !feof(file) ){
    printf("%d: ", i);
    i++;
    switch (identifier[0]) {
        case '#':
        printf("#\n");
        fgets(identifier, 100, file);
        break;
        
        case 'o':
        fscanf(file, "%s", objectname);
        obj1 = new physical_objects::ModelObject(objectname);
        printf("\tRead Object:%s\n", objectname);
        fgets(identifier, 100, file);
        break;
        
        case 'v':
        if (identifier[1] == ' ') {
          fscanf(file, "%f %f %f", &value1, &value2, &value3 );
          obj1->vertices.push_back(value1);
          obj1->vertices.push_back(value2);
          obj1->vertices.push_back(value3);
        } else {
          printf("vt\n");
          // it is vt
        }
        fgets(identifier, 100, file);
        break;
        
        case 'u':
        printf("u\n");
        // texture image?
        fgets(identifier, 100, file);
        break;
        case 's':
        printf("s\n");
        fgets(identifier, 100, file);
        // skip s line
        break;
        case 'f':
        //load face indice
//        printf("f\n");
        fscanf(file, "%d/%d %d/%d %d/%d %d/%d",
               &v1, &vt1, &v2, &vt2, &v3, &vt3, &v4, &vt4);
        obj1->indices.push_back(v1);
        obj1->indices.push_back(v2);
        obj1->indices.push_back(v3);
        obj1->indices.push_back(v4);
        break;
        case 'm':
        printf("m\n");
        fgets(identifier, 100, file);
        break;
    }
    fgets(identifier, 3, file);
  }
  return obj1;
}