//
//  objects.h
//  PhysicalAnimation
//
//  Created by Julian Wu on 9/4/12.
//  Copyright (c) 2012 Julian Wu. All rights reserved.
//

#ifndef PhysicalAnimation_objects_h
#define PhysicalAnimation_objects_h

#include<limits>
#include"MovementStrategy.h"

namespace physical_objects {


//  Object abstract class that represent all objects in the physical world
//  the elasticity coeffcient should between (0.0, 1.0)
template<class MotionVector, class DoFVector>
class Object {
public:
  enum ObjectType { UnAccess = 0, SphericalObject = 11, };
  
  Object();
  
  // a const identifier number of the object, every object should have one
  // permenant identifier 
  const long identifier() {
    return this->identifier_;
  }
  
  virtual float mass() = 0;
  // elasticity coeffieicnt 
  virtual float elasticity() = 0;
  
  virtual motion_strategies::IMovementStrategy<MotionVector, DoFVector>&
    motion_strategy() = 0;
  
  static const ObjectType object_type_id_;
private:
  const long identifier_;
};


//  Particle objects that havs no shape and be totally rigid
//class ParticleObject : Object {
//public:
//  virtual float mass() = 0;
//  virtual float elasticity() = 0;
//};

// An abstract class that represent shperical object with radius
template<class MotionVector, class DoFVector>
class SphericalObject : public Object<MotionVector, DoFVector> {
public:
  SphericalObject(float mass, float elasticity, float radius) :
    mass_(mass), elasticity_(elasticity), radius_(radius) {}
  
  float mass() {
    return this->mass_;
  }
  float elasticity() {
    return this->elasticity_;
  }
  
  float radius() {
    return this->radius_;
  }
private:
  float mass_, elasticity_, radius_;
};


// A 2D ball
template<class MotionVector, class DoFVector>
class Ball2D: public SphericalObject<MotionVector, DoFVector> {
public:
  Ball2D(float mass, float elasticity, float radius,
         MotionVector init_loc,
         MotionVector init_A,
         MotionVector init_V,
         physical_world::Medium<MotionVector> & medium) ;
  
  motion_strategies::IMovementStrategy<MotionVector, DoFVector>&
    motion_strategy() {
      return this->motion_strategy_;
  }
  
  static const typename Object<MotionVector, DoFVector>::ObjectType
    object_type_id_;
private:
  motion_strategies::StandardSphericalMovement<MotionVector, DoFVector>
    motion_strategy_;
};
  
//  An immortal object
template<class MotionVector, class DoFVector>
class ImmortalObject: Object<MotionVector, DoFVector> {
public:
  float mass() {
    return this->mass_;
  }
  float elasticity() {
    return this->elasticity_;
  }
  
private:
  float mass_ = std::numeric_limits<float>::max(),
        elasticity_ = 1.0f;
};

} //ns physical_objects



#endif
