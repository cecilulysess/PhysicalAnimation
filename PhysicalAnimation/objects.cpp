//
//  objects.cpp
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/7/12.

#include "objects.h"

#include<stdexcept>
namespace physical_objects {
  
  Ball2D::Ball2D(float mass, float elasticity, float radius ) :
    mass_(mass), elasticity_(elasticity), radius_(radius) {
      if (elasticity <= 0.0f) {
        throw std::invalid_argument("elasticity should larger than 0.0");
      }
      if (radius <= 0.0f ) {
        throw std::invalid_argument("radius should larger than 0.0");
      }
  }

}