//
//  objects.cpp
//  PhysicalAnimation
//
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//  
//  Author: cosmobserver@gmail.com (Julian Wu )
//    9/7/12.

#include "objects.h"

#include<time.h>
#include<stdexcept>
namespace physical_objects {
  inline long get_random () {
    srand(time(NULL));
    return rand();
  }
  template<class MotionVector, class DoFVector>
  const typename Object<MotionVector,DoFVector>::ObjectType
    Object<MotionVector,DoFVector>::object_type_id_ =
    Object::ObjectType::UnAccess;
  
  template<class MotionVector, class DoFVector>
  Object<MotionVector,DoFVector>::Object():identifier_(get_random()){}
  
  template<class MotionVector, class DoFVector>
  Ball2D<MotionVector,DoFVector>::Ball2D(float mass, float elasticity, float radius,
                                               MotionVector init_loc,
                                               MotionVector init_A,
                                               MotionVector init_V,
                                               physical_world::Medium<MotionVector> & medium) :
    SphericalObject<MotionVector, DoFVector>(mass,elasticity,radius),
    motion_strategy_(init_V, init_A, init_loc, NULL, medium){
      if (elasticity <= 0.0f) {
        throw std::invalid_argument("elasticity should larger than 0.0");
      }
      if (radius <= 0.0f ) {
        throw std::invalid_argument("radius should larger than 0.0");
      }
      
  }
  
  template<class MotionVector, class DoFVector>
  const typename Object<MotionVector,DoFVector>::ObjectType
    Ball2D<MotionVector,DoFVector>::object_type_id_ =
    Object<MotionVector,DoFVector>::ObjectType::SphericalObject;
}