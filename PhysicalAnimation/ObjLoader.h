//
//  ObjLoader.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 11/7/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef __PhysicalAnimation__ObjLoader__
#define __PhysicalAnimation__ObjLoader__

#include <iostream>
#include <vector>
#include "Vector.h"
#include "object.h"

class ObjLoader{
public:
  static std::vector<physical_objects::ModelObject*> objects;
  //read object into an array, if file have problem, return NULL
  // return obj numbers and its obj
  static physical_objects::ModelObject* loadObject(char const *path, int &obj_no);
  
private:
  
};


#endif /* defined(__PhysicalAnimation__ObjLoader__) */
